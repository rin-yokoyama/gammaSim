#ifndef __EXP_CONSTANTS_HH__
#define __EXP_CONSTANTS_HH__

#include "G4SystemOfUnits.hh"
#include "G4RotationMatrix.hh"

namespace B1
{
    const G4String kSourceFile = "./gamma_definitions/60Co.txt";
    const G4double kWorldSize = 200. * cm;
    const G4double kdeadthickness = 2.5 * mm;
    const G4double kGeRadius = ((57.1 * mm) / 2) - (0 * mm);
    const G4double kGeLength = (54.6 * mm) - (B1::kdeadthickness * 2);
    const G4double kGeHoleRadiusRatio = 0.17;
    const G4double kGeHoleLengthRatio = 0.85;
    const G4double kdeadradius = (57.1 * mm) / 2.;
    const G4double kdeadlength = 54.6 * mm;
    const G4double kWindowThickness = 1.5 * mm;
    static G4ThreeVector kWindowPos(0, 0, (B1::kGeLength / 2.0) + kdeadthickness + (B1::kWindowThickness / 2.) + 3. * mm);
    // const G4double kPlasticThickness = 3.0 * mm;
    // static G4ThreeVector kPlasticPos(0, 0, (B1::kGeLength / 2.0) + kdeadthickness + kWindowThickness + 3. * mm - 3. * mm + (kPlasticThickness / 2.0));
    const G4bool kUseSource = true; // otherwise array
    const G4double kSourceSizeX = 23.5 * mm;
    const G4double kSourceSizeY = 2.1 * mm;
    const G4double kSourceSizeZ = 11.3 * mm;
    const G4double kArraySizeX = 50.0 * mm;
    const G4double kArraySizeY = 2.5 * mm;
    const G4double kArraySizeZ = 50.0 * mm;
    static G4RotationMatrix kDetRotation(0, -90 * deg, 0);
    static G4ThreeVector kDetPosition(0, -((B1::kGeLength / 2.0) + kdeadthickness + 4.5 * mm + 10 * mm), 0); // mother volume position
    static G4RotationMatrix kSampleRotation(0, 0, 0);
    static G4ThreeVector kSamplePosition(0, (kSourceSizeY / 2.), 0);
    static const bool k60CoGammaAngularCorrelation = true; // on only for 60Co
}

#endif
