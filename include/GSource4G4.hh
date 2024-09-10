#ifndef GSOURCE4G4_INCLUDE
#define GSOURCE4G4_INCLUDE

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <memory>
#include <time.h>
#include "Randomize.hh"
#include "GSStateG4.hh"

class GSource4G4
{
public:
    GSource4G4(const std::string &prm_file);
    virtual ~GSource4G4();
    void Clear();
    void ReadPrm(const std::string &fname);
    int EmitGamma();
    inline std::vector<double> GetEGamma() { return E_gamma_; }

protected:
    int n_state;
    std::map<std::string, std::shared_ptr<GSStateG4>> State_;
    int n_gamma;
    std::vector<double> E_gamma_;
};

#endif