// Minecraft App.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "Block.h"

int wmain(int argc, wchar_t* argv[], wchar_t* envp[])
{
    
    //Bunch of returns if the input is wrong
    if (argc != 3 && argc != 5)
    {
        std::wcout << L"Invalid number of argument" << std::endl;
        return -1;
    }

    if (wcscmp(argv[1], L"-i") != 0)
    {
        std::wcout << L"use -i to specify the input file" << std::endl;
        return -1;
    }

    if (argc == 5)
    {
        if (wcscmp(argv[3], L"-o") != 0)
        {
            std::wcout << L"use -o to specify the output file" << std::endl;
            return -1;
        }
    }

    //Extract the given input and output file names
    std::wstring inputName, outputName;
    if (argc == 3)
    {
        outputName = L"scene1.obj";
    }
    else if (argc == 5)
    {
        outputName = argv[4];
    }

    inputName = argv[2];

    //Check if the formats are valid

    std::wstring suffix = L".json";
    if (inputName.length() <= suffix.length())
    {
        std::wcout << L"The input file name is not valid" << std::endl;
        return -1;
    }
    if (inputName.substr(inputName.length() - suffix.length()) != suffix)
    {
        std::wcout << L"The input file must be a .json" << std::endl;
        return -1;
    }

    suffix = L".obj";
    if (outputName.length() <= suffix.length())
    {
        std::wcout << L"The input file name is not valid" << std::endl;
        return -1;
    }
    if (outputName.substr(outputName.length() - suffix.length()) != suffix)
    {
        std::wcout << L"The output file must be a .obj" << std::endl;
        return -1;
    }

    BlockUtils::JsonToOBJ(inputName,outputName);

    return 0;

}

