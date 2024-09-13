#ifndef __INIT_PARTICLE_EVENT_INFO_HH__
#define __INIT_PARTICLE_EVENT_INFO_HH__

#include "G4VUserEventInformation.hh"
#include "G4ThreeVector.hh"
#include <vector>
class InitParticleEventInfo : public G4VUserEventInformation
{
public:
    InitParticleEventInfo() : eGammas_(), directions_(), positions_() {};
    ~InitParticleEventInfo() {};

    void AppendData(const double &e, const G4ThreeVector dir, const G4ThreeVector pos)
    {
        eGammas_.emplace_back(e);
        directions_.emplace_back(dir);
        positions_.emplace_back(pos);
    }
    std::vector<double> GetGammaEnergy() const { return eGammas_; }
    std::vector<G4ThreeVector> GetDirections() const { return directions_; }
    std::vector<G4ThreeVector> GetPositions() const { return positions_; }
    void Clear()
    {
        eGammas_.clear();
        directions_.clear();
        positions_.clear();
    }

    void Print() const override {};

private:
    std::vector<double> eGammas_;
    std::vector<G4ThreeVector> directions_;
    std::vector<G4ThreeVector> positions_;
};

#endif