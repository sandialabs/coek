#include "model.hpp"

//
// An extension model that uses the ASL library for autograd.
//

class ASL_Model : public Model
{
public:

    ASL_Model(ExpressionContext* context) : Model(context) {}

    void build();

    void compute_adjoints(unsigned int i);

    void print(std::ostream& ostr, int df);

protected:

    double _compute_f(unsigned int i);

    void _compute_df(double& f, std::vector<double>& df, unsigned int i);

    void _compute_c(std::vector<double>& c);

    void _compute_dc(std::vector<double>& dc, unsigned int i);

    void _compute_Hv(std::vector<double>& v, std::vector<double>& Hv, unsigned int i);

};

