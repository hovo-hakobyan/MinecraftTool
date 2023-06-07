#pragma once
#include "float3.h"
#include "HelperEnums.h"
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

bool IsFaceHidden(const float3& blockPos, const float3& posToCompare);

namespace BlockUtils
{
    void JsonToOBJ(const std::wstring& inputPath, const std::wstring& outputPath);
}


