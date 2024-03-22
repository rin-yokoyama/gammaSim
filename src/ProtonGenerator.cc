#include "ProtonGenerator.hh"
#include <iostream>
#include <fstream>
#include <sstream>
#include <numeric>
#include <math.h>

ProtonGenerator::ProtonGenerator() : total_cs_(0), v_cm_(0) {}

ProtonGenerator::~ProtonGenerator() {}

int ProtonGenerator::ReadFile(std::string fname)
{
    std::ifstream fin(fname);
    if (!fin)
    {
        std::cout << "Cannot open file:" << fname << std::endl;
        return 1;
    }
    double deg_cm, deg_lab, en_p, deg_rec, en_rec, cs;
    std::string line;
    Clear();
    while (std::getline(fin, line))
    {
        std::istringstream iss(line);
        iss >> deg_cm >> deg_lab >> en_p >> deg_rec >> en_rec >> cs;
        deg_lab_.emplace_back(deg_lab_);
        en_p_.emplace_back(en_p);
        cross_section_.emplace_back(cs);
    }
    total_cs_ = std::reduce(cross_section_.begin(), cross_section_.end());
    return 0;
}

double ProtonGenerator::CalculateVCM()
{
    const int numberOfNucleon = 148;
    const double beamEnergy = 40.0 * numberOfNucleon;                                  // MeV
    const double protonMass = 938.27;                                                  // MeV/c^2
    const double neutronMass = 939.57;                                                 // MeV/c^2
    const double beamMassExcess = 70.398;                                              // MeV/c^2
    const double beamStaticMass = protonMass * 58 + neutronMass * 90 - beamMassExcess; // MeV/c^2
    const double targetStaticMass = protonMass;                                        // MeV/c^2
    const double beamGamma = beamEnergy / beamStaticMass + 1;
    const double beamVelocity = sqrt(1. - 1. / (beamGamma * beamGamma));
    const double beamMomentum = beamGamma * beamVelocity * beamStaticMass;
    const double centerOfMassVelocity = 2. * beamMomentum / (targetStaticMass + beamStaticMass + beamMomentum * beamMomentum);
    return centerOfMassVelocity;
}

void ProtonGenerator::Clear()
{
    deg_lab_.clear();
    en_p_.clear();
    cross_section_.clear();
}