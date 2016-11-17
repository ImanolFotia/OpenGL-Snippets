#include <iostream>

#define GLEW_STATIC

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SOIL.h>
#include <vector>

#include <shader.h>

using namespace std;
const float width = 800;
const float height = 600;


struct Line{
float x;
float y;
};

int main(int argc, char* argv[])
{
    const char* imagen = argv[1];

    if(!glfwInit())
    {
        cout << "No se pudo iniciar GLFW" << endl;
        return -1;
    }

    std::vector<Line> Lines;
/*
    for(float i = 0 ; i < width ; i+=30)
            {
                Line line;
                line.x = (i / width) * 2 - 1;
                line.y = 1.0;
                Lines.push_back(line);
                cout << "X: " << line.x << " Y: " << line.y << endl;
                line.y = -1.0;
                Lines.push_back(line);
                cout << "X: " << line.x << " Y: " << line.y << endl;
            }

    for(float i = 0 ; i < height ; i+=30)
            {
                Line line;
                line.x = 1.0;
                line.y = (i / height) * 2 - 1;
                Lines.push_back(line);
                line.x = -1.0;
                Lines.push_back(line);
            }
*/


    GLFWwindow* window = glfwCreateWindow(800, 600, "Ejemplo OpenGL", nullptr, nullptr);

    if(!window)
    {
        cout << "No se pudo abrir la ventana." << endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    glewExperimental = true;
{
    int a;
}
    int a;

    if(glewInit() != GLEW_OK)
    {
        cout << "No se pudo iniciar GLEW." << endl;
        glfwTerminate();
        return -1;
    }

    int width, height, channels;
    unsigned char* image = SOIL_load_image(imagen, &width, &height, &channels, SOIL_LOAD_RGB);
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE,image);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(image);
    glBindTexture(GL_TEXTURE_2D, 0);

    GLuint VAO, VBO, EBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Line) * Lines.size(), &Lines[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Line), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    GLuint shader = crearShader("vertex.glsl", "fragment.glsl");

    bool running = true;
    int wwidth, wheight;

    double xpos, ypos;

    while(running)
    {
        glfwGetWindowSize(window, &wwidth, &wheight);

        glViewport(0, 0, wwidth, wheight);

        if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        running = false;

        glfwPollEvents();

        glClearColor(.5, .5, .5, 1.0f);

        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shader);

        glBindVertexArray(VAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glfwGetCursorPos(window, &xpos, &ypos);
        glUniform2f(glGetUniformLocation(shader, "mousepos"), xpos, ypos);
        cout << "X: " << xpos << " Y: " << ypos << endl;
        glUniform1f(glGetUniformLocation(shader, "time"), glfwGetTime());
        glDrawArrays(GL_LINES, 0, Lines.size());
        glBindVertexArray(0);

        glUseProgram(0);

        glfwSwapBuffers(window);
    }

    glfwTerminate();

    return 0;
}
