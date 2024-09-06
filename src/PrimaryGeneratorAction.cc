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
/// \file B1/src/PrimaryGeneratorAction.cc
/// \brief Implementation of the B1::PrimaryGeneratorAction class

#include "PrimaryGeneratorAction.hh"

#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4RunManager.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"
#include "InitParticleEventInfo.hh"

namespace B1
{

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  PrimaryGeneratorAction::PrimaryGeneratorAction()
  {
    G4int n_particle = 1;
    fParticleGun = new G4ParticleGun(n_particle);

    // default particle kinematic
    G4ParticleTable *particleTable = G4ParticleTable::GetParticleTable();
    G4String particleName;
    G4ParticleDefinition *particle = particleTable->FindParticle(particleName = "proton");
    fParticleGun->SetParticleDefinition(particle);

    fProtonGenerator = new ProtonGenerator;
    fProtonGenerator->Clear();
    fProtonGenerator->ReadFile("work/generated_data_3n.csv");
  }

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  PrimaryGeneratorAction::~PrimaryGeneratorAction()
  {
    delete fParticleGun;
  }

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  void PrimaryGeneratorAction::GeneratePrimaries(G4Event *anEvent)
  {
    // this function is called at the begining of ecah event
    //

    // In order to avoid dependence of PrimaryGeneratorAction
    // on DetectorConstruction class we get Envelope volume
    // from G4LogicalVolumeStore.

    G4ThreeVector direction;
    G4ThreeVector position;
    double energy;
    fProtonGenerator->SetParticle(direction, energy, position);
    anEvent->SetUserInformation(new InitParticleEventInfo(energy, direction.getTheta(), direction.getPhi()));
    fParticleGun->SetParticleMomentumDirection(direction);
    fParticleGun->SetParticleEnergy(energy);
    fParticleGun->SetParticlePosition(position);
    fParticleGun->GeneratePrimaryVertex(anEvent);
  }

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

}
