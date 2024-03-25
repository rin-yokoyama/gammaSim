#include "ProtonGenerator.hh"
#include <fstream>
#include <sstream>
#include <numeric>
#include <math.h>

ProtonGenerator::ProtonGenerator() : total_cs_(0), v_cm_(0)
{
    CalculateBeam();
}

ProtonGenerator::~ProtonGenerator()
{
    Clear();
}

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
    fTH1D = new TH1D("CrossSection", "Cross Section", 179, 0.5, 179.5);
    fProtonEnergy = new TGraph(179);
    fLabTheta = new TGraph(179);
    Int_t ibin = 0;
    while (std::getline(fin, line))
    {
        std::istringstream iss(line);
        iss >> deg_cm >> deg_lab >> en_p >> deg_rec >> en_rec >> cs;
        fTH1D->Fill(deg_cm, cs);
        fProtonEnergy->SetPoint(ibin, deg_cm, en_p);
        fLabTheta->SetPoint(ibin++, deg_cm, deg_lab);
        deg_cm_.emplace_back(deg_cm);
        deg_lab_.emplace_back(deg_lab);
        en_p_.emplace_back(en_p);
        cross_section_.emplace_back(cs);
    }
    total_cs_ = std::reduce(cross_section_.begin(), cross_section_.end());
    return 0;
}

double ProtonGenerator::GetRandomDegCM()
{
    return fTH1D->GetRandom();
}

void ProtonGenerator::CalculateBeam()
{
    const int numberOfNucleon = 148;
    const double beamEnergy = 40.0 * numberOfNucleon;                                  // MeV
    const double protonMass = 938.27;                                                  // MeV/c^2
    const double neutronMass = 939.57;                                                 // MeV/c^2
    const double beamMassExcess = 70.398;                                              // MeV/c^2
    const double beamStaticMass = protonMass * 58 + neutronMass * 90 - beamMassExcess; // MeV/c^2
    const double targetStaticMass = protonMass;                                        // MeV/c^2
    const double beamGamma = beamEnergy / beamStaticMass + 1;
    const double beamBeta = sqrt(1. - 1. / (beamGamma * beamGamma));
    gamma_cm_ = beamGamma;
    beta_cm_ = beamBeta;
    proton_mass_ = protonMass;
}

void ProtonGenerator::Clear()
{
    deg_lab_.clear();
    en_p_.clear();
    cross_section_.clear();
    if (fTH1D)
        delete fTH1D;
    fTH1D = nullptr;
    if (fProtonEnergy)
        delete fProtonEnergy;
    fProtonEnergy = nullptr;
    if (fLabTheta)
        delete fLabTheta;
    fLabTheta = nullptr;
}

void ProtonGenerator::SetParticle(G4ThreeVector &vec, double &energy)
{
    const double deg_cm = GetRandomDegCM();
    const double en_cm = fProtonEnergy->Eval(deg_cm);
    const double gamma_p_cm = en_cm / proton_mass_ + 1;
    const double beta_p_cm = sqrt(1. - 1. / (gamma_p_cm * gamma_p_cm));
    const double beta_p_cm_x = -beta_p_cm * cos(deg_cm * M_PIf64 / 180.);
    const double beta_p_cm_y = beta_p_cm * sin(deg_cm * M_PIf64 / 180.);
    const double beta_p_x = (beta_p_cm_x - beta_cm_) / (1. - beta_cm_ * beta_p_cm_x);
    const double beta_p_y = beta_p_cm_y / (gamma_cm_ * (1. - beta_cm_ * beta_p_cm_x));
    const double beta_p = sqrt(beta_p_x * beta_p_x + beta_p_y * beta_p_y);
    const double gamma_p = 1. / sqrt(1. - beta_p * beta_p);
    // vec.setRThetaPhi(beta_p, acos(beta_p_x / beta_p), rand.Uniform(0, 2 * M_PIf64));
    //  energy = gamma_p * proton_mass_;
    vec.setRThetaPhi(beta_p, fLabTheta->Eval(deg_cm), rand.Uniform(0, 2 * M_PIf64));
    energy = fProtonEnergy->Eval(deg_cm);
    // std::cout << "beta_cm: " << beta_cm_ << ",\tbeta_p_cm_x: " << beta_p_cm_x;
    // std::cout << "beta_p_cm_y: " << beta_p_cm_y << ",\tbeta_p_x: " << beta_p_x;
    // std::cout << ",\tbeta_p_y: " << beta_p_y << ",\tbeta_p: " << beta_p;
    //  std::cout << std::endl;
}