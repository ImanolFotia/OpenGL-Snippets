#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>

class TGAImage
{
public:
    TGAImage(std::string);
    ~TGAImage() {}

public:
    int m_Width;
    int m_height;
    std::vector<unsigned char> pixels;

};
