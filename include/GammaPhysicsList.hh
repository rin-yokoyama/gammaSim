#ifndef __GAMMA_PHYSICS_LIST_HH__
#define __GAMMA_PHYSICS_LIST_HH__

#include "G4VModularPhysicsList.hh"
#include "G4EmStandardPhysics_option4.hh"

class GammaPhysicsList : public G4VModularPhysicsList
{
public:
    GammaPhysicsList();
    virtual ~GammaPhysicsList() {};
};

GammaPhysicsList::GammaPhysicsList() : G4VModularPhysicsList()
{
    defaultCutValue = 0.001 * mm;
    RegisterPhysics(new G4EmStandardPhysics_option4());
}
#endif
