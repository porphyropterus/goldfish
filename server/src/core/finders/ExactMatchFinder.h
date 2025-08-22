#include <string>

#include "AbstractFinder.h"

#include "types.h"

template <typename TInput, typename TOutput>
class ExactMatchFinder : public AbstractFinder<TInput, TOutput>
{
public:
    ExactMatchFinder(u32 numHashes, const std::string &filePath)
        : AbstractFinder(numHashes, filePath) {}

    std::vector<TOutput> getResults(const std::vector<u32> &seeds) override
    {
        std::vector<TOutput> results;

        for (u32 seed : seeds)
        {
            TOutput output = generatePotentialOutputFromSeed(seed);
            if (doesPotentialOutputMatchInput(output, input))
            {
                results.push_back(output);
            }
        }

        return results;
    }

    virtual bool doesPotentialOutputMatchInput(const TOutput &output, const TInput &input) = 0;
};