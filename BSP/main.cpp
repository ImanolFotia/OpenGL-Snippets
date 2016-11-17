#include <iostream>

#define GLEW_STATIC

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <shader.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "BSP.h"
#include "camera.h"

using namespace std;

// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void Do_Movement();

bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

int width = 1366;
int height = 768;

Camera camera(glm::vec3(0,5,0));

int main(int argc, char* argv[])
{
    if(!glfwInit())
        cout << "No se puedo iniciar GLFW." << endl;

    GLFWwindow* window = glfwCreateWindow(width, height, "::BSP LOADER::", glfwGetPrimaryMonitor(), nullptr);

    glfwMakeContextCurrent(window);

    glewExperimental = true;

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    if(glewInit() != GLEW_OK)
    {
        cout << "No se pudo iniciar GLEW" << endl;
        glfwTerminate();
    }

    glViewport(0,0,width,height);

    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    GLuint shader = crearShader("assets/shaders/vertex.glsl", "assets/shaders/fragment.glsl");

    bool running = true;

    CQuake3BSP* BSPMap = new CQuake3BSP();

    BSPMap->LoadBSP((string("assets/maps/") + "aomap.bsp").c_str());

    CVector3 pos = {0,0,0};

    glm::mat4 model = glm::mat4(1);
    model = glm::scale(model, glm::vec3(0.1, 0.1, 0.1));

    bool PVSTesting = true;
    glm::vec3 Position;

    glfwSwapInterval(0);

    long long int totalframes = 0;
    double starttime = glfwGetTime();
    double higher = 0;
    double lower = 1000;
    int startpoint = 0;
    double frametime = 0;
    glm::mat4 ViewProj;

        glm::mat4 normalmatrix = glm::transpose(glm::inverse(model));

        glm::mat4 projection;

        projection = glm::perspective(90.0f, (float)800/(float)600, 0.1f, 10000.0f);
        const float *pSource = (const float*)glm::value_ptr(projection);
        for(int i = 0; i < 16;i++)
        cout << "Matrix:" << pSource[i] << endl;

        projection = glm::perspective(90.0f, (float)width/(float)height, 0.1f, 10000.0f);

    while(running)
    {
        // Set frame time
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        glfwPollEvents();
        Do_Movement();
        //glfwSetCursorPos(window, 640/2, 480/2);

        if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            running = false;

        if(glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
            PVSTesting = !PVSTesting;

        //glfwSetWindowTitle(window, ("FPS: " + to_string(1/deltaTime)).c_str());
        glfwSetWindowTitle(window, ("Position: X: " + to_string(Position.x) + " Y: " + to_string(Position.y) + " Z: " + to_string(Position.z)).c_str());
        glClearColor(0.5,0.5,0.5,1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shader);

        // Create camera transformation
        glm::mat4 view;
        view = camera.GetViewMatrix();
        // Get the uniform locations
        GLint modelLoc = glGetUniformLocation(shader, "model");
        GLint viewLoc = glGetUniformLocation(shader, "view");
        GLint projLoc = glGetUniformLocation(shader, "projection");
        GLint normLoc = glGetUniformLocation(shader, "normalmatrix");
        // Pass the matrices to the shader

        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, &projection[0][0]);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);
        glUniformMatrix4fv(normLoc, 1, GL_FALSE, &normalmatrix[0][0]);


        if(PVSTesting){
        Position = camera.Position;
        ViewProj = projection*view;
}
        //glfwSetWindowTitle(window, ("FPS: " + to_string(PVSTesting)).c_str());

        glUniform3f(glGetUniformLocation(shader, "viewPos"), Position.x, Position.y, Position.z);
        glViewport(0,0,width,height);

        BSPMap->Frustum.CalculateFrustum(ViewProj, model);

        BSPMap->RenderLevel(Position, shader, window);

        glUseProgram(0);

        glfwSwapBuffers(window);

        if(1/deltaTime > higher)
            higher = 1/deltaTime;
        if(1/deltaTime < lower && totalframes > 60){
            lower = 1/deltaTime;
            }
        totalframes++;
        frametime += deltaTime;
    }

    cout << "Average Framerate: " << totalframes/(glfwGetTime() - starttime) << endl;
    cout << "Average Frametime: " << frametime/totalframes << endl;
    cout << "Minimum: " << lower<< endl;
    cout << "Maximum: " << higher<< endl;


    glfwTerminate();

    return 0;
}

// Moves/alters the camera positions based on user input
void Do_Movement()
{
    // Camera controls
    if(keys[GLFW_KEY_W])
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if(keys[GLFW_KEY_S])
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if(keys[GLFW_KEY_A])
        camera.ProcessKeyboard(LEFT, deltaTime);
    if(keys[GLFW_KEY_D])
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    //cout << key << endl;
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    if (key >= 0 && key < 1024)
    {
        if(action == GLFW_PRESS)
            keys[key] = true;
        else if(action == GLFW_RELEASE)
            keys[key] = false;
    }
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if(firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    GLfloat xoffset = xpos - lastX;
    GLfloat yoffset = lastY - ypos;  // Reversed since y-coordinates go from bottom to left

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}


void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}
