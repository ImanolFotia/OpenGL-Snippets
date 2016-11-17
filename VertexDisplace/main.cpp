#include <iostream>

#define GLEW_STATIC

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <shader.h>
#include <vector>

using namespace std;

GLfloat triangle1[18] =
{
    0.0, 0.5, 0.0, 1.0, 0.0, 0.0,
    -0.5, -0.5, 0.0, 0.0, 1.0, 0.0,
    0.5, -0.5, 0.0,0.0, 0.0, 1.0
};

GLfloat triangle2[18] =
{
    0.0, 0.52, 0.0, 1.0, 0.0, 0.0,
    -0.5, -0.5, 0.0, 0.0, 1.0, 0.0,
    0.5, -0.5, 0.0,0.0, 0.0, 1.0
};

GLfloat triangle3[18] =
{
    0.0, 0.54, 0.0, 1.0, 0.0, 0.0,
    -0.5, -0.5, 0.0, 0.0, 1.0, 0.0,
    0.5, -0.5, 0.0,0.0, 0.0, 1.0
};

GLfloat triangle4[18] =
{
    0.0, 0.56, 0.0, 1.0, 0.0, 0.0,
    -0.5, -0.5, 0.0, 0.0, 1.0, 0.0,
    0.5, -0.5, 0.0,0.0, 0.0, 1.0
};

GLfloat triangle5[18] =
{
    0.0, 0.54, 0.0, 1.0, 0.0, 0.0,
    -0.5, -0.5, 0.0, 0.0, 1.0, 0.0,
    0.5, -0.5, 0.0,0.0, 0.0, 1.0
};

GLfloat triangle6[18] =
{
    0.0, 0.52, 0.0, 1.0, 0.0, 0.0,
    -0.5, -0.5, 0.0, 0.0, 1.0, 0.0,
    0.5, -0.5, 0.0,0.0, 0.0, 1.0
};

int main()
{
    glfwInit();
    GLFWwindow* window = glfwCreateWindow(640, 480, "VertexDisplacement", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    glewExperimental = true;
    glewInit();

    bool running = true;

    glClearColor(0.5, 0.5, 0.5, 1.0);

    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangle1), &triangle1[0],GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (GLvoid*)(sizeof(GLfloat) * 3));
    glEnableVertexAttribArray(1);
    glBindVertexArray(VAO);

    glBindVertexArray(VAO);

    GLuint shader = crearShader("vertex.glsl", "fragment.glsl");
    float i = 0;

    while(running)
    {
        glfwPollEvents();
        if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            running = false;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shader);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        switch((int)i)
        {
        case 1.0:
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(triangle1), &triangle1[0]);
            break;
        case 2.0:
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(triangle2), &triangle2[0]);
            break;
        case 3:
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(triangle3), &triangle3[0]);
            break;
        case 4:
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(triangle4), &triangle4[0]);
            break;
        case 5:
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(triangle5), &triangle5[0]);
            break;
        case 6:
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(triangle6), &triangle6[0]);
            break;
        }
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        i+=0.33;
        if(i > 6)
            i = 0;
    }

    return 0;
}
