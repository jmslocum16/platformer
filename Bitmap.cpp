//
// bitmap.cpp
//
// handle MS bitmap I/O. For portability, we don't use the data structure defined in Windows.h
//

#include "Bitmap.h"
 
#include <cstdio>
#include <cstring>
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

// As seen on stack overflow: http://stackoverflow.com/questions/20595340/loading-a-tga-bmp-file-in-c-opengl
unsigned char * readBMP(const char* FilePath, int &width, int &height)
{
    short BitsPerPixel = 0;
    std::vector<unsigned char> Pixels;

    std::fstream hFile(FilePath, std::ios::in | std::ios::binary);
    if (!hFile.is_open()) throw std::invalid_argument("Error: File Not Found.");

    hFile.seekg(0, std::ios::end);
    int Length = hFile.tellg();
    hFile.seekg(0, std::ios::beg);
    std::vector<std::uint8_t> FileInfo(Length);
    hFile.read(reinterpret_cast<char*>(FileInfo.data()), 54);

    if(FileInfo[0] != 'B' && FileInfo[1] != 'M')
    {
        hFile.close();
        throw std::invalid_argument("Error: Invalid File Format. Bitmap Required.");
    }

    if (FileInfo[28] != 24 && FileInfo[28] != 32)
    {
        hFile.close();
        throw std::invalid_argument("Error: Invalid File Format. 24 or 32 bit Image Required.");
    }

    BitsPerPixel = FileInfo[28];
    width = FileInfo[18] + (FileInfo[19] << 8);
    height = FileInfo[22] + (FileInfo[23] << 8);
    std::uint32_t PixelsOffset = FileInfo[10] + (FileInfo[11] << 8);
    std::uint32_t size = ((width * BitsPerPixel + 31) / 32) * 4 * height;
    Pixels.resize(size);

    hFile.seekg (PixelsOffset, std::ios::beg);
    hFile.read(reinterpret_cast<char*>(Pixels.data()), size);
    hFile.close();
    unsigned char * dat = new unsigned char[Pixels.size()];
    copy(Pixels.begin(), Pixels.end(), dat);
    return dat;
}
