#include "BMPLoader.h"
#include "BMPFormat.h"
#include <fstream>
#include <cstring>
BMPImage::BMPImage(std::string filePath)
{
    std::ifstream FILE(filePath.c_str(), std::ios::binary);

    if(!FILE.is_open())
    {
        std::cout << "Fail to open BMP file" << std::endl;
        return;
    }

    unsigned char *buff[2] = {nullptr, nullptr};
    buff[0] = new unsigned char[sizeof(char) * 14];
    buff[1] = new unsigned char[sizeof(char) * 40];


    BMP::FileHeader* header = new BMP::FileHeader[sizeof(char) * 14];
    BMP::BMPHeader* bmp_header = new BMP::BMPHeader[sizeof(char) * 40];

    FILE.read((char*)&header[0], sizeof(char) * 14);
    FILE.read((char*)&bmp_header[0], sizeof(char) * 40);

    if(header->bfType != 0x4D42)
    return;

    if(bmp_header->biBitCount != 8 && bmp_header->biBitCount != 24)
        return;

    if(bmp_header->biSizeImage == 0)
        bmp_header->biSizeImage = bmp_header->biWidth * bmp_header->biHeight * 3;

    this->m_Width = bmp_header->biWidth;
    this->m_height = bmp_header->biHeight;


    pixels.reserve(bmp_header->biSizeImage);

    FILE.seekg(header->bfOffBits);
    FILE.read((char*)&pixels[0], bmp_header->biSizeImage);

    unsigned char b = 0;
    unsigned char g = 0;
    unsigned char r = 0;

    unsigned char tmpRGB = 0; // Swap buffer
    if(bmp_header->biBitCount == 24)
    {

        for (unsigned long i = 0; i < bmp_header->biSizeImage; i += 3)
        {
            tmpRGB        = pixels[i];
            pixels[i]     = pixels[i + 2];
            pixels[i + 2] = tmpRGB;
        }
    }
    else if(bmp_header->biBitCount == 8)
    {
        unsigned char* tmpPixels;
        tmpPixels = new unsigned char[bmp_header->biSizeImage];
        std::memcpy(tmpPixels, &pixels[0], sizeof(unsigned char) * bmp_header->biSizeImage);
        pixels.clear();
        long counter = 0;
        for (unsigned long i = 0; i < bmp_header->biSizeImage*3; i+=3)
        {
            pixels.push_back(tmpPixels[counter]);
            pixels.push_back(tmpPixels[counter]);
            pixels.push_back(tmpPixels[counter]);

            counter++;

        }

        delete[] tmpPixels;
    }

}
