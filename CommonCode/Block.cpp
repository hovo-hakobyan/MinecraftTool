#include "Block.h"

#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
#include "rapidjson/stream.h"
#include "rapidjson/istreamwrapper.h"

#include <iostream>
#include <fstream>
#include <direct.h>
#include <Windows.h>
#include <locale>


bool Block::IsFaceHidden(const float3& blockPos, const float3& posToCompare)
{
    return blockPos == posToCompare;
}


void Block::JsonToOBJ(const std::wstring& inputName, const std::wstring& outputName)
{
    using namespace rapidjson;
     if (std::ifstream is{ inputName.c_str() })
    {
        //Json library can work with std::Ifstrean if they are wrapped in the IStreamWrapper object
        IStreamWrapper isw{ is };

        Document jsonDoc;
        jsonDoc.ParseStream(isw);

        std::vector<BlockType> blockTypes{};
        for (Value::ConstValueIterator itr = jsonDoc.Begin(); itr != jsonDoc.End(); ++itr)
        {
            const Value& item = *itr;
            const Value& layer = item["layer"];
            const Value& opaque = item["opaque"];

            const Value& positions = item["positions"];
            BlockType currentTile{};
            currentTile.bOpaque = opaque.GetBool();
            currentTile.layer = layer.GetString();

            for (const auto& pos : positions.GetArray())
            {
                float3 currentPos{ pos[0].GetFloat() + 0.5f,pos[2].GetFloat() + 0.5f,pos[1].GetFloat() + 0.5f };
                currentTile.Blocks.push_back(currentPos);
            }

            blockTypes.push_back(currentTile);
        }

        //data is read and inside tiles vector

        //Optimizing
        for (auto& blockType : blockTypes)
        {
            for (auto& block : blockType.Blocks)
            {
                //Is up face visible?
                float3 posToCheck = block.center;
                posToCheck.y += 1.0f;

                bool isFaceHidden = std::any_of(blockType.Blocks.begin(), blockType.Blocks.end(), [&](const BlockInfo& blockInfo)
                    {
                        return IsFaceHidden(blockInfo.center, posToCheck);
                    });

                if (isFaceHidden)
                {
                    block.facesVisibility[FACE_UP] = false;
                }

                ////Is down face visible?
                posToCheck.y -= 2.0f;
                isFaceHidden = std::any_of(blockType.Blocks.begin(), blockType.Blocks.end(), [&](const BlockInfo& blockInfo)
                    {
                        return IsFaceHidden(blockInfo.center, posToCheck);
                    });

                if (isFaceHidden)
                {
                    block.facesVisibility[FACE_DOWN] = false;
                }

                ////Is left face visible?
                posToCheck = block.center;
                posToCheck.x += 1.0f;
                isFaceHidden = std::any_of(blockType.Blocks.begin(), blockType.Blocks.end(), [&](const BlockInfo& blockInfo)
                    {
                        return IsFaceHidden(blockInfo.center, posToCheck);
                    });

                if (isFaceHidden)
                {
                    block.facesVisibility[FACE_LEFT] = false;
                }

                ////Is right face visible?
                posToCheck.x -= 2.0f;
                isFaceHidden = std::any_of(blockType.Blocks.begin(), blockType.Blocks.end(), [&](const BlockInfo& blockInfo)
                    {
                        return IsFaceHidden(blockInfo.center, posToCheck);
                    });

                if (isFaceHidden)
                {
                    block.facesVisibility[FACE_RIGHT] = false;
                }

                ////Is front face visible?
                posToCheck = block.center;
                posToCheck.z += 1.0f;
                isFaceHidden = std::any_of(blockType.Blocks.begin(), blockType.Blocks.end(), [&](const BlockInfo& blockInfo)
                    {
                        return IsFaceHidden(blockInfo.center, posToCheck);
                    });

                if (isFaceHidden)
                {
                    block.facesVisibility[FACE_FRONT] = false;
                }

                ////Is back face visible?
                posToCheck.z -= 2.0f;
                isFaceHidden = std::any_of(blockType.Blocks.begin(), blockType.Blocks.end(), [&](const BlockInfo& blockInfo)
                    {
                        return IsFaceHidden(blockInfo.center, posToCheck);
                    });

                if (isFaceHidden)
                {
                    block.facesVisibility[FACE_BACK] = false;
                }
            }
        }


        //writing to obj

        FILE* pFile = nullptr;
        _wfopen_s(&pFile, outputName.c_str(), L"w+,ccs=UTF-8");
        if (pFile)
        {
            const wchar_t* text = L"# is the symbol for partial derivative.\n";
            fwrite(text, wcslen(text) * sizeof(wchar_t), 1, pFile);

            fwprintf_s(pFile, L"vn %.4f %.4f %.4f\n", 0.f, 0.f, 1.f);
            fwprintf_s(pFile, L"vn %.4f %.4f %.4f\n", 0.f, 0.f, -1.f);
            fwprintf_s(pFile, L"vn %.4f %.4f %.4f\n", 0.f, 1.f, 0.f);
            fwprintf_s(pFile, L"vn %.4f %.4f %.4f\n", 0.f, -1.f, 0.f);
            fwprintf_s(pFile, L"vn %.4f %.4f %.4f\n", 1.f, 0.f, 0.f);
            fwprintf_s(pFile, L"vn %.4f %.4f %.4f\n", -1.f, 0.f, 0.f);

            int startVertIdx = 1;
            for (const auto& currentBlockType : blockTypes)
            {
                for (const auto& currentBlock : currentBlockType.Blocks)
                {
                    fwprintf_s(pFile, L"v %.4f %.4f %.4f\n", currentBlock.center.x - 0.5f, currentBlock.center.y - 0.5f, currentBlock.center.z - 0.5f);
                    fwprintf_s(pFile, L"v %.4f %.4f %.4f\n", currentBlock.center.x - 0.5f, currentBlock.center.y - 0.5f, currentBlock.center.z + 0.5f);
                    fwprintf_s(pFile, L"v %.4f %.4f %.4f\n", currentBlock.center.x - 0.5f, currentBlock.center.y + 0.5f, currentBlock.center.z - 0.5f);
                    fwprintf_s(pFile, L"v %.4f %.4f %.4f\n", currentBlock.center.x - 0.5f, currentBlock.center.y + 0.5f, currentBlock.center.z + 0.5f);
                    fwprintf_s(pFile, L"v %.4f %.4f %.4f\n", currentBlock.center.x + 0.5f, currentBlock.center.y - 0.5f, currentBlock.center.z - 0.5f);
                    fwprintf_s(pFile, L"v %.4f %.4f %.4f\n", currentBlock.center.x + 0.5f, currentBlock.center.y - 0.5f, currentBlock.center.z + 0.5f);
                    fwprintf_s(pFile, L"v %.4f %.4f %.4f\n", currentBlock.center.x + 0.5f, currentBlock.center.y + 0.5f, currentBlock.center.z - 0.5f);
                    fwprintf_s(pFile, L"v %.4f %.4f %.4f\n", currentBlock.center.x + 0.5f, currentBlock.center.y + 0.5f, currentBlock.center.z + 0.5f);

                    if (currentBlock.facesVisibility[FACE_BACK])
                    {
                        //Back faces
                        fwprintf_s(pFile, L"f %d//%d %d//%d %d//%d\n", startVertIdx, 2, startVertIdx + 6, 2, startVertIdx + 4, 2);
                        fwprintf_s(pFile, L"f %d//%d %d//%d %d//%d\n", startVertIdx, 2, startVertIdx + 2, 2, startVertIdx + 6, 2);
                    }
                    if (currentBlock.facesVisibility[FACE_FRONT])
                    {
                        //Front faces
                        fwprintf_s(pFile, L"f %d//%d %d//%d %d//%d\n", startVertIdx + 1, 1, startVertIdx + 5, 1, startVertIdx + 7, 1);
                        fwprintf_s(pFile, L"f %d//%d %d//%d %d//%d\n", startVertIdx + 1, 1, startVertIdx + 7, 1, startVertIdx + 3, 1);
                    }
                    if (currentBlock.facesVisibility[FACE_RIGHT])
                    {
                        //Right faces
                        fwprintf_s(pFile, L"f %d//%d %d//%d %d//%d\n", startVertIdx, 6, startVertIdx + 3, 6, startVertIdx + 2, 6);
                        fwprintf_s(pFile, L"f %d//%d %d//%d %d//%d\n", startVertIdx, 6, startVertIdx + 1, 6, startVertIdx + 3, 6);
                    }
                    if (currentBlock.facesVisibility[FACE_LEFT])
                    {
                        //Left faces
                        fwprintf_s(pFile, L"f %d//%d %d//%d %d//%d\n", startVertIdx + 4, 5, startVertIdx + 6, 5, startVertIdx + 7, 5);
                        fwprintf_s(pFile, L"f %d//%d %d//%d %d//%d\n", startVertIdx + 4, 5, startVertIdx + 7, 5, startVertIdx + 5, 5);
                    }
                    if (currentBlock.facesVisibility[FACE_UP])
                    {
                        //Up faces
                        fwprintf_s(pFile, L"f %d//%d %d//%d %d//%d\n", startVertIdx + 2, 3, startVertIdx + 7, 3, startVertIdx + 6, 3);
                        fwprintf_s(pFile, L"f %d//%d %d//%d %d//%d\n", startVertIdx + 2, 3, startVertIdx + 3, 3, startVertIdx + 7, 3);
                    }
                    if (currentBlock.facesVisibility[FACE_DOWN])
                    {
                        //Down faces
                        fwprintf_s(pFile, L"f %d//%d %d//%d %d//%d\n", startVertIdx, 4, startVertIdx + 4, 4, startVertIdx + 5, 4);
                        fwprintf_s(pFile, L"f %d//%d %d//%d %d//%d\n", startVertIdx, 4, startVertIdx + 5, 4, startVertIdx + 1, 4);
                    }

                    startVertIdx += 8;
                }
            }

            fclose(pFile);

        }
        else
        {
            std::wcout << L"Something went wrong, couldn't make output file" << std::endl;
            return;
        }

    }
    else
    {
        std::wcout << L"Input file not found" << std::endl;
        return;
    }


    std::wcout << L"Finished successfully" << std::endl;
}
