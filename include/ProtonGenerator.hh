#ifndef __PROTON_GENERATOR_HH__
#define __PROTON_GENERATOR_HH__
#include <iostream>
#include <vector>
#include <string>
#include <TH1D.h>
#include <TGraph.h>
#include <G4ThreeVector.hh>
#include <TRandom3.h>

class ProtonGenerator
{
public:
    ProtonGenerator();
    virtual ~ProtonGenerator();
    int ReadFile(std::string fname);
    void Clear();
    double GetRandomDegCM();
    void SetParticle(G4ThreeVector &vec, double &energy);

protected:
    void CalculateBeam();
    std::vector<double> deg_cm_;
    std::vector<double> deg_lab_;
    std::vector<double> en_p_;
    std::vector<double> cross_section_;
    double total_cs_;
    double v_cm_;
    double proton_mass_;
    double gamma_cm_;
    double beta_cm_;
    TH1D *fTH1D = nullptr;
    TGraph *fProtonEnergy = nullptr;
    TGraph *fLabTheta = nullptr;
    TRandom3 rand;
};
#endif
