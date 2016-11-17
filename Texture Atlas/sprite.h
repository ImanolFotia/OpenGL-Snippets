#ifndef SPRITE_H_INCLUDED
#define SPRITE_H_INCLUDED

#include <string>
#include <GL/glew.h>
#include <iostream>
#include <vector>

using namespace std;


class sprite
{

public:
    sprite(std::string path)
    {
        texPath = path;

        createMesh();

        PrepareVAO();
        LoadTexture();
        LoadShader();

    }

    virtual ~sprite()
    {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    }
public:
    void Render()
    {
        glUseProgram(shader);

        framecount++;
        if(framecount > 2)
        {
            x = x + 0.25;

            acum++;
            if(acum == 4)
            {
                y = y + 0.25;
                x = 0.0;
                acum = 0;
            }

            if(y > 0.75)
                y = 0;
            if(x > 0.75)
                x = 0;
            framecount = 0;
        }

        glUniform1f(glGetUniformLocation(shader, "x"), x);
        glUniform1f(glGetUniformLocation(shader, "y"), y);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glUniform1i(glGetUniformLocation(shader, "sampler"), 0);

        glBindVertexArray(this->VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glUseProgram(0);
    }

private:
    bool PrepareVAO()
    {

        glGenVertexArrays(1, &this->VAO);

        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * quad.size(), &quad[0], GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, (GLvoid*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, (GLvoid*)(3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(), &indices[0], GL_STATIC_DRAW);

        glBindVertexArray(0);
    }

    GLuint LoadTexture()
    {
        int width, height, channels;

        unsigned char* pixels = SOIL_load_image(texPath.c_str(), &width, &height, &channels, SOIL_LOAD_RGBA);

        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
        delete[] pixels;
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    GLuint LoadShader()
    {

        shader = crearShader("vertex.glsl", "fragment.glsl");
    }

    bool createMesh()
    {

        indices.push_back(0);
        indices.push_back(1);
        indices.push_back(3);
        indices.push_back(1);
        indices.push_back(2);
        indices.push_back(3);

        quad.push_back(1.0);
        quad.push_back(1.0);
        quad.push_back(0.0);
        quad.push_back(0.0);
        quad.push_back(0.0);

        quad.push_back(1.0);
        quad.push_back(-1.0);
        quad.push_back(0.0);
        quad.push_back(0.0);
        quad.push_back(1.0);

        quad.push_back(-1.0);
        quad.push_back(-1.0);
        quad.push_back(0.0);
        quad.push_back(1.0);
        quad.push_back(1.0);

        quad.push_back(-1.0);
        quad.push_back(1.0);
        quad.push_back(0.0);
        quad.push_back(1.0);
        quad.push_back(0.0);

    }

private:
    GLuint shader;
    GLuint VAO;
    GLuint EBO;
    GLuint VBO;
    GLuint texture;
    std::string texPath;
    float x = -0.25;
    float y;
    int framecount = 0;
    int acum = 0;

    vector<GLuint> indices;
    vector<GLfloat> quad;

};

#endif // SPRITE_H_INCLUDED
