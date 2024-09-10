#include "GSource4G4.hh"

GSource4G4::GSource4G4(const std::string &prm_file)
{
    ReadPrm(prm_file);
    Clear();
}

GSource4G4::~GSource4G4(void)
{
}

void GSource4G4::Clear()
{
    E_gamma_.clear();
}

void GSource4G4::ReadPrm(const std::string &fname)
{
    std::ifstream fin(fname.c_str(), std::ios::in);
    if (!fin)
    {
        std::cout << "[GSource4G4-E]: cannot open file: " << fname << std::endl;
    }
    fin >> n_state;
    State_.emplace("parent", std::make_shared<GSStateG4>("parent"));
    double brsum = 0;
    for (int i = 0; i < n_state; i++)
    {
        std::string sname, buf;
        double bratio;
        int brnum;
        double branchsum = 0;
        fin >> sname;
        fin >> bratio;
        fin >> brnum;
        brsum += bratio;
        State_["parent"]->SetBranch(0, bratio, 0, sname);
        getline(fin, buf);
        getline(fin, buf);
        State_.emplace(sname, std::make_shared<GSStateG4>(sname));
        for (int j = 0; j < brnum; j++)
        {
            double Energy, Pbr, Pg;
            std::string dsname;
            fin >> Energy;
            fin >> dsname;
            fin >> Pbr;
            fin >> Pg;
            branchsum += Pbr;
            State_[sname]->SetBranch(Energy, Pbr, Pg, dsname);
        }
    }
    fin.close();
    return;
}

int GSource4G4::EmitGamma()
{
    int Ifg;
    double Eg;
    n_gamma = 0;
    E_gamma_.clear();

    // beta decay from parent
    auto sname = State_["parent"]->Decay(G4UniformRand(), G4UniformRand(), Ifg, Eg);

    // gamma decays
    while (sname != "gnd")
    {
        const auto st = State_[sname]->Decay(G4UniformRand(), G4UniformRand(), Ifg, Eg);
        if (Ifg)
        {
            E_gamma_.emplace_back(Eg);
        }
        sname = st;
    }
    return E_gamma_.size();
}
