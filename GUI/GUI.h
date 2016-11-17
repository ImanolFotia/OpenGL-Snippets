#ifndef GUI_H_INCLUDED
#define GUI_H_INCLUDED

#include "GUIDeclarations.h"
#include <vector>

#include <GL/glew.h>

#include <GLFW/glfw3.h>


class GUI
{

public:

    GUI(int WIN_WIDTH, int WIN_HEIGHT, GLFWwindow* window);

    virtual ~GUI() {}

public:

    bool gAddButton(ScreenPosition scrPos, double size_w, double size_h, void*(func_Pointer)());

    bool gPollEvents();

    void gRenderGUI();

private:

    std::vector<Button> gButtons;
    int WIN_WIDTH;
    int WIN_HEIGHT;
    GLFWwindow* window;

};

class GUIContext
{

}

class Button : public GUI
{

public:

    Button(ScreenPosition scrPos, double size_w, double size_h, void*(func_Pointer)());

    virtual ~Button() {}

public:

    void bRender();

    void bOnClick();

    bool bIsHover();

    void bPollEvents();


private:

    void setupAttributes();

    void LoadShaders();

    CursorPosition GetCursor();

    void*(func_Pointer)();

    ScreenPosition scrPos;

    double bButtonSize_w;
    double bButtonSize_h;

    unsigned int Shader;

    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;

};



#endif // GUI_H_INCLUDED
