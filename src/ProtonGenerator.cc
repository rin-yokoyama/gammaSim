#include "ProtonGenerator.hh"
#include <fstream>
#include <sstream>
#include <numeric>
#include <math.h>

ProtonGenerator::ProtonGenerator() : itr_(0)
{
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
    double deg_lab, en_p, phi;
    std::string line;
    Clear();
    std::getline(fin, line);
    std::cout << "reading file: " << fname << ", " << line << std::endl;
    while (std::getline(fin, line))
    {
        std::istringstream iss(line);
        iss >> deg_lab >> en_p >> phi;
        deg_lab_.emplace_back(deg_lab);
        en_p_.emplace_back(en_p);
        phi_.emplace_back(phi);
    }
    return 0;
}

void ProtonGenerator::Clear()
{
    deg_lab_.clear();
    en_p_.clear();
    phi_.clear();
    itr_ = 0;
}

void ProtonGenerator::SetParticle(G4ThreeVector &vec, double &energy)
{
    vec.setRThetaPhi(1, M_PI * deg_lab_[itr_] / 180., phi_[itr_]);
    energy = en_p_[itr_];
    ++itr_;
}