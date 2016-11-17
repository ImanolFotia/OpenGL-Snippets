#include "GUI.h"





namespace GUI
{

/**GUI INITALIZATION AND FUNCTIONS*/

GUI::GUI(int WIN_WIDTH, int WIN_HEIGHT, GLFWwindow* window)
{

}

void GUI::gRenderGUI()
{
    for(int i = 0 ; i < gButtons.size() ; i++)
    {
        gButtons.at(i).bRender();
    }
}

bool GUI::gAddButton(ScreenPosition scrPos, double size_w, double size_h, void*(func_Pointer)())
{
    gButtons.push_back(Button(scrPos, size_w, size_h, func_Pointer));
}

bool GUI::gPollEvents()
{

}

/**BUTTON INITALIZATION AND FUNCTIONS*/

Button::Button(ScreenPosition scrPos, double size_w, double size_h, void*(func_Pointer)())
{
    this->func_pointer = func_Pointer;
    this->bButtonSize_w = size_w;
    this->bButtonSize_h = size_h;
    this->scrPos = scrPos;
}

void Button::bOnClick()
{
    if(this->bIdHover() && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
    func_pointer();
}

bool Button::bIsHover()
{
       /** TOP LEFT*/    /** BOTTOM LEFT*/   /**TOP RIGHT*/    /**BOTTOM RIGHT*/
    if(Cursor.x > -1  && Cursor.x > -1    && Cursor.x < 0.5  &&  Cursor.x < 0.5)
    {
    if(Cursor.y < 1  && Cursor.y > 0.25   && Cursor.y < 1.0  &&  Cursor.y > 0.25)
    {
    return true;
    }
    else
    return false;
    }
    else
    return false;
}

void Button::bRender()
{
        glUseProgram(shader);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        glUseProgram(0);
}

void Button::setupAttributes()
{
    GLfloat Quad[] =
    {
        // Positions          // Colors           // Texture Coords
        0.5f,  1.0f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // Top Right
        0.5f,  0.25f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // Bottom Right
        -1.0f,  0.25f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // Bottom Left
        -1.0f,  1.0f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // Top Left
    };

    GLuint indices[] =    // Note that we start from 0!
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
}

void Button::LoadShaders()
{
    shader = crearShader("gui.vglsl", "gui.fglsl");
}

void Button::bPollEvents()
{

}

CursorPosition Button::GetCursor()
{
    curPos Cursor;

    glfwGetCursorPos(window, &Cursor.x, &Cursor.y);

    /** Converting to Vieport Space*/

    Cursor.x = Cursor.x / WIN_WIDTH;
    Cursor.y = Cursor.y / WIN_HEIGHT;

    /** Converting to normalized device coordinates*/

    Cursor.x = Cursor.x * 2 - 1;
    Cursor.y = Cursor.y * 2 - 1;
    Cursor.y *= -1;

    return Cursor;
}

};
