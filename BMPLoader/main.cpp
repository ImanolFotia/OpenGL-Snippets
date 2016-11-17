#include <iostream>
#include <vector>
#include <BMPLoader.h>
#include <TGALoader.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <shader.h>

using namespace std;




int main()
{
vector<unsigned int> indices;
vector<float> quad;

    GLuint VAO, VBO, EBO;

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

    BMPImage image("image2.bmpZ");

    glfwInit();

    GLFWwindow* window = glfwCreateWindow(640, 480, "BMPLoader", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    glewExperimental = true;
    glewInit();

    glGenVertexArrays(1, &VAO);

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

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.m_Width, image.m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, &image.pixels[0]);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);

    GLuint shader = crearShader("vertex.glsl", "fragment.glsl");
    glViewport(0,0,640,480);
    glClearColor(0.0, 0.5, 0.4, 1.0);

    while(!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glBindVertexArray(VAO);
        glUseProgram(shader);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glUniform1i(glGetUniformLocation(shader, "texture"), 0);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glBindVertexArray(0);
        glfwSwapBuffers(window);
    }
    return 0;
}
