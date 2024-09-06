#ifndef __PROTON_GENERATOR_HH__
#define __PROTON_GENERATOR_HH__
#include <iostream>
#include <vector>
#include <string>
#include <G4ThreeVector.hh>
class ProtonGenerator
{
public:
    ProtonGenerator();
    virtual ~ProtonGenerator();
    int ReadFile(std::string fname);
    void Clear();
    void SetParticle(G4ThreeVector &vec, double &energy, G4ThreeVector &position);

protected:
    u_int64_t itr_;
    std::vector<double> deg_lab_;
    std::vector<double> en_p_;
    std::vector<double> phi_;
    std::vector<double> targetX_;
    std::vector<double> targetY_;
    std::vector<double> targetZ_;
};
#endif
