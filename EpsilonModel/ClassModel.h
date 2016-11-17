#ifndef CLASSMODEL_H_INCLUDED
#define CLASSMODEL_H_INCLUDED

#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include "Types.h"
#include "emlFormat.h"

class eModel
{
public:
    eModel() {}
    virtual ~eModel() {}

    bool loadModel(std::string path)
    {

        std::ifstream inFILE(std::string(path).c_str(), std::ios::binary);

        t_eml_header* h3 = new t_eml_header[1];
        t_Lumps lumps[3];

        inFILE.seekg(0, inFILE.beg);
        for(int i = 0 ; i < 1 ; i++)
            inFILE.read((char*)&h3[i], sizeof(t_eml_header));
        //cout << h3->format << endl;

        inFILE.seekg(sizeof(t_eml_header), inFILE.beg);
        for(int i = 0 ; i < 3 ; i++)
        {
            inFILE.read((char*)&lumps[i], sizeof(t_Lumps));
            //cout << lumps[i].offset << endl;
        }

        int numOfVerts = (lumps[lIndices].offset - sizeof(t_eml_header) + sizeof(t_Lumps) * 3) / sizeof(t_Vertex);
        std::cout << numOfVerts << std::endl;

        int numOfIndices = (lumps[lTexCoords].offset - sizeof(t_eml_header) + sizeof(t_Lumps) * 3 + sizeof(t_Vertex) * numOfVerts) / sizeof(t_Index);
        std::cout << numOfIndices << std::endl;

        t_Vertex* vert2 = new t_Vertex[numOfVerts];
        t_Index* index2 = new t_Index[numOfIndices];
        //t_TexCoord* texCoords2 = new t_TexCoord[model->meshes.at(0).vertices.size()];

        EpsilonModel::float3 f3;
        inFILE.seekg(lumps[lVertices].offset, inFILE.beg);
        for(int i = 0 ; i < numOfVerts ; i++)
            inFILE.read((char*)&vert2[i], sizeof(t_Vertex));

        for(int i = 0 ; i < numOfVerts ; i++)
        {
            f3.x = vert2[i].x;
            f3.y = vert2[i].y;
            f3.z = vert2[i].z;
            Vertices.push_back(f3);
        }

        inFILE.seekg(lumps[lIndices].offset, inFILE.beg);
        for(int i = 0 ; i < numOfIndices ; i++)
            inFILE.read((char*)&index2[i], sizeof(t_Index));


        for(int i = 0 ; i <  numOfIndices ; i++)
        {
            Indices.push_back(index2[i].index);
        }

        //cout << vert2[i].x << endl;
        /*
        inFILE.seekg(lumps[lTexCoords].offset, inFILE.beg);
        for(int i = 0 ; i < model->meshes.at(0).vertices.size() ; i++)
        inFILE.read((char*)&texCoords2[i], sizeof(t_TexCoord));

        for(int i = 0 ; i < model->meshes.at(0).vertices.size() ; i++)*/


    }

public:
    std::vector<EpsilonModel::float3> getVertices()
    {
        return this->Vertices;
    }

    std::vector<EpsilonModel::float2> getTexCoords()
    {
        return this->TexCoords;
    }

    std::vector<EpsilonModel::uint> getIndices()
    {
        return this->Indices;
    }

    int getNumOfVerts()
    {
        return this->Vertices.size();
    }

    int getNumOfIndices()
    {
        return this->Indices.size();
    }


private:

    std::vector<EpsilonModel::float3> Vertices;
    std::vector<EpsilonModel::float2> TexCoords;
    std::vector<EpsilonModel::uint> Indices;
};

#endif // CLASSMODEL_H_INCLUDED
