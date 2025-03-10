#ifndef __EXP_CONSTANTS_HH__
#define __EXP_CONSTANTS_HH__

#include "G4SystemOfUnits.hh"
#include "G4RotationMatrix.hh"

namespace B1
{
    // Gamma source definition file
    const G4String kSourceFile = "./gamma_definitions/60Co.txt";
    // Size of the World Volume
    const G4double kWorldSize = 200. * cm;
    // Thickness of the dead layer of the Ge detector
    const G4double kdeadthickness = 2.5 * mm;
    // Radius of the Ge crystal cylinder excluding the dead layer
    const G4double kGeRadius = ((57.1 * mm) / 2) - (0 * mm);
    // Length of the Ge crystal cylinder excluding the dead layer
    const G4double kGeLength = (54.6 * mm) - (B1::kdeadthickness * 2);
    // Radius of the Ge crystal including the dead layer
    const G4double kdeadradius = (57.1 * mm) / 2.;
    // Length of the Ge crystal including the dead layer
    const G4double kdeadlength = 54.6 * mm;
    // Ratio between the hole radius and crystal radius of the Ge detector
    const G4double kGeHoleRadiusRatio = 0.17;
    // Ratio between the hole length and crystal length of the Ge detector
    const G4double kGeHoleLengthRatio = 0.85;
    // Thickness of the Ge window
    const G4double kWindowThickness = 1.5 * mm;
    // Ge window position
    static G4ThreeVector kWindowPos(0, 0, (B1::kGeLength / 2.0) + kdeadthickness + (B1::kWindowThickness / 2.) + 3. * mm);
    // // Thickness of the plastic film on the Ge head
    // const G4double kPlasticThickness = 3.0 * mm;
    // // Position of the plastic film on the Ge head
    // static G4ThreeVector kPlasticPos(0, 0, (B1::kGeLength / 2.0) + kdeadthickness + kWindowThickness + 3. * mm - 3. * mm + (kPlasticThickness / 2.0));
    // True if you use a point source. Otherwise array
    const G4bool kUseSource = true;
    // X size of the plastic shielding of the source
    const G4double kSourceSizeX = 23.5 * mm;
    // Y size of the plastic shielding of the source
    const G4double kSourceSizeY = 2.1 * mm;
    // Z size of the plastic shielding of the source
    const G4double kSourceSizeZ = 11.3 * mm;
    // X size of the La-GPS array
    const G4double kArraySizeX = 50.0 * mm;
    // Y size of the La-GPS array
    const G4double kArraySizeY = 2.5 * mm;
    // Z size of the La-GPS array
    const G4double kArraySizeZ = 50.0 * mm;

    // Rotation for the detector mother volume
    static G4RotationMatrix kDetRotation(0, -90 * deg, 0);
    // Offset for the detector mother volume
    static G4ThreeVector kDetPosition(0, -((B1::kGeLength / 2.0) + kdeadthickness + 4.5 * mm + 10 * mm), 0);
    // Rotation for the sample volume
    static G4RotationMatrix kSampleRotation(0, 0, 0);
    // Offset for the sample volume
    static G4ThreeVector kSamplePosition(0, (kSourceSizeY / 2.), 0);
    // Turn it true ONLY WHEN USING 60Co SOURCE!!
    static const bool k60CoGammaAngularCorrelation = true;
}

#endif
