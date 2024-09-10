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
#include "G4Tubs.hh"
#include "G4Sphere.hh"
#include "G4Trd.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "ExpConstants.hh"
#include "G4Material.hh"
#include "G4VisAttributes.hh"

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
    G4double world_sizeXY = B1::kWorldSize;
    G4double world_sizeZ = B1::kWorldSize;
    G4Material *world_mat = nist->FindOrBuildMaterial("G4_AIR");
    G4Material *det_mat = nist->FindOrBuildMaterial("G4_Galactic");

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

    // Mother logical volume for detector geometry
    auto solidDet = new G4Box("Detector",                                                 // its name
                              0.1 * world_sizeXY, 0.1 * world_sizeXY, 0.1 * world_sizeZ); // its size
    auto logicDet = new G4LogicalVolume(solidDet, det_mat, "detectorMother");
    G4VisAttributes *invisibleAttributes = new G4VisAttributes();
    invisibleAttributes->SetVisibility(false);
    logicDet->SetVisAttributes(invisibleAttributes);

    // Mother logical volume for sample geometry
    auto solidSample = new G4Box("Sample",                                                   // its name
                                 0.1 * world_sizeXY, 0.1 * world_sizeXY, 0.1 * world_sizeZ); // its size
    auto logicSample = new G4LogicalVolume(solidSample, det_mat, "sampleMother");
    logicSample->SetVisAttributes(invisibleAttributes);

    if (!B1::kUseSource)
    {
      // La-GPS
      G4Material *gps = nullptr;
      {
        const int ncomp = 3;
        const G4double density = 5.3 * g / cm3;
        gps = new G4Material("LaGPS", density, ncomp);
        auto elemGd = new G4Element("Gadolinium", "Gd", 64., 157.25 * g / mole);
        auto elemSi = new G4Element("Silicon", "Si", 14., 28.0 * g / mole);
        auto elemO = new G4Element("Oxygen", "O", 8., 16.00 * g / mole);
        gps->AddElement(elemGd, 2);
        gps->AddElement(elemSi, 2);
        gps->AddElement(elemO, 5);
      }
      auto boxArray = new G4Box("Array", B1::kArraySizeX / 2., B1::kArraySizeY / 2., B1::kArraySizeZ / 2.);
      auto logicArray = new G4LogicalVolume(boxArray, gps, "Array");
      G4ThreeVector array_pos(0, B1::kArraySizeY / 2., 0);
      new G4PVPlacement(nullptr, array_pos, logicArray, "array", logicSample, false, 0, checkOverlaps);
    }
    else
    {
      /// Source
      G4Material *source_mat = nist->FindOrBuildMaterial("G4_POLYETHYLENE");
      auto boxSource = new G4Box("Source", B1::kSourceSizeX / 2., B1::kSourceSizeY / 2., B1::kSourceSizeZ / 2.);
      auto logicSource = new G4LogicalVolume(boxSource, source_mat, "Source");
      G4ThreeVector source_pos(0, B1::kSourceSizeY / 2., 0);
      new G4PVPlacement(nullptr, source_pos, logicSource, "source", logicSample, false, 0, checkOverlaps);
    }

    /// Ge crystal
    G4Material *ge_mat = nist->FindOrBuildMaterial("G4_Ge");

    auto geTube = new G4Tubs("Ge", 0, B1::kGeRadius, B1::kGeLength / 2., 2 * M_PI, 2 * M_PI);

    auto geLogic = new G4LogicalVolume(geTube, // its solid
                                       ge_mat, // its material
                                       "Ge");  // its name

    G4VisAttributes *detVisAttributes = new G4VisAttributes();
    detVisAttributes->SetColor(0, 1, 1, 0.8);
    geLogic->SetVisAttributes(detVisAttributes);

    {
      // Place Ge in the detector mother volume
      new G4PVPlacement(nullptr,                // no rotation
                        G4ThreeVector(0, 0, 0), // at position
                        geLogic,                // its logical volume
                        "Ge",                   // its name
                        logicDet,               // its mother  volume
                        false,                  // no boolean operation
                        0,                      // copy number
                        checkOverlaps);         // overlaps checking
    }
    // Set siStip as scoring volume
    //
    fScoringVolume = geLogic;

    /// Ge window
    auto window_mat = nist->FindOrBuildMaterial("G4_Mg");
    auto windowTube = new G4Tubs("Window", 0, B1::kGeRadius * 1.1, B1::kWindowThickness, 2 * M_PI, 2 * M_PI);
    auto windowLogic = new G4LogicalVolume(windowTube, window_mat, "Window");
    new G4PVPlacement(nullptr, B1::kWindowPos, windowLogic, "window", logicDet, false, 0);
    G4VisAttributes *windowVisAttributes = new G4VisAttributes();
    windowVisAttributes->SetColor(1, 1, 0, 0.8);
    windowLogic->SetVisAttributes(windowVisAttributes);

    // Place the detector mother volume in the world volume
    new G4PVPlacement(&B1::kDetRotation, B1::kDetPosition, logicDet, "detector", logicWorld, false, 0);

    // Place the sample mother volume in the world volume
    new G4PVPlacement(&B1::kSampleRotation, B1::kSamplePosition, logicSample, "sample", logicWorld, false, 0);

    //
    // always return the physical World
    //
    return physWorld;
  }

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

}
