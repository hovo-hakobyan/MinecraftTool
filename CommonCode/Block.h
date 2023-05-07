#pragma once
#include "float3.h"
#include <algorithm>
#include <vector>
#include <string>

struct BlockInfo
{
    BlockInfo(const float3& _pos) :
        center{ _pos } 
    {
        std::fill_n(facesVisibility, 5, true);
    }

    float3 center;
    bool facesVisibility[5];

};

struct BlockType
{
    std::string layer;
    bool bOpaque;
    std::vector<BlockInfo> Blocks;
};