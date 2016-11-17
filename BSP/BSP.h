#ifndef BSP_H_INCLUDED
#define BSP_H_INCLUDED

#define MAX_TEXTURES 1000
#define FACE_POLYGON	1
#define FACE_MESHVERT   3

#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <vector>
#include <map>

#include <GLFW/glfw3.h>

#include "BSPFace.h"
#include "Frustum.h"

typedef char byte;


static GLuint LoadTexture(string imagePath)
{
    int width, height, channels;/*
    cout << imagePath << endl;*/
    unsigned char* image;
    GLuint texture;
    cout << "assets/" + imagePath + ".jpg" << endl;
    if(image = SOIL_load_image(string("assets/" + imagePath + ".jpg").c_str(), &width, &height, &channels, SOIL_LOAD_RGBA))
    {
    }
    else if(image = SOIL_load_image(string(imagePath + ".tga").c_str(), &width, &height, &channels, SOIL_LOAD_RGBA))
    {

    }
    else
    {
        return false;
    }
    //cout << imagePath << endl;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE,image);

    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(image);
    glBindTexture(GL_TEXTURE_2D, 0);
    return texture;
}

struct tVector3i
{
    int x, y, z;
};

struct CVector3
{
    float x, y, z;
};

struct CVector2
{
    float x, y;
};

struct tBSPHeader
{
    char strID[4];
    int version;
};



struct tBSPLump
{
    int offset;
    int length;
};


struct tBSPVertex
{
    glm::vec3 vPosition;
    glm::vec2 vTextureCoord;
    glm::vec2 vLightmapCoord;
    glm::vec3 vNormal;
    byte color[4];
};



struct tBSPFace
{
    int textureID;
    int effect;
    int type;
    int startVertIndex;
    int numOfVerts;
    int startIndex;
    int numOfIndices;
    int lightmapID;
    int lMapCorner[2];
    int lMapSize[2];
    CVector3 lMapPos;
    CVector3 lMapVecs[2];
    CVector3 vNormal;
    int size[2];
};

struct tBSPTexture
{
    char strName[64];
    int flags;
    int contents;
};

struct Texture
{
string path;
GLuint GLTextureID;
int type;
};


enum eLumps
{
    kEntities = 0,
    kTextures,
    kPlanes,
    kNodes,
    kLeafs,
    kLeafFaces,
    kLeafBrushes,
    kModels,
    kBrushes,
    kBrushSides,
    kVertices,
    kIndices,
    kShaders,
    kFaces,
    kLightmaps,
    kLightVolumes,
    kVisData,
    kMaxLumps
};


struct tBSPNode
{
    int plane;
    int front;
    int back;
    tVector3i min;
    tVector3i max;
};

struct tBSPLeaf
{
    int cluster;
    int area;
    tVector3i min;
    tVector3i max;
    int leafface;
    int numOfLeafFaces;
    int leafBrush;
    int numOfLeafBrushes;
};


struct tBSPPlane
{
    CVector3 vNormal;
    float d;
};

struct tBSPVisData
{
	int numOfClusters;
	int bytesPerCluster;
	byte *pBitsets;
};

class CBitset
{

public:


    CBitset() : m_bits(0), m_size(0) {}


    ~CBitset()
    {

        if(m_bits)
        {
            delete m_bits;
            m_bits = NULL;
        }
    }

    void Resize(int count)
    {

        m_size = count/32 + 1;

        if(m_bits)
        {
            delete m_bits;
            m_bits = 0;
        }

        m_bits = new unsigned int[m_size];
        ClearAll();
    }

    void Set(int i)
    {
        m_bits[i >> 5] |= (1 << (i & 31));
    }

    int On(int i)
    {
        return m_bits[i >> 5] & (1 << (i & 31 ));
    }

    void Clear(int i)
    {
        m_bits[i >> 5] &= ~(1 << (i & 31));
    }

    void ClearAll()
    {
        memset(m_bits, 0, sizeof(unsigned int) * m_size);
    }

private:

    unsigned int *m_bits;
    int m_size;
};


class CQuake3BSP
{

public:

    CQuake3BSP();

    ~CQuake3BSP();

    bool LoadBSP(const char *strFileName);

    void RenderLevel(glm::vec3 vPos, GLuint shader, GLFWwindow * window);

    void Destroy();

    CFrustum Frustum;

private:

    void FindTextureExtension(char *strFileName);

    void RenderFace(int faceIndex, GLuint shader);

    int IsClusterVisible(int current, int test);

	int FindLeaf(glm::vec3 vPos);

    int  m_numOfVerts;
    int  m_numOfFaces;
    int  m_numOfIndices;
    int  m_numOfTextures;
    int  m_numOfLightmaps;


	int m_numOfNodes;
	int m_numOfLeafs;
	int m_numOfLeafFaces;
	int m_numOfPlanes;

    int			*m_pIndices;
    tBSPVertex  *m_pVerts;
    tBSPFace	*m_pFaces;


	tBSPNode    *m_pNodes;
	tBSPLeaf    *m_pLeafs;
	tBSPPlane   *m_pPlanes;
	int         *m_pLeafFaces;
	tBSPVisData  m_clusters;

    unsigned int m_textures[MAX_TEXTURES];

    unsigned int* VertexBufferArrays;

    CBitset m_FacesDrawn;

    std::map<int, BSPFace> Faces;
    std::vector<Texture> Textures;
    std::vector<Texture> normalTextures;
    std::map<int, Texture> faceTexture;
    std::map<int, Texture> faceTexture_normal;

    bool lightmap;
    bool color;
    int faceCount;
    GLuint NormalTextureID;

};


#endif // BSP_H_INCLUDED
