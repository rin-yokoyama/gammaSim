#include "GammaGenerator.hh"
#include <fstream>
#include <sstream>
#include <numeric>
#include <math.h>
#include "ExpConstants.hh"
#include "InitParticleEventInfo.hh"

GammaGenerator::GammaGenerator() : point_source_(true)
{
}

GammaGenerator::~GammaGenerator()
{
}

void GammaGenerator::SetSize(const double &size_x, const double &size_y, const double &size_z)
{
    sizeX_ = size_x;
    sizeY_ = size_y;
    sizeZ_ = size_z;
    point_source_ = false;
}

int GammaGenerator::ReadFile(const std::string &fname)
{
    gamma_source_ = std::make_unique<GSource4G4>(fname);
    return 0;
}

void GammaGenerator::Clear()
{
    point_source_ = true;
}

void GammaGenerator::SetParticles(G4ParticleGun *gun, G4Event *anEvent)
{
    auto info = new InitParticleEventInfo();

    gamma_source_->EmitGamma();
    auto energies = gamma_source_->GetEGamma();
    for (const auto &en : energies)
    {
        // Energy
        gun->SetParticleEnergy(en);

        // Direction
        G4ThreeVector direction(G4RandomDirection());
        gun->SetParticleMomentumDirection(direction);

        // Position
        G4ThreeVector position(0, 0, 0);
        if (point_source_)
        {
        }
        else
        {
            position.setX((G4UniformRand() - 0.5) * sizeX_ * mm);
            position.setY((G4UniformRand() - 0.5) * sizeY_ * mm);
            position.setZ((G4UniformRand() - 0.5) * sizeZ_ * mm);
        }
        gun->SetParticlePosition(position);

        gun->GeneratePrimaryVertex(anEvent);
        info->AppendData(en, direction, position);
    }
    anEvent->SetUserInformation(info);
}