// #define _GLIBCXX_USE_CXX11_ABI 0
#include <cmath>
#include <cstdlib>
#include <vector>

#include "gurobi_c++.h"
using namespace std;

void fac(GRBModel& model, size_t F)
{
    const int G = F;
    // cout << "G: " << G << ", F: " << F << endl;

    // Create variables
    GRBVar d = model.addVar(0, GRB_INFINITY, 1.0, GRB_CONTINUOUS);

    vector<vector<GRBVar>> y;
    for (int f = 0; f < F; f++) {
        y.push_back(vector<GRBVar>());
        for (int k = 0; k < 2; k++) {
            y[f].push_back(model.addVar(0, 1, 0, GRB_CONTINUOUS));
        }
    }

    vector<vector<vector<GRBVar>>> z;
    for (int i = 0; i <= G; i++) {
        z.push_back(vector<vector<GRBVar>>());
        for (int j = 0; j <= G; j++) {
            z[i].push_back(vector<GRBVar>());
            for (int f = 0; f < F; f++) {
                z[i][j].push_back(model.addVar(0, 1, 0, GRB_BINARY));
            }
        }
    }

    vector<vector<vector<GRBVar>>> s;
    for (int i = 0; i <= G; i++) {
        s.push_back(vector<vector<GRBVar>>());
        for (int j = 0; j <= G; j++) {
            s[i].push_back(vector<GRBVar>());
            for (int f = 0; f < F; f++) {
                s[i][j].push_back(model.addVar(0, GRB_INFINITY, 0, GRB_CONTINUOUS));
            }
        }
    }

    vector<vector<vector<vector<GRBVar>>>> r;
    for (int i = 0; i <= G; i++) {
        r.push_back(vector<vector<vector<GRBVar>>>());
        for (int j = 0; j <= G; j++) {
            r[i].push_back(vector<vector<GRBVar>>());
            for (int f = 0; f < F; f++) {
                r[i][j].push_back(vector<GRBVar>());
                for (int k = 0; k < 2; k++) {
                    r[i][j][f].push_back(
                        model.addVar(-GRB_INFINITY, GRB_INFINITY, 0, GRB_CONTINUOUS));
                }
            }
        }
    }

    model.update();

    GRBLinExpr obj;
    obj += d;
    model.setObjective(obj);

    // Add constraints

    // Each customer is assigned to a facility
    for (int i = 0; i <= G; i++) {
        for (int j = 0; j <= G; j++) {
            GRBLinExpr lhs;
            for (int f = 0; f < F; f++)
                lhs += z[i][j][f];
            model.addConstr(lhs, GRB_EQUAL, 1);
        }
    }

    const double M = 2 * sqrt(2.0);
    for (int i = 0; i <= G; i++) {
        for (int j = 0; j <= G; j++) {
            for (int f = 0; f < F; f++) {
                model.addConstr(s[i][j][f], GRB_EQUAL, d + M * (1 - z[i][j][f]));
                model.addConstr(r[i][j][f][0], GRB_EQUAL, (1.0 * i) / G - y[f][0]);
                model.addConstr(r[i][j][f][1], GRB_EQUAL, (1.0 * j) / G - y[f][1]);
                model.addQConstr(r[i][j][f][0] * r[i][j][f][0] + r[i][j][f][1] * r[i][j][f][1],
                                 GRB_LESS_EQUAL, s[i][j][f] * s[i][j][f]);
            }
        }
    }
}
