#ifndef __GAMMA_GENERATOR_HH__
#define __GAMMA_GENERATOR_HH__
#include <iostream>
#include <vector>
#include <string>
#include <G4ThreeVector.hh>
#include <G4RandomDirection.hh>
#include <Randomize.hh>
#include <G4ParticleGun.hh>
#include <G4Event.hh>
#include "GSource4G4.hh"

class GammaGenerator
{
public:
    GammaGenerator();
    virtual ~GammaGenerator();
    int ReadFile(const std::string &fname);
    void Clear();
    void SetSize(const double &size_x, const double &size_y, const double &size_z);
    void SetParticles(G4ParticleGun *gun, G4Event *anEvent);

protected:
    bool point_source_;
    double sizeX_;
    double sizeY_;
    double sizeZ_;
    std::unique_ptr<GSource4G4> gamma_source_;
};
#endif
