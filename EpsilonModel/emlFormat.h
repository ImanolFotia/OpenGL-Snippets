#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>

// Format identifier must always be 0x53454D4C
#define EMLHEADER (('L'<<24)+('M'<<16)+('E'<<8)+'S')

const int maxLumps = 3;

enum eLumps
{
    lVertices = 0,
    lIndices,
    lTexCoords
};

struct t_eml_header
{
    int format;
    float version;
    int numLumps;
};

struct t_Lumps
{
    int offset;
};

struct t_Vertex
{
    float x;
    float y;
    float z;
};

struct t_Index
{
    unsigned int index;
};

struct t_TexCoord
{
    float s;
    float t;
};


