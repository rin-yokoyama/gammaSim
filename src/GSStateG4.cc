#include "GSStateG4.hh"

GSStateG4::GSStateG4(const std::string &name) : sname(name)
{
    Clear();
}

GSStateG4::~GSStateG4(void)
{
}

void GSStateG4::Clear()
{
    E_gamma_.clear();
    P_branch_.clear();
    P_gamma_.clear();
}
void GSStateG4::SetBranch(const double &E, const double &Pb, const double &Pg, const std::string &sname)
{
    E_gamma_[sname] = E;
    P_branch_[sname] = Pb;
    P_gamma_[sname] = Pg;
}

const std::string GSStateG4::Decay(const double &rnd1, const double &rnd2, int &Ifg, double &Eg)
{
    Ifg = 0;
    Eg = 0;
    double p = 0;
    for (const auto &br : P_branch_)
    {
        const auto name = br.first;
        p += br.second;
        if (rnd1 < p)
        {
            // Decay to the state `name`
            if (rnd2 < P_gamma_[name])
            {
                // Decay with gamma-ray emission
                Ifg = 1;
                Eg = E_gamma_[name];
            }
            return name;
        }
    }
    // Decay to the ground state
    return "gnd";
}