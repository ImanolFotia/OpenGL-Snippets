#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <iostream>
#include <cstdlib>
#include <shader.h>
#include <glm/glm.hpp>
#include <SOIL.h>

using namespace std;

typedef struct CursorPosition
{
    double x;
    double y;
} curPos;

GLuint loadTexture(string path)
{
    int width, height, channels;

    unsigned char* pixels = SOIL_load_image(path.c_str(), &width, &height, &channels, SOIL_LOAD_RGBA);
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
    delete[] pixels;
    glBindTexture(GL_TEXTURE_2D, 0);
    return texture;
}

CursorPosition GetCursor(GLFWwindow* window);
bool OnClick(GLFWwindow* window);
bool OnHover(CursorPosition Cursor, glm::vec4);
curPos cur;

class Control
{
public:
    Control(glm::vec2 Position, float sizex, float sizey, GLuint tex) : Pos(Position), x(sizex), y(sizey), texture(tex)
    {
        CalculateBoundingBox();
        CalculateBorders();
    }
    virtual ~Control() {
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteVertexArrays(1, &VAO);
    glDeleteTextures(1, &texture);
    cout << __FUNCTION__ << endl;
    }

public:
    glm::vec4 getBoundingBox()
    {
        return glm::vec4(BBL, BBR, BBT, BBB);
    }

    glm::vec2 getPosition()
    {
        return Pos;
    }

    void Render()
    {
        if(firstTime)
        {
            GLfloat Quad[32] =
            {
                // Positions          // Colors           // Texture Coords
                R,  T, 0.0f,   1.0f, 0.0f, 0.0f,   0.0f, 0.0f,   // Top Right
                R,  B, 0.0f,   0.0f, 1.0f, 0.0f,   0.0f, 1.0f,   // Bottom Right
                L,  B, 0.0f,   0.0f, 0.0f, 1.0f,   1.0f, 1.0f,   // Bottom Left
                L,  T, 0.0f,   1.0f, 1.0f, 0.0f,   1.0f, 0.0f    // Top Left
            };

            GLuint indices[6] =    // Note that we start from 0!
            {
                0, 1, 3, // First Triangle
                1, 2, 3  // Second Triangle
            };

            glGenVertexArrays(1, &VAO);
            glGenBuffers(1, &VBO);
            glGenBuffers(1, &EBO);

            glBindVertexArray(VAO);

            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(Quad), Quad, GL_STATIC_DRAW);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
            glEnableVertexAttribArray(0);

            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
            glEnableVertexAttribArray(1);

            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
            glEnableVertexAttribArray(2);
            glBindVertexArray(0);

            firstTime = false;
        }

        glBindVertexArray(VAO);
        glBindTexture(GL_TEXTURE_2D, texture);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

    }

private:
    void CalculateBorders()
    {
        L = (-x/2);
        R = (x/2);
        T = (y/2);
        B = (-y/2);
    }

    void CalculateBoundingBox()
    {
        BBL = (-x/2 + Pos.x);
        BBR = (x/2 + Pos.x);
        BBT = (y/2 + Pos.y);
        BBB = (-y/2 + Pos.y);
    }

private:
    glm::vec2 Pos;
    float x;
    float y;
    GLuint VAO;
    float L, R, T, B;
    float   BBL, BBR, BBT, BBB;
    bool firstTime = true;
    GLuint texture;
    GLuint VBO, EBO;
};

float width = 640;
float height = 480;

int main(int argc, char* argv[])
{
    if(!glfwInit())
    {
        cout << "GLFW could not be initialized" << endl;
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(width, height, "::EPSILON MODULE: GUI::", nullptr, nullptr);

    glfwMakeContextCurrent(window);

    glewExperimental = true;


    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    Control cont(glm::vec2(-0.5, 0.0), 0.75, 0.25, loadTexture("quit.png"));

    Control cont3(glm::vec2(-0.5, 0.25), 0.75, 0.25, loadTexture("button.png"));

    Control cont2(glm::vec2(0.0, 0.0), 2.0, 2.0, loadTexture("blur.png"));

    if(glewInit() != GLEW_OK)
    {
        cout << "GLEW could not be initialized" << endl;
        glfwTerminate();
        return -1;
    }

    glClearColor(.5f, .5f, .5f, 1.0f);

    GLuint shader = crearShader("gui.vglsl", "gui.fglsl");

    glViewport(0,0,width, height);

    /** Main Loop*/

    while(!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        glClear(GL_COLOR_BUFFER_BIT);

        cur = GetCursor(window);

        glm::vec4 contr = cont.getBoundingBox();
        cout << cur.x << "  " << cur.y << endl;
        if(OnHover(cur, contr))
        {
            cout << "hover" << endl;
            if(OnClick(window))
                return 0;
        }
        glUseProgram(shader);
        float color1 = 0, color2 = 0;
        if(OnHover(cur, cont.getBoundingBox()))
        {
            color1 = 0.1;
        }
        else
        {
            color1 = 1;
        }

        if(OnHover(cur, cont3.getBoundingBox()))
        {
            color2 = 0.1;
        }
        else
        {
            color2 = 1;
        }

        glUniform2f(glGetUniformLocation(shader, "pos"), cont2.getPosition().x, cont2.getPosition().y);
        glUniform1f(glGetUniformLocation(shader, "color"), 0.0);
        cont2.Render();

        glUniform2f(glGetUniformLocation(shader, "pos"), cont.getPosition().x, cont.getPosition().y);
        glUniform1f(glGetUniformLocation(shader, "color"), color1);
        cont.Render();

        glUniform2f(glGetUniformLocation(shader, "pos"), cont3.getPosition().x, cont3.getPosition().y);
        glUniform1f(glGetUniformLocation(shader, "color"), color2);
        cont3.Render();
        glUseProgram(0);

        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

CursorPosition GetCursor(GLFWwindow* window)
{
    curPos Cursor;

    glfwGetCursorPos(window, &Cursor.x, &Cursor.y);

    /** Converting to Viewport Space*/

    Cursor.x = Cursor.x / width;
    Cursor.y = Cursor.y / height;

    /** Converting to normalized device coordinates*/

    Cursor.x = Cursor.x * 2 - 1;
    Cursor.y = Cursor.y * 2 - 1;
    Cursor.y *= -1;

    return Cursor;
}

bool OnHover(CursorPosition Cursor, glm::vec4 Control)
{
    /** TOP LEFT*/    /** BOTTOM LEFT*/   /**TOP RIGHT*/    /**BOTTOM RIGHT*/
    if(Cursor.x > Control.x  && Cursor.x > Control.x    && Cursor.x < Control.y  &&  Cursor.x < Control.y)
    {
        if(Cursor.y < Control.z  && Cursor.y > Control.w   && Cursor.y < Control.z  &&  Cursor.y > Control.w)
        {
            return true;
        }
        else
            return false;
    }
    else
        return false;
}

bool OnClick(GLFWwindow* window)
{
    if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
        return true;
}


