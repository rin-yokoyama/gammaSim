#ifndef __EXP_CONSTANTS_HH__
#define __EXP_CONSTANTS_HH__

#include "G4SystemOfUnits.hh"
#include "G4RotationMatrix.hh"

namespace B1
{
    const G4double kWorldSize = 200. * cm;
    const G4int kNSiStrips = 128;
    const G4double kSiSize = 10. * cm;
    const G4double kSiThickness = 0.3 * mm;
    const G4double kSiXOffset = 0.0 * cm;
    const G4double kSiYOffset = 1.0 * cm;
    const G4double kSiZOffset = 5.0 * cm;
    const G4double kCsISize = 5.0 * cm;
    const G4double kCsIThickness = 2.0 * cm;
    const G4double kCsIZOffset = 1.0 * mm;
    static G4RotationMatrix kRotation(0, -90 * deg, 0);
    static G4ThreeVector kPosition(0, 1 * cm, 12 * cm);
    const bool kLimitTo2Pi = true;
}

#endif