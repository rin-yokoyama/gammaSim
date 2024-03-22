#ifndef __PROTON_GENERATOR_HH__
#define __PROTON_GENERATOR_HH__
#include <vector>
#include <string>

class ProtonGenerator
{
public:
    ProtonGenerator();
    virtual ~ProtonGenerator();
    int ReadFile(std::string fname);
    void Clear();

protected:
    std::vector<double> deg_lab_;
    std::vector<double> en_p_;
    std::vector<double> cross_section_;
    double total_cs_;
    double v_cm_;
};
#endif
