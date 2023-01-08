#include "celero/Celero.h"
#include "SPIRIT/Math.hpp"

CELERO_MAIN

////////////////////////////////////////////////////////////
// Benchmarks the overhead of converting Eigen expressions
// back to matrices after every basic operation.
// Benchmarks on matrix multiplication of 2 to 6 matrices.
//
// Assembly was verified, all benchmarks are vectorized code (and not optimized away)
// => No real slowdown on my machine. 
//  We keep the wrapper since it is simpler to work with
////////////////////////////////////////////////////////////


class Fixture : public celero::TestFixture
{
public:

    Fixture() {}

    virtual std::vector<celero::TestFixture::ExperimentValue>
    getExperimentValues() const override
    {
        std::vector<celero::TestFixture::ExperimentValue> problemSpace;

        // We will run some total number of sets of tests together.
        // Each one growing by a power of 2.
        const int totalNumberOfTests = 5;

        for (int i = 0; i < totalNumberOfTests; i++)
        {
            // ExperimentValues is part of the base class and allows us to specify
            // some values to control various test runs to end up building a nice graph.
            problemSpace.push_back({(i + 2), 500000});
        }

        return problemSpace;
    }

    /// Before each run, build a vector of random integers.
    virtual void
    setUp(const celero::TestFixture::ExperimentValue & experimentValue)
    {
        this->nMatrices = experimentValue.Value;
        this->eigMatrices.reserve(this->nMatrices);
        this->matrices.reserve(this->nMatrices);

        this->randomize();
    }

    virtual void
    tearDown() override
    {
    }

    /// Before each iteration. A common utility function to push back random ints to sort.
    void
    randomize()
    {
        this->clear();

        std::array<std::array<float, 4>, 4> dat;

        for (int i = 0; i < this->nMatrices; i++)
        {
            sp::RandList::random((float *)dat.begin(), (float *)dat.end());

            Eigen::Matrix4f eig;
            sp::Mat4 mat;

            for (int r = 0; r < 4; ++r)
            {
                for (int c = 0; c < 4; ++c)
                {
                    eig(r, c) = dat[r][c];
                    mat(r, c) = dat[r][c];
                }
            }

            this->eigMatrices.push_back(eig);
            this->matrices.push_back(mat);
        }
    }

    void
    clear()
    {
        this->eigMatrices.clear();
        this->matrices.clear();
    }

    std::vector<Eigen::Matrix4f> eigMatrices;
    std::vector<sp::Mat4> matrices;
    std::int64_t nMatrices;
};


BASELINE_F(MatrixMul, Eigen, Fixture, 100, 0)
{
    Eigen::Matrix4f res;

    // necessary to keep the expressions
    switch (nMatrices)
    {
        case 2: res = eigMatrices[0] * eigMatrices[1]; break;
        case 3: res = eigMatrices[0] * eigMatrices[1] * eigMatrices[2]; break;
        case 4:
            res = eigMatrices[0] * eigMatrices[1] * eigMatrices[2]
                  * eigMatrices[3];
            break;
        case 5:
            res = eigMatrices[0] * eigMatrices[1] * eigMatrices[2]
                  * eigMatrices[3] * eigMatrices[4];
            break;
        case 6:
            res = eigMatrices[0] * eigMatrices[1] * eigMatrices[2]
                  * eigMatrices[3] * eigMatrices[4] * eigMatrices[5];
    }

    celero::DoNotOptimizeAway(res);
}

BENCHMARK_F(MatrixMul, Spirit, Fixture, 100, 0)
{
    sp::Mat4 res = matrices[0];

    for(int i = 1; i< nMatrices; ++i){
        res *= matrices[i];
    }

    celero::DoNotOptimizeAway(res);
}

BENCHMARK_F(MatrixMul, SpiritSwitch, Fixture, 100, 0)
{
    sp::Mat4 res;

    // must use equivalent form.
    // even if we could just loop
    switch (nMatrices)
    {
        case 2: res = matrices[0] * matrices[1]; break;
        case 3: res = matrices[0] * matrices[1] * matrices[2]; break;
        case 4:
            res = matrices[0] * matrices[1] * matrices[2]
                  * matrices[3];
            break;
        case 5:
            res = matrices[0] * matrices[1] * matrices[2]
                  * matrices[3] * matrices[4];
            break;
        case 6:
            res = matrices[0] * matrices[1] * matrices[2]
                  * matrices[3] * matrices[4] * matrices[5];
    }

    celero::DoNotOptimizeAway(res);
}
