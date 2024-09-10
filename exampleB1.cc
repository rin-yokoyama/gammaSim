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
/// \file exampleB1.cc
/// \brief Main program of the B1 example

#include "DetectorConstruction.hh"
#include "ActionInitialization.hh"

#include "G4RunManagerFactory.hh"
#include "G4SteppingVerbose.hh"
#include "G4UImanager.hh"

#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

#include "Randomize.hh"

#include "ExpConstants.hh"
#include "GammaGenerator.hh"
#include "LinuxDirectoryFunctions.hh"
#include "GammaPhysicsList.hh"

using namespace B1;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int main(int argc, char **argv)
{
  // Detect interactive mode (if no arguments) and define UI session
  //
  std::string outFileName = "";
  G4UIExecutive *ui = nullptr;
  if (argc == 1)
  {
    ui = new G4UIExecutive(argc, argv);
  }
  else if (argc == 2)
  {
    std::cout << "Usage: exampleB1 run.mac [output_dir]" << std::endl;
    return 1;
  }
  else
  {
    outFileName = argv[2];
  }
  // Optionally: choose a different Random engine...
  // G4Random::setTheEngine(new CLHEP::MTwistEngine);

  // use G4SteppingVerboseWithUnits
  // G4int precision = 4;
  // G4SteppingVerbose::UseBestUnit(precision);

  // Construct the default run manager
  //
  auto *runManager =
      G4RunManagerFactory::CreateRunManager(G4RunManagerType::MT, 1);

  // #ifdef G4MULTITHREADED
  // runManager->SetNumberOfThreads(1);
  // #endif
  //  Set mandatory initialization classes
  //
  //  Detector construction
  runManager->SetUserInitialization(new DetectorConstruction());

  // Physics list
  G4VModularPhysicsList *physicsList = new GammaPhysicsList();
  physicsList->SetVerboseLevel(1);
  runManager->SetUserInitialization(physicsList);

  // User action initialization
  runManager->SetUserInitialization(new ActionInitialization(outFileName));

  // Initialize visualization
  //
  G4VisManager *visManager = new G4VisExecutive;
  // G4VisExecutive can take a verbosity argument - see /vis/verbose guidance.
  // G4VisManager* visManager = new G4VisExecutive("Quiet");
  visManager->Initialize();

  // Get the pointer to the User Interface manager
  G4UImanager *UImanager = G4UImanager::GetUIpointer();

  // Process macro or start UI session
  //
  if (!ui)
  {
    // batch mode
    UImanager->ApplyCommand("/control/macroPath ./");
    G4String command = "/control/execute ";
    std::string macFileName = argv[1];
    if (!createOrCleanDirectory(outFileName))
      return 1;
    if (!createOrCleanDirectory(outFileName + "/detectorData"))
      return 1;
    if (!createOrCleanDirectory(outFileName + "/particleData"))
      return 1;
    UImanager->ApplyCommand(command + macFileName);
  }
  else
  {
    // interactive mode
    UImanager->ApplyCommand("/control/execute init_vis.mac");
    ui->SessionStart();
    delete ui;
  }

  // Job termination
  // Free the store: user actions, physics_list and detector_description are
  // owned and deleted by the run manager, so they should not be deleted
  // in the main() program !

  delete visManager;
  delete runManager;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....
