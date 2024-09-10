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
  }

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  void RunAction::BeginOfRunAction(const G4Run *)
  {
    // inform the runManager to save random number seed
    G4RunManager::GetRunManager()->SetRandomNumberStore(false);
    const u_int64_t nevent = G4RunManager::GetRunManager()->GetNumberOfEventsToBeProcessed();
    const int nworkers = G4RunManager::GetRunManager()->GetNumberOfThreads();
    const u_int64_t nevnet_per_worker = nevent / nworkers;

    // Initialize Array builders
    arrayBuilder_.Init();

    worker_id_ = G4Threading::G4GetThreadId();
    worker_event_ = 0;
  }

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  void RunAction::EndOfRunAction(const G4Run *run)
  {
    G4int nofEvents = run->GetNumberOfEvent();
    if (nofEvents == 0)
      return;
    if (file_prefix_ != "")
    {
      {
        std::string fname = file_prefix_ + "/particleData/worker" + std::to_string(G4Threading::G4GetThreadId()) + ".parquet";
        arrayBuilder_.FinalizeParticleData(fname);
      }
      {
        std::string fname = file_prefix_ + "/detectorData/worker" + std::to_string(G4Threading::G4GetThreadId()) + ".parquet";
        arrayBuilder_.FinalizeDetectorData(fname);
      }
    }

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
    arrayBuilder_.FillEDep(worker_id_, worker_event_, detName, eDep, copyNum);
  }

  void RunAction::AddEventInfo(const double &energy, const G4double &theta, const G4double &phi, const G4ThreeVector &vec)
  {
    arrayBuilder_.FillParticle(worker_id_, worker_event_, "gamma", energy, theta, phi, vec.getX(), vec.getY(), vec.getZ());
  }
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
}