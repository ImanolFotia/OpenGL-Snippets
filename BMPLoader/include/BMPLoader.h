#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>

class BMPImage
{
public:
    BMPImage(std::string);
    ~BMPImage() {}

public:
    int m_Width;
    int m_height;
    std::vector<unsigned char> pixels;

};

