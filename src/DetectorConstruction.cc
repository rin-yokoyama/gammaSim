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
/// \file B1/src/DetectorConstruction.cc
/// \brief Implementation of the B1::DetectorConstruction class

#include "DetectorConstruction.hh"

#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4Trd.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"

namespace B1
{

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  G4VPhysicalVolume *DetectorConstruction::Construct()
  {
    // Get nist material manager
    G4NistManager *nist = G4NistManager::Instance();

    // Option to switch on/off checking of volumes overlaps
    //
    G4bool checkOverlaps = true;

    //
    // World
    //
    G4double world_sizeXY = 10 * cm;
    G4double world_sizeZ = 10 * cm;
    G4Material *world_mat = nist->FindOrBuildMaterial("G4_AIR");

    auto solidWorld = new G4Box("World",                                                    // its name
                                0.5 * world_sizeXY, 0.5 * world_sizeXY, 0.5 * world_sizeZ); // its size

    auto logicWorld = new G4LogicalVolume(solidWorld, // its solid
                                          world_mat,  // its material
                                          "World");   // its name

    auto physWorld = new G4PVPlacement(nullptr,         // no rotation
                                       G4ThreeVector(), // at (0,0,0)
                                       logicWorld,      // its logical volume
                                       "World",         // its name
                                       nullptr,         // its mother  volume
                                       false,           // no boolean operation
                                       0,               // copy number
                                       checkOverlaps);  // overlaps checking

    G4Material *gps = nullptr;
    {
      const int ncomp = 3;
      const G4double density = 5.3 * g / cm3;
      gps = new G4Material("LaGPS", density, ncomp);
      auto elemGd = new G4Element("Gadolinium", "Gd", 64., 157.25 * g / mole);
      auto elemSi = new G4Element("Silicon", "Si", 14., 28.0 * g / mole);
      auto elemO = new G4Element("Gadolinium", "Gd", 8., 16.00 * g / mole);
      gps->AddElement(elemGd, 2);
      gps->AddElement(elemSi, 2);
      gps->AddElement(elemO, 5);
    }

    //
    // Shape 1
    //
    G4ThreeVector pos1 = G4ThreeVector(0, 0, 0);

    const G4double shape1_size = 5.0 * mm;
    auto sampleCrystalSolid = new G4Box("Shape1", 0.5 * shape1_size, 0.5 * shape1_size, 0.5 * shape1_size);

    auto sampleCrystalLogic = new G4LogicalVolume(sampleCrystalSolid, // its solid
                                                  gps,                // its material
                                                  "SampleCrystal");   // its name

    new G4PVPlacement(nullptr,            // no rotation
                      pos1,               // at position
                      sampleCrystalLogic, // its logical volume
                      "SampleCrystal",    // its name
                      logicWorld,         // its mother  volume
                      false,              // no boolean operation
                      0,                  // copy number
                      checkOverlaps);     // overlaps checking

    // Set Shape2 as scoring volume
    //
    fScoringVolume = sampleCrystalLogic;

    //
    // always return the physical World
    //
    return physWorld;
  }

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

}
