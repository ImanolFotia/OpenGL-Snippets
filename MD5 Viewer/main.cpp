#include <iostream>
#define GLEW_STATIC

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "MD5_Anim.h"
#include "MD5_Model.h"
#include <shader.h>
#include <glm/gtc/matrix_transform.hpp>
#include <AntTweakBar.h>
#include "camera.h"

// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback();
void Do_Movement();

inline void TwEventMouseButtonGLFW3(GLFWwindow* window, int button, int action, int mods)
{
    TwEventMouseButtonGLFW(button, action);
}
inline void TwEventMousePosGLFW3(GLFWwindow* window, double xpos, double ypos)
{
    TwMouseMotion(int(xpos), int(ypos));
}
inline void TwEventMouseWheelGLFW3(GLFWwindow* window, double xoffset, double yoffset)
{
    TwEventMouseWheelGLFW(yoffset);
}
inline void TwEventKeyGLFW3(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    TwEventKeyGLFW(key, action);
}
inline void TwEventCharGLFW3(GLFWwindow* window, int codepoint)
{
    TwEventCharGLFW(codepoint, GLFW_PRESS);
}
inline void TwWindowSizeGLFW3(GLFWwindow* window, int width, int height)
{
    TwWindowSize(width, height);
}

bool keys[1024];
GLfloat lastX = 400, lastY = 300;

using namespace std;
bool firstMouse = true;
const int width = 800;
const int height = 600;
Camera camera(glm::vec3(100,100,100));

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;
bool running = true;
bool pause = false;
bool bones = false;
bool joints = false;
float mix = 0.0;
bool showmodel = true;
double dir[3] = {0.5, 0.5, 0.5};
void TW_CALL RUN_cb(void* clientstate)
{
    running = false;
}

void TW_CALL PAUSE(void* clientstate)
{
    pause = !pause;
}

GLfloat soil[] =
{
    -600.0, 0.0, 600.0, 0.0, 0.1, 0.0,
    600.0, 0.0, 600.0, 0.0, 0.1, 0.0,
    -600.0, 0.0, -600.0, 0.0, 0.1, 0.0,
    600.0, 0.0, -600.0, 0.0, 0.1, 0.0
};

GLfloat axis[] =
{
    0.0, 0.0, 0.0, 1.0, 0.0, 0.0,
    0.0, 40.0, 0.0, 1.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 1.0, 0.0,
    40.0, 0.0, 0.0, 0.0, 1.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 1.0,
    0.0, 0.0, 40.0, 0.0, 0.0, 1.0
};

