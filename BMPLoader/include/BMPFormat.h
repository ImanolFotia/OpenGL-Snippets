#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>

#define Uncompressed 0
#define RLE8 1
#define RLE4 2
#define Bitfields 3

namespace BMP{
struct FileHeader
{
    unsigned short bfType;
    int bfSize;
    unsigned short bfReserved1;
    unsigned short bfReserved2;
    int bfOffBits;
};

struct BMPHeader
{
    int biSize;
    int biWidth;
    int biHeight;
    unsigned short biPlanes;
    unsigned short biBitCount;
    int biCompression;
    int biSizeImage;
    int biXPelsPerMeter;
    int biYPelsPerMeter;
    int biClrUsed;
    int biClrImportant;
};
}

