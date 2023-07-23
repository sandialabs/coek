#include <algorithm>
#include <random>
#include <limits>
#include <thread>
#include "coek/coek.hpp"

coek::Model srosenbr_vector();

// void _eval_model(size_t thread_id, size_t niters, coek::NLPModel& f_min) {}

void eval_model(size_t thread_id, size_t niters, coek::NLPModel& nlp, double& f_min)
{
    std::mt19937 gen(123456789 + thread_id);
    std::uniform_real_distribution<> urand(-1.0, 1.0);
    std::vector<double> x(nlp.num_variables());

    double f;
    std::vector<double> df(nlp.num_variables());

    f_min = std::numeric_limits<double>::max();
    for (auto i : coek::range(niters)) {
        for (double& val : x) val = urand(gen);

        nlp.set_variable_view(x);
        nlp.compute_df(f, df);
        f_min = std::min(f, f_min);
    }
}

//
// Randomly search for low values of srosenbr across nthreads
//
double test_srosenbr_vector_threadeval(const std::string& asl_type, size_t nthreads, size_t niters,
                                       double timelimit)
{
    std::vector<double> f_min(nthreads);

    //
    // Allocate models before creating threads
    //
    std::vector<coek::NLPModel> models;
    for (auto i : coek::range(nthreads)) {
        coek::Model tmp = srosenbr_vector();
        models.emplace_back(coek::NLPModel(tmp, asl_type));
    }

    //
    // Create threads and start them
    //
    std::vector<std::thread> threads;
    for (size_t i : coek::range(nthreads)) {
        threads.emplace_back(eval_model, i, niters, std::ref(models[i]), std::ref(f_min[i]));
    }
    //
    // Join the threads
    //
    for (auto& t : threads) t.join();

    return *min_element(f_min.begin(), f_min.end());
}
