#include "ProtonGenerator.hh"
#include <fstream>
#include <sstream>
#include <numeric>
#include <math.h>
#include "ExpConstants.hh"

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
    double deg_lab, en_p, phi, x, y, z;
    std::string line;
    Clear();
    std::getline(fin, line);
    std::cout << "reading file: " << fname << ", " << line << std::endl;
    while (std::getline(fin, line))
    {
        std::istringstream iss(line);

        std::string deg_lab_str;
        std::getline(iss, deg_lab_str, ',');
        deg_lab = std::atof(deg_lab_str.c_str());

        std::string en_p_str;
        std::getline(iss, en_p_str, ',');
        en_p = std::atof(en_p_str.c_str());

        std::string phi_str;
        std::getline(iss, phi_str, ',');
        phi = std::atof(phi_str.c_str());

        std::string z_str;
        std::getline(iss, z_str, ',');
        z = std::atof(z_str.c_str());

        std::string x_str;
        std::getline(iss, x_str, ',');
        x = std::atof(x_str.c_str());

        std::string y_str;
        std::getline(iss, y_str, ',');
        y = std::atof(y_str.c_str());

        deg_lab_.emplace_back(deg_lab);
        en_p_.emplace_back(en_p);
        phi_.emplace_back(phi);
        targetX_.emplace_back(x);
        targetY_.emplace_back(y);
        targetZ_.emplace_back(z);
    }
    fin.close();
    return 0;
}

void ProtonGenerator::Clear()
{
    deg_lab_.clear();
    en_p_.clear();
    phi_.clear();
    targetX_.clear();
    targetY_.clear();
    targetZ_.clear();
    itr_ = 0;
}

void ProtonGenerator::SetParticle(G4ThreeVector &vec, double &energy, G4ThreeVector &position)
{
    if (itr_ >= en_p_.size())
        itr_ = 0;
    if (B1::kLimitTo2Pi)
    {
        if (phi_[itr_] > M_PI)
            phi_[itr_] = phi_[itr_] - M_PI;
    }
    vec.setRThetaPhi(1, M_PI * deg_lab_[itr_] / 180., phi_[itr_]);
    position.setX(targetX_[itr_] * mm);
    position.setY(targetY_[itr_] * mm);
    position.setZ(targetZ_[itr_] * mm);
    energy = en_p_[itr_];
    ++itr_;
}