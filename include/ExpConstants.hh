#ifndef __EXP_CONSTANTS_HH__
#define __EXP_CONSTANTS_HH__

#include "G4SystemOfUnits.hh"
#include "G4RotationMatrix.hh"

namespace B1
{
    const G4String kSourceFile = "./gamma_definitions/60Co.txt";
    const G4double kWorldSize = 200. * cm;
    const G4double kGeRadius = 57.1 * mm;
    const G4double kGeLength = 54.6 * mm;
    const G4double kWindowThickness = 1.5 * mm;
    static G4ThreeVector kWindowPos(0, 0, B1::kGeLength / 2.0 + 3.0 * mm);
    const G4bool kUseSource = true; // otherwise array
    const G4double kSourceSizeX = 23.5 * mm;
    const G4double kSourceSizeY = 2.1 * mm;
    const G4double kSourceSizeZ = 11.3 * mm;
    const G4double kArraySizeX = 50.0 * mm;
    const G4double kArraySizeY = 2.5 * mm;
    const G4double kArraySizeZ = 50.0 * mm;
    static G4RotationMatrix kDetRotation(0, -90 * deg, 0);
    static G4ThreeVector kDetPosition(0, -(kWindowPos.getZ() + kWindowThickness), 0);
    static G4RotationMatrix kSampleRotation(0, 0, 0);
    static G4ThreeVector kSamplePosition(0, kSourceSizeY / 2., 0);
}

#endif