GLFWwindow* window;
int main()
{
    int FPS;
    float blend = 1.0;
    float timestep = 0.016;
    if(!glfwInit())
    {
        cout << "No se pudo iniciar GLFW" << endl;
        return -1;
    }

    window = glfwCreateWindow(width, height, "Epsilon Model Viewer", nullptr, nullptr);

    glfwMakeContextCurrent(window);

    glewExperimental = true;
    glewInit();

    TwInit(TW_OPENGL, nullptr);
    TwWindowSize(width, height);
    glEnable(GL_PROGRAM_POINT_SIZE);

    TwBar *myBar;
    myBar = TwNewBar("Main");
    TwAddVarRW(myBar, "Salir", TW_TYPE_INT32, &FPS,
               " label='FPS' help='Color and transparency of the cube.' ");
    TwAddVarRW(myBar, "timestep", TW_TYPE_FLOAT, &timestep,
               " label='Timestep' help='Color and transparency of the cube.' step = 0.001");
    TwAddVarRW(myBar, "blendfactor", TW_TYPE_FLOAT, &blend,
               " label='Blend Factor' help='Color and transparency of the cube.' step = 0.001 ");
    TwAddVarRW(myBar, "texmix", TW_TYPE_FLOAT, &mix,
               " label='Texture Mix' help='Color and transparency of the cube.' step = 0.001 ");
    TwDefine(" GLOBAL help='This example shows how to integrate AntTweakBar with GLFW and OpenGL.' ");
    TwDefine("Main/blendfactor  min=0 max=1");
    TwDefine("Main/texmix  min=0 max=1");
    TwDefine("Main/timestep  min=-0.50 max=0.50");
    TwAddVarRW(myBar, "LightDir", TW_TYPE_DIR3D, &dir, " label='Light Direction' visible=true");
    TwDefine(" Main/LightDir opened=true ");
    TwAddVarRW(myBar, "bones", TW_TYPE_BOOL8, &bones,
               " label='Render Bones' help='Color and transparency of the cube.' ");
    TwAddVarRW(myBar, "model", TW_TYPE_BOOL8, &showmodel,
               " label='Render Model' help='Color and transparency of the cube.'");
    TwAddVarRW(myBar, "joints", TW_TYPE_BOOL8, &joints,
               " label='Render Joints' help='Color and transparency of the cube.'");
    TwAddButton(myBar, "Pause", PAUSE, NULL, " label='Pause' ");
    TwAddButton(myBar, "Exit", RUN_cb, NULL, " label='Exit' ");

    glfwSetMouseButtonCallback(window, (GLFWmousebuttonfun)TwEventMouseButtonGLFW3);
    glfwSetCursorPosCallback(window, (GLFWcursorposfun)TwEventMousePosGLFW3);
    glfwSetScrollCallback(window, (GLFWscrollfun)TwEventMouseWheelGLFW3);
    glfwSetKeyCallback(window, (GLFWkeyfun)TwEventKeyGLFW3);
    glfwSetCharCallback(window, (GLFWcharfun)TwEventCharGLFW3);

    //glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    MD5Model *Model = new MD5Model();

    Model->LoadModel("models/md5/monsters/cyberdemon/cyberdemon.md5mesh");
    Model->LoadAnim("models/md5/monsters/cyberdemon/idle.md5anim");
    Model->LoadAnim("models/md5/monsters/cyberdemon/turret3.md5anim");

    GLuint shader = crearShader("VertexShader.glsl", "FragmentShader.glsl");
    GLuint shader2 = crearShader("FloorShaderv.glsl", "FloorShaderf.glsl");

    //floor
    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(soil), &soil[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (GLvoid*)(sizeof(GLfloat) * 3));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    //axis
    GLuint aVBO, aVAO;
    glGenVertexArrays(1, &aVAO);
    glGenBuffers(1, &aVBO);

    glBindVertexArray(aVAO);

    glBindBuffer(GL_ARRAY_BUFFER, aVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(axis), &axis[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (GLvoid*)(sizeof(GLfloat) * 3));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    glClearColor(0, 0, 0, 1.0);

    glm::mat4 model = glm::mat4(1);

    glm::mat4 view;
    glEnable(GL_DEPTH_TEST);

    TwSetCurrentWindow(0);
    while(running)
    {
        // Set frame time
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        Do_Movement();
        if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        }
        else
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }

        FPS = 1/deltaTime;
        //glfwSetWindowTitle(window, ("FPS: " + to_string(1/deltaTime)).c_str());
        if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            running = false;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shader);
        //Render Model
        GLint modelLoc = glGetUniformLocation(shader, "model");
        GLint viewLoc = glGetUniformLocation(shader, "view");
        GLint projLoc = glGetUniformLocation(shader, "projection");

        view = camera.GetViewMatrix();
        glm::mat4 projection;
        projection = glm::perspective(90.0f, (float)width/(float)height, 0.1f, 10000.0f);
        glm::mat4 model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(0,70,0));
        model = glm::rotate(model,270.0f, glm::vec3(1,0,0));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, &projection[0][0]);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);
        glUniform3f(glGetUniformLocation(shader, "viewPos"), camera.Position.x,  camera.Position.y,  camera.Position.z);
        glUniform3f(glGetUniformLocation(shader, "lightDir"), dir[0],  dir[1], dir[2]);
        glUniform1f(glGetUniformLocation(shader, "texmix"), mix);

        if(!pause)
            Model->Update(timestep, blend);

        if(showmodel)
            Model->Render(shader);

        //floor
        glUseProgram(shader2);
        model = glm::mat4();
        glUniformMatrix4fv(glGetUniformLocation(shader2, "view"), 1, GL_FALSE, &view[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(shader2, "projection"), 1, GL_FALSE, &projection[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(shader2, "model"), 1, GL_FALSE, &model[0][0]);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glBindVertexArray(0);

        glDisable(GL_DEPTH_TEST);
        model = glm::translate(model, glm::vec3(0,70,0));
        model = glm::rotate(model,270.0f, glm::vec3(1,0,0));
        glUniformMatrix4fv(glGetUniformLocation(shader2, "view"), 1, GL_FALSE, &view[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(shader2, "projection"), 1, GL_FALSE, &projection[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(shader2, "model"), 1, GL_FALSE, &model[0][0]);
        if(bones)
            Model->RenderSkeleton();
        if(joints)
            Model->RenderJoints();
        glEnable(GL_DEPTH_TEST);

        model = glm::mat4();
        //axis
        glUseProgram(shader2);
        model = glm::mat4();
        glUniformMatrix4fv(glGetUniformLocation(shader2, "view"), 1, GL_FALSE, &view[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(shader2, "projection"), 1, GL_FALSE, &projection[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(shader2, "model"), 1, GL_FALSE, &model[0][0]);
        glBindVertexArray(aVAO);
        glDrawArrays(GL_LINES, 0, 6);
        glBindVertexArray(0);
        glfwPollEvents();
        TwDraw();

        glfwSwapBuffers(window);

    }

    glfwDestroyWindow(window);
    glfwTerminate();
    TwTerminate();
    return 0;
}

// Moves/alters the camera positions based on user input
void Do_Movement()
{
    // Camera controls
    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);

    mouse_callback();
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

void mouse_callback()
{
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
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

    if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
        camera.ProcessMouseMovement(xoffset, yoffset);
}


void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}


