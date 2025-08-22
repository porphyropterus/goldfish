#include <string>

#include "AbstractFinder.h"

#include "types.h"

template <typename TInput, typename TOutput>
struct ScoredOutput
{
    double score;
    TOutput output;
};

template <typename TInput, typename TOutput>
class TopNFinder : public AbstractFinder<TInput, ScoredOutput<TOutput>>
{
public:
    TopNFinder(u32 numHashes, const std::string &filePath, const u32 n)
        : AbstractFinder(numHashes, filePath), n(n) {}

    std::vector<ScoredOutput<TInput, TOutput>> getResults(const std::vector<u32> &seeds, const TInput &input)
    {
        std::vector<ScoredOutput<TInput, TOutput>> scored;

        for (u32 seed : seeds)
        {
            TOutput output = this->generatePotentialOutputFromSeed(seed);
            double score = scoreOutput(output, input);
            scored.push_back({score, output});
        }

        if (scored.size() > n)
        {
            std::partial_sort(scored.begin(), scored.begin() + n, scored.end(),
                              [](const auto &a, const auto &b)
                              { return a.score < b.score; });
            scored.resize(n);
        }

        return scored;
    }

    virtual double
    scoreOutput(const TOutput &output, const TInput &input) = 0;

private:
    u32 n;
};