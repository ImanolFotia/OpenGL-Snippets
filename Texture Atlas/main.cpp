#include <iostream>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <SOIL/SOIL.h>
#include <shader.h>
#include "sprite.h"

using namespace std;


int main(int argc, char* argv[])
{

    glfwInit();

    GLFWwindow* window = glfwCreateWindow(640, 480, "Texture Atlas", 0, nullptr);

    glfwMakeContextCurrent(window);

    cout << "Animated sprites using texture atlases." << endl << endl;

    cout << "Press ESC to exit" << endl;
    cout << "Press Q to show the first sprite" << endl;
    cout << "Press E to show the second sprite" << endl << endl;
    cout << "(Both can be pressed at the same time)" << endl;

    glewExperimental = GL_TRUE;

    glewInit();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glViewport(0,0,640,480);
    glClearColor(0.0, 0.5, 0.4, 1.0);

    sprite* Sprite = new sprite("fire_atlas.jpg");
    sprite* Sprite2 = new sprite("exp.png");

    bool running = true;

    while(running)
    {
        if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            running = false;

        glfwPollEvents();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if(glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
             Sprite->Render();

        if(glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
             Sprite2->Render();


        glfwSwapBuffers(window);

    }
    return 0;
}
