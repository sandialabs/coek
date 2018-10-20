#include "../model.h"

//
// An extension model that uses the CppAD library for autograd.
//

class CppAD_Model : public Model
{
public:

    CppAD_Model() : Model() {}

    void build();

    double compute_f(unsigned int i);

    void compute_df(double& f, std::vector<double>& df, unsigned int i);

    void compute_c(std::vector<double>& c);

    void compute_dc(std::vector<double>& dc, unsigned int i);

    void compute_adjoints(unsigned int i);

    void compute_Hv(std::vector<double>& v, std::vector<double>& Hv, unsigned int i);

    void print(std::ostream& ostr, int df);

};

