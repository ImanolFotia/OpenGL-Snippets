#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <shader.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Quad
{
public:
    Quad()
    {
        GLfloat quadVertices[] = {-1.0, 1.0, 0.0, 0.0, 1.0,
                                  1.0, 1.0, 0.0, 1.0, 0.0,
                                  -1.0,-1.0, 0.0, 0.0, 0.0,
                                  1.0,-1.0, 0.0, 1.0, 0.0
                                 };

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*5, 0);

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*5, ((void*)(sizeof(GLfloat) * 3)));

        glBindVertexArray(0);
    }

    void Render(GLuint shader)
    {
        glBindVertexArray(VAO);
        glUseProgram(shader);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glUseProgram(0);
        glBindVertexArray(0);
    }

    GLuint VAO;

private:
    GLuint VBO;
};

class Cube
{
public:
    Cube()
    {
        GLfloat cubeVertices[] =
        {
            // Back face
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0, 0.0, 1.0, // Bottom-left
            0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0, 0.0, 1.0, // bottom-right
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0, 0.0, 1.0, // top-right
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0, 0.0, 1.0, // top-right
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0, 0.0, 1.0, // top-left
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0, 0.0, 1.0, // bottom-left
            // Front face
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0, 0.0, -1.0,// bottom-left
            0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0, 0.0, -1.0, // top-right
            0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0, 0.0, -1.0, // bottom-right
            0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0, 0.0, -1.0, // top-right
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0, 0.0, -1.0, // bottom-left
            -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0, 0.0, -1.0, // top-left
            // Left face
            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0, 0.0, 0.0, // top-right
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0, 0.0, 0.0, // bottom-left
            -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0, 0.0, 0.0, // top-left
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0, 0.0, 0.0, // bottom-left
            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0, 0.0, 0.0, // top-right
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0, 0.0, 0.0, // bottom-right
            // Right face
            0.5f,  0.5f,  0.5f,  1.0f, 0.0f, -1.0, 0.0, 0.0, // top-left
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f, -1.0, 0.0, 0.0, // top-right
            0.5f, -0.5f, -0.5f,  0.0f, 1.0f, -1.0, 0.0, 0.0, // bottom-right
            0.5f, -0.5f, -0.5f,  0.0f, 1.0f, -1.0, 0.0, 0.0, // bottom-right
            0.5f, -0.5f,  0.5f,  0.0f, 0.0f, -1.0, 0.0, 0.0, // bottom-left
            0.5f,  0.5f,  0.5f,  1.0f, 0.0f, -1.0, 0.0, 0.0, // top-left
            // Bottom face
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0, 1.0, 0.0, // top-right
            0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0, 1.0, 0.0, // bottom-left
            0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.0, 1.0, 0.0, // top-left
            0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0, 1.0, 0.0, // bottom-left
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0, 1.0, 0.0, // top-right
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0, 1.0, 0.0, // bottom-right
            // Top face
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0, -1.0, 0.0, // top-left
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0, -1.0, 0.0, // top-right
            0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0, -1.0, 0.0, // bottom-right
            0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0, -1.0, 0.0, // bottom-right
            -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0, -1.0, 0.0, // bottom-left
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0, -1.0, 0.0  // top-left
        };

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*8, 0);

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*8, ((void*)(sizeof(GLfloat) * 3)));

        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*8, ((void*)(sizeof(GLfloat) * 5)));

        glBindVertexArray(0);

    }

    void Render(GLuint shader)
    {
        glBindVertexArray(VAO);
        glUseProgram(shader);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glUseProgram(0);
        glBindVertexArray(0);
    }

    GLuint VAO;

private:

    GLuint VBO;
};

using namespace std;

int main()
{
    glfwInit();
    GLFWwindow* window = glfwCreateWindow(640, 480, "SSR", nullptr, nullptr);

    glfwMakeContextCurrent(window);

    glewExperimental = true;
    glewInit();

    GLuint shader = crearShader("vertex.glsl", "fragment.glsl");

    Cube *cube = new Cube();

    glClearColor(0.5, 0.8, 0.7, 1.0);

    glm::mat4 model;
    glm::mat4 view, projection;
    view = glm::lookAt(glm::vec3(1.0, 1.0, 1.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0,1,0));
    projection = glm::perspective((float)glm::radians(75.0f), 4.0f / 3.0f, 0.1f, 3000.0f);
    model = glm::translate(model, glm::vec3(0.0,0,0));
    glm::mat4 MVP = projection * view * model;
    while(!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shader);
        glUniformMatrix4fv(glGetUniformLocation(shader, "MVP"),1, GL_FALSE, &MVP[0][0]);
        cube->Render(shader);

        glfwSwapBuffers(window);
    }

    glfwTerminate();

    return 0;
}
