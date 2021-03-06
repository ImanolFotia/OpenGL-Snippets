#ifndef BSPFACE_H_INCLUDED
#define BSPFACE_H_INCLUDED

#include <glm/glm.hpp>
#include <GL/glew.h>
#include <iostream>
#include <string>
#include <vector>
#include <SOIL.h>

struct tBSPLightmap
{
    char imageBits[128][128][3];   // The RGB data in a 128x128 image
};

using namespace std;


class BSPFace
{
public:
    BSPFace()
    {

    }

    virtual ~BSPFace() {}

    bool BuildFace(std::vector<glm::vec3> Vertices, std::vector<glm::vec3> Normals, std::vector<glm::vec2> TexCoords, std::vector<glm::vec2> LMTexCoords, std::vector<unsigned int> Indices, int ID, string imagePath, tBSPLightmap LightMap)
    {
        //cout << "Face #" << ID << endl;
        this->Vertices = Vertices;

        this->TexCoords = TexCoords;

        this->Indices = Indices;
        this->Normals = Normals;
        this->faceID = ID;
        this->imagePath = imagePath;
        this->imagePath = this->imagePath;
        this->LightMap = LightMap;
        this->LMTexCoords = LMTexCoords;
this->CalcTangentSpace();
        bool res = this->prepareVAO();

        this->LoadLightMapTexture();


    }
public:

    void RenderFace(GLuint shader, GLuint TextureID,GLuint normalID)
    {
        glBindVertexArray(this->VAO);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, TextureID);
        glUniform1i(glGetUniformLocation(shader, "sampler"), 0);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, LightMaptexture);
        glUniform1i(glGetUniformLocation(shader, "LightMapsampler"), 1);

        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, normalID);
        glUniform1i(glGetUniformLocation(shader, "normalSampler"), 2);

        glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

    }

private:

    bool LoadLightMapTexture()
    {
        glGenTextures(1, &LightMaptexture);
        glPixelStorei (GL_UNPACK_ALIGNMENT, 1);
        glBindTexture(GL_TEXTURE_2D, LightMaptexture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 128, 128, 0, GL_RGB, GL_UNSIGNED_BYTE,this->LightMap.imageBits);
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    bool CalcTangentSpace()
    {

        // calculate tangent/bitangent vectors of both triangles
        glm::vec3 tangent1, bitangent1;
        for(int i = 0 ; i < Indices.size() ; i+=3)
        {

        glm::vec3 edge1 = Vertices.at(Indices.at((i+1))) - Vertices.at(Indices.at((i)));
        glm::vec3 edge2 = Vertices.at(Indices.at((i+2))) - Vertices.at(Indices.at((i)));

        glm::vec2 deltaUV1 = TexCoords.at(Indices.at((i+1))) - TexCoords.at(Indices.at((i)));
        glm::vec2 deltaUV2 = TexCoords.at(Indices.at((i+2))) - TexCoords.at(Indices.at((i)));

        GLfloat f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

        tangent1.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
        tangent1.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
        tangent1.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
        //tangent1 = glm::normalize(tangent1);

        Tangents.push_back(tangent1);
        Tangents.push_back(tangent1);
        Tangents.push_back(tangent1);
        bitangent1.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
        bitangent1.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
        bitangent1.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
        //bitangent1 = glm::normalize(bitangent1);

        Bitangents.push_back(bitangent1);
        Bitangents.push_back(bitangent1);
        Bitangents.push_back(bitangent1);
        }
        return true;
    }

    bool prepareVAO()
    {
        GLuint VBO, EBO, UVB, LMUVB, NBO, TBO, BTBO;

        glGenVertexArrays(1, &this->VAO);

        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);
        glGenBuffers(1, &UVB);
        glGenBuffers(1, &LMUVB);
        glGenBuffers(1, &NBO);
        glGenBuffers(1, &TBO);
        glGenBuffers(1, &BTBO);

        glBindVertexArray(this->VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        /*cout << "SizeOf Vertices: " << sizeof(glm::vec3) * Vertices.size() << endl;*/
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * Vertices.size(), &Vertices[0], GL_STATIC_DRAW);
        /*
        cout << "Vertices: " << Vertices.size() << endl;
        */
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (GLvoid*)0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, UVB);
        /*cout << "SizeOf UVS: " << sizeof(glm::vec2) * TexCoords.size() << endl;*/
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * TexCoords.size(), &TexCoords[0], GL_STATIC_DRAW);

        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (GLvoid*)0);
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, LMUVB);
        /*cout << "SizeOf LMUVS: " << sizeof(glm::vec2) * LMTexCoords.size() << endl;*/
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * LMTexCoords.size(), &LMTexCoords[0], GL_STATIC_DRAW);

        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (GLvoid*)0);
        glEnableVertexAttribArray(2);

        glBindBuffer(GL_ARRAY_BUFFER, NBO);
        /*cout << "SizeOf LMUVS: " << sizeof(glm::vec2) * LMTexCoords.size() << endl;*/
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * Normals.size(), &Normals[0], GL_STATIC_DRAW);

        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (GLvoid*)0);
        glEnableVertexAttribArray(3);

        glBindBuffer(GL_ARRAY_BUFFER, TBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * Tangents.size(), &Tangents[0], GL_STATIC_DRAW);
        /*
        cout << "Tangents: " << Tangents.size() << endl;*/
        glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (GLvoid*)0);
        glEnableVertexAttribArray(4);

        glBindBuffer(GL_ARRAY_BUFFER, BTBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * Bitangents.size(), &Bitangents[0], GL_STATIC_DRAW);

        glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (GLvoid*)0);
        glEnableVertexAttribArray(5);

        /*cout << "SizeOf indices: " << sizeof(unsigned int) * Indices.size() << endl;*/
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * Indices.size(), &Indices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        return true;
    }

private:

    GLuint VAO;

    int faceID;
    GLuint texture;
    string imagePath;
    tBSPLightmap LightMap;
    GLuint LightMaptexture;
    std::vector<glm::vec3> Vertices;
    std::vector<glm::vec3> Normals;
    std::vector<glm::vec2> TexCoords;
    std::vector<glm::vec2> LMTexCoords;
    std::vector<glm::vec3> Tangents;
    std::vector<glm::vec3> Bitangents;
    std::vector<unsigned int> Indices;

};

#endif // BSPFACE_H_INCLUDED
