#ifndef GSSTATEG4_INCLUDE
#define GSSTATEG4_INCLUDE

#include <iostream>
#include <map>
#include <string>

class GSStateG4
{
public:
    GSStateG4(const std::string &name);
    virtual ~GSStateG4();
    void SetBranch(const double &E, const double &Pb, const double &Pg, const std::string &sname);
    const std::string Decay(const double &rnd1, const double &rnd2, int &Ifg, double &Eg);
    inline std::string GetName() const { return sname; }
    void Clear();

protected:
    const std::string sname;
    std::map<std::string, double> E_gamma_;
    std::map<std::string, double> P_branch_;
    std::map<std::string, double> P_gamma_;
};

#endif