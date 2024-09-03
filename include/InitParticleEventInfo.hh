#include "G4VUserEventInformation.hh"

class InitParticleEventInfo : public G4VUserEventInformation
{
public:
    InitParticleEventInfo(double e, double t, double p) : eProton_(e), thetaLab_(t), phiLab_(p) {};
    ~InitParticleEventInfo() {};

    void SetData(double energy, double theta, double phi)
    {
        eProton_ = energy;
        thetaLab_ = theta;
        phiLab_ = phi;
    }
    double GetProtonEnergy() const { return eProton_; }
    double GetThetaLab() const { return thetaLab_; }
    double GetPhiLab() const { return phiLab_; }

    void Print() const override {};

private:
    double eProton_;
    double thetaLab_;
    double phiLab_;
};