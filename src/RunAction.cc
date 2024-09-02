//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
/// \file B1/src/RunAction.cc
/// \brief Implementation of the B1::RunAction class

#include "RunAction.hh"
#include "PrimaryGeneratorAction.hh"
#include "DetectorConstruction.hh"
// #include "Run.hh"

#include "G4RunManager.hh"
#include "G4Run.hh"
#include "G4AccumulableManager.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

namespace B1
{

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  RunAction::RunAction(const std::string &file_prefix) : file_prefix_(file_prefix)
  {
    pool_ = arrow::default_memory_pool();
  }

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  void RunAction::BeginOfRunAction(const G4Run *)
  {
    // inform the runManager to save random number seed
    G4RunManager::GetRunManager()->SetRandomNumberStore(false);

    // Initialize Array builders
    builder_map_.clear();
    builder_map_["workerId"] = std::make_shared<arrow::Int32Builder>(pool_);
    builder_map_["eventId"] = std::make_shared<arrow::Int32Builder>(pool_);
    builder_map_["detName"] = std::make_shared<arrow::StringBuilder>(pool_);
    builder_map_["copyId"] = std::make_shared<arrow::Int32Builder>(pool_);
    builder_map_["eDep"] = std::make_shared<arrow::DoubleBuilder>(pool_);

    worker_id_ = G4Threading::G4GetThreadId();
    n_worker_event_ = 0;
  }

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  void RunAction::EndOfRunAction(const G4Run *run)
  {
    G4int nofEvents = run->GetNumberOfEvent();
    if (nofEvents == 0)
      return;

    // Finalize arrays
    std::vector<std::string> cols = {"workerId", "eventId", "detName", "copyId", "eDep"};
    arrow::ArrayVector arrayVec;
    for (const auto &col : cols)
    {
      std::shared_ptr<arrow::Array> array;
      PARQUET_THROW_NOT_OK(builder_map_[col]->Finish(&array));
      arrayVec.emplace_back(array);
    }

    // Create schema
    arrow::FieldVector fieldVec;
    fieldVec.emplace_back(std::make_shared<arrow::Field>("workerId", arrow::int32()));
    fieldVec.emplace_back(std::make_shared<arrow::Field>("eventId", arrow::int32()));
    fieldVec.emplace_back(std::make_shared<arrow::Field>("detName", arrow::utf8()));
    fieldVec.emplace_back(std::make_shared<arrow::Field>("copyId", arrow::int32()));
    fieldVec.emplace_back(std::make_shared<arrow::Field>("eDep", arrow::float64()));
    auto schema = arrow::schema(fieldVec);

    // Write table to file
    G4int threadId = G4Threading::G4GetThreadId();
    std::string filename = file_prefix_ + std::to_string(threadId) + ".parquet";
    auto table = arrow::Table::Make(schema, arrayVec);
    std::shared_ptr<arrow::io::FileOutputStream> outfile;
    PARQUET_ASSIGN_OR_THROW(
        outfile,
        arrow::io::FileOutputStream::Open(filename));

    PARQUET_THROW_NOT_OK(parquet::arrow::WriteTable(*table, pool_, outfile));

    // Run conditions
    //  note: There is no primary generator action object for "master"
    //        run manager for multi-threaded mode.
    const auto generatorAction = static_cast<const PrimaryGeneratorAction *>(
        G4RunManager::GetRunManager()->GetUserPrimaryGeneratorAction());
    G4String runCondition;
    if (generatorAction)
    {
      const G4ParticleGun *particleGun = generatorAction->GetParticleGun();
      runCondition += particleGun->GetParticleDefinition()->GetParticleName();
      runCondition += " of ";
      G4double particleEnergy = particleGun->GetParticleEnergy();
      runCondition += G4BestUnit(particleEnergy, "Energy");
    }

    // Print
    //
    if (IsMaster())
    {
      G4cout
          << G4endl
          << "--------------------End of Global Run-----------------------";
    }
    else
    {
      G4cout
          << G4endl
          << "--------------------End of Local Run------------------------";
    }

    G4cout
        << G4endl
        << " The run consists of " << nofEvents << " " << runCondition
        << G4endl;
  }
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  void RunAction::AddEdep(const std::string &detName, const G4double &eDep, G4int copyNum)
  {
    PARQUET_THROW_NOT_OK(static_cast<arrow::Int32Builder *>(builder_map_["workerId"].get())->Append(worker_id_));
    PARQUET_THROW_NOT_OK(static_cast<arrow::Int32Builder *>(builder_map_["eventId"].get())->Append(n_worker_event_));
    PARQUET_THROW_NOT_OK(static_cast<arrow::StringBuilder *>(builder_map_["detName"].get())->Append(detName));
    PARQUET_THROW_NOT_OK(static_cast<arrow::Int32Builder *>(builder_map_["copyNum"].get())->Append(copyNum));
    PARQUET_THROW_NOT_OK(static_cast<arrow::DoubleBuilder *>(builder_map_["eDep"].get())->Append(eDep));
  }
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
}