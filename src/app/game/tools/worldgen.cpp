#include "worldgen.hpp"

std::vector<Block*>* WorldGen::generate()
{
    std::vector<Block*>* aLevel = new std::vector<Block*>(aLevelWidth * aLevelHeight);
    int a;
    int comb = 0;
    for (int i = 0; i < aLevel->size(); i++) {
        //wypełij powietrzem
        if (i < 126 * aLevelWidth) {
            (*aLevel)[i] = nullptr;

        } //poniżej wypełnij ziemią/trawą
        else if (i < 127 * aLevelWidth) {
            if (comb <= 0) {
                int test = pge::random::get(1, 100);
                if (test > 90)
                    comb = pge::random::get(2, 8);
                (*aLevel)[i] = nullptr;
            } else {
                comb--;
                (*aLevel)[i] = &aBlocks[b_hardrockgrass];
            }
        } else if (i < 128 * aLevelWidth) {
            if (comb <= 0) {
                int test = pge::random::get(1, 100);
                if (test > 80)
                    comb = pge::random::get(4, 16);
                if ((*aLevel)[i - aLevelWidth] == nullptr)
                    (*aLevel)[i] = nullptr;
                else
                    (*aLevel)[i] = &aBlocks[b_hardrockgrass];
            } else {
                comb--;
                if ((*aLevel)[i - aLevelWidth] == nullptr)
                    (*aLevel)[i] = &aBlocks[b_hardrockgrass];
                else
                    (*aLevel)[i] = &aBlocks[b_hardrock];
            }
        } else if (i < 198 * aLevelWidth) {
            if ((*aLevel)[i - aLevelWidth] == nullptr)
                (*aLevel)[i] = &aBlocks[b_hardrockgrass];
            else
                (*aLevel)[i] = &aBlocks[b_hardrock];
        } else if (i < 150 * aLevelWidth) {
            (*aLevel)[i] = &aBlocks[b_hardrock];
        } else if (i < 154 * aLevelWidth) {
            a = pge::random::get(0, 100) + (i / aLevelWidth - 154) * 10;
            if (a > 60)
                (*aLevel)[i] = &aBlocks[b_hardrock];
            else
                (*aLevel)[i] = &aBlocks[b_hardrock];
        } else {
            (*aLevel)[i] = &aBlocks[b_hardrock];
        }
    }

    return aLevel;
}
