#include <iostream>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#define GLFW_STATIC
#include <shader.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "camera.h"
#include <AntTweakBar.h>
#include <SOIL.h>
#include <string>
using namespace std;
// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback();
GLuint loadCubemap(vector<string> faces);
GLuint loadTexture(string path);
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
void Do_Movement();

    float samplingpattern;
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

Camera camera(glm::vec3(0,10,0));


GLfloat cube[] =
{
    // Positions          // Normals           // Texture Coords
    200.0f,  0.0f, 200.0f,   1.0f, 0.0f, 0.0f,   0.0f, 0.0f,   // Top Right
    200.0f, 0.0f, -200.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // Bottom Right
    -200.0f, 0.0f, -200.0f,   0.0f, 0.0f, 1.0f,   1.0f, 1.0f,   // Bottom Left
    -200.0f,  0.0f, 200.0f,   1.0f, 1.0f, 1.0f,   0.0f, 1.0f    // Top Left
};

GLuint indices[] =    // Note that we start from 0!
{
    0, 1, 3, // First Triangle
    1, 2, 3  // Second Triangle
};

GLfloat skyboxVertices[] =
{
    // Positions
    -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

    1.0f, -1.0f, -1.0f,
    1.0f, -1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,
    1.0f,  1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,
    1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

    -1.0f,  1.0f, -1.0f,
    1.0f,  1.0f, -1.0f,
    1.0f,  1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
    1.0f, -1.0f,  1.0f
};

GLuint quadVAO = 0;
GLuint quadVBO;
void RenderQuad()
{
    if (quadVAO == 0)
    {
        // positions
        glm::vec3 pos1(-1.0, 1.0, 0.0);
        glm::vec3 pos2(-1.0, -1.0, 0.0);
        glm::vec3 pos3(1.0, -1.0, 0.0);
        glm::vec3 pos4(1.0, 1.0, 0.0);
        // texture coordinates
        glm::vec2 uv1(0.0, 1.0);
        glm::vec2 uv2(0.0, 0.0);
        glm::vec2 uv3(1.0, 0.0);
        glm::vec2 uv4(1.0, 1.0);
        // normal vector
        glm::vec3 nm(0.0, 0.0, 1.0);

        // calculate tangent/bitangent vectors of both triangles
        glm::vec3 tangent1, bitangent1;
        glm::vec3 tangent2, bitangent2;
        // - triangle 1
        glm::vec3 edge1 = pos2 - pos1;
        glm::vec3 edge2 = pos3 - pos1;
        glm::vec2 deltaUV1 = uv2 - uv1;
        glm::vec2 deltaUV2 = uv3 - uv1;

        GLfloat f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

        tangent1.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
        tangent1.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
        tangent1.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
        tangent1 = glm::normalize(tangent1);

        bitangent1.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
        bitangent1.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
        bitangent1.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
        bitangent1 = glm::normalize(bitangent1);

        // - triangle 2
        edge1 = pos3 - pos1;
        edge2 = pos4 - pos1;
        deltaUV1 = uv3 - uv1;
        deltaUV2 = uv4 - uv1;

        f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

        tangent2.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
        tangent2.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
        tangent2.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
        tangent2 = glm::normalize(tangent2);


        bitangent2.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
        bitangent2.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
        bitangent2.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
        bitangent2 = glm::normalize(bitangent2);


        GLfloat quadVertices[] = {
            // Positions            // normal         // TexCoords  // Tangent                          // Bitangent
            pos1.x, pos1.y, pos1.z, nm.x, nm.y, nm.z, uv1.x, uv1.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,
            pos2.x, pos2.y, pos2.z, nm.x, nm.y, nm.z, uv2.x, uv2.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,
            pos3.x, pos3.y, pos3.z, nm.x, nm.y, nm.z, uv3.x, uv3.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,

            pos1.x, pos1.y, pos1.z, nm.x, nm.y, nm.z, uv1.x, uv1.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z,
            pos3.x, pos3.y, pos3.z, nm.x, nm.y, nm.z, uv3.x, uv3.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z,
            pos4.x, pos4.y, pos4.z, nm.x, nm.y, nm.z, uv4.x, uv4.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z
        };
        // Setup plane VAO
        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 14 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(GLfloat), (GLvoid*)(8 * sizeof(GLfloat)));
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(GLfloat), (GLvoid*)(11 * sizeof(GLfloat)));
    }
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

bool running = true;

void TW_CALL RUN_cb(void* clientstate)
{
    running = false;
}

GLFWwindow* window = nullptr;
int main()
{
    float width = 800;
    float height = 600;

    glfwInit();
    window = glfwCreateWindow(width, height, "PBR", nullptr, nullptr);

    glEnable(GL_DEPTH_TEST);

    if(!window)
    {
        glfwTerminate();
        return 1;
    }

    glfwMakeContextCurrent(window);
    glewExperimental = true;
    glewInit();
    TwInit(TW_OPENGL, nullptr);
    TwWindowSize(width, height);

    float Roughness = 32, Metallic = 0.5;

    TwBar *myBar;
    myBar = TwNewBar("Main");
    TwAddVarRW(myBar, "Roughness", TW_TYPE_FLOAT, &Roughness,
               " label='Roughness' help='Color and transparency of the cube.' step = 0.5");
    TwAddVarRW(myBar, "Sampling", TW_TYPE_FLOAT, &samplingpattern,
               " label='Sampling' help='Color and transparency of the cube.' step = 0.25");
    TwAddVarRW(myBar, "Metallic", TW_TYPE_FLOAT, &Metallic,
               " label='Metallic' help='Color and transparency of the cube.' step = 0.001 ");
    TwAddButton(myBar, "Exit", RUN_cb, NULL, " label='Exit' ");

    glClearColor(0.5, 0.5, 0.5, 1.0);

    glfwSetMouseButtonCallback(window, (GLFWmousebuttonfun)TwEventMouseButtonGLFW3);
    glfwSetCursorPosCallback(window, (GLFWcursorposfun)TwEventMousePosGLFW3);
    glfwSetScrollCallback(window, (GLFWscrollfun)TwEventMouseWheelGLFW3);
    glfwSetKeyCallback(window, (GLFWkeyfun)TwEventKeyGLFW3);
    glfwSetCharCallback(window, (GLFWcharfun)TwEventCharGLFW3);

    GLuint shader = crearShader("vertex.glsl", "fragment.glsl");
    GLuint Skyshader = crearShader("Skyvertex.glsl", "Skyfragment.glsl");
    glDisable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glCullFace(GL_BACK);
    GLuint VAO, VBO, IBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &IBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube), &cube[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*) 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*) (sizeof(float) * 3));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*) (sizeof(float) * 6));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glBindVertexArray(0);

    GLuint skyVAO, skyVBO;
    glGenVertexArrays(1, &skyVAO);
    glGenBuffers(1, &skyVBO);

    glBindVertexArray(skyVAO);

    glBindBuffer(GL_ARRAY_BUFFER, skyVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*) 0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    vector<string> texs =
    {
        "textures/cubemap4/right.jpg",
        "textures/cubemap4/left.jpg",
        "textures/cubemap4/top.jpg",
        "textures/cubemap4/bottom.jpg",
        "textures/cubemap4/back.jpg",
        "textures/cubemap4/front.jpg"
    };

    GLuint CubeMap = loadCubemap(texs);
    GLuint normalMap = loadTexture("textures/normal2.bmp");
    GLuint AlbedoMap = loadTexture("textures/13119-diffuse.jpg");
    GLuint HeightMap = loadTexture("textures/depth2.bmp");
    int MaxVertexTextureImageUnits;
    glGetIntegerv(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS, &MaxVertexTextureImageUnits);
    cout << MaxVertexTextureImageUnits << endl;

    unsigned char pixels[2][2];
    pixels[0][0] = 0xFFFFFFFF;
    pixels[0][1] = 0;
    pixels[1][0] = 0;
    pixels[1][1] = 0xFFFFFFFF;

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glPixelStorei ( GL_UNPACK_ALIGNMENT,   1 );
    glPixelStorei ( GL_UNPACK_ROW_LENGTH,  0 );
    glPixelStorei ( GL_UNPACK_SKIP_ROWS,   0 );
    glPixelStorei ( GL_UNPACK_SKIP_PIXELS, 0 );
    glTexImage2D(GL_TEXTURE_2D, 0,GL_RED, 2, 2, 0, GL_RED, GL_UNSIGNED_BYTE, pixels);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glBindTexture(GL_TEXTURE_2D, 0);


    while(running)
    {
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        glfwPollEvents();
        Do_Movement();

        if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        }
        else
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }

        if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            running = false;

            if(glfwGetKey(window, GLFW_KEY_KP_ADD) == GLFW_PRESS)
            samplingpattern += 0.0005;

            if(glfwGetKey(window, GLFW_KEY_KP_SUBTRACT) == GLFW_PRESS)
            samplingpattern -= 0.0005;

        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

        glm::mat4 Projection = glm::perspective(90.0f, (float)640/480, 0.1f, 1000.0f);
        ///Draw Skybox
        //glDepthMask(GL_FALSE);
        glBindVertexArray(skyVAO);
        glUseProgram(Skyshader);
        glUniformMatrix4fv(glGetUniformLocation(Skyshader, "view"), 1, GL_FALSE, &camera.GetViewMatrix()[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(Skyshader, "projection"), 1, GL_FALSE, &Projection[0][0]);
        glActiveTexture(GL_TEXTURE0);
        glUniform1i(glGetUniformLocation(Skyshader, "skybox"), 0);
        glBindTexture(GL_TEXTURE_2D, CubeMap);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glUseProgram(0);
        glBindVertexArray(0);

        glUseProgram(shader);

        glm::mat4 model = glm::translate(glm::mat4(), glm::vec3(0.0, 0.0, -200.0));
        model = glm::scale(model, glm::vec3(10, 10, 10));
        //model = glm::rotate(model, 270.0f, glm::vec3(1.0, 0.0, 0.0));
        glm::mat4 MVP = Projection * camera.GetViewMatrix() * model;
        glUniformMatrix4fv(glGetUniformLocation(shader, "MVP"), 1, GL_FALSE, &MVP[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, &model[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(shader, "view"), 1, GL_FALSE, &camera.GetViewMatrix()[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, GL_FALSE, &Projection[0][0]);
        glUniform3f(glGetUniformLocation(shader, "viewPos"), camera.Position.x, camera.Position.y, camera.Position.z);
        glUniform1f(glGetUniformLocation(shader, "Roughness"), Roughness);
        glUniform1f(glGetUniformLocation(shader, "Metallic"), Metallic);
        glUniform1f(glGetUniformLocation(shader, "samplingpattern"), samplingpattern);
        glUniform1f(glGetUniformLocation(shader, "time"), glfwGetTime());

        glActiveTexture(GL_TEXTURE0);
        glUniform1i(glGetUniformLocation(shader, "cubeMap"), 0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, CubeMap);

        glActiveTexture(GL_TEXTURE1);
        glUniform1i(glGetUniformLocation(shader, "normalMap"), 1);
        glBindTexture(GL_TEXTURE_2D, normalMap);

        glActiveTexture(GL_TEXTURE2);
        glUniform1i(glGetUniformLocation(shader, "AlbedoMap"), 2);
        glBindTexture(GL_TEXTURE_2D, AlbedoMap);

        glActiveTexture(GL_TEXTURE3);
        glUniform1i(glGetUniformLocation(shader, "noise"), 3);
        glBindTexture(GL_TEXTURE_2D, textureID);

        glActiveTexture(GL_TEXTURE4);
        glUniform1i(glGetUniformLocation(shader, "Height"), 4);
        glBindTexture(GL_TEXTURE_2D, HeightMap);

        RenderQuad();

        glUseProgram(shader);
        model = glm::translate(glm::mat4(), glm::vec3(0.0, 5.0, 0.3));
        model = glm::scale(model, glm::vec3(10, 10, 10));
        MVP = Projection * camera.GetViewMatrix() * model;
        glUniformMatrix4fv(glGetUniformLocation(shader, "MVP"), 1, GL_FALSE, &MVP[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, &model[0][0]);
        glUniform3f(glGetUniformLocation(shader, "viewPos"), camera.Position.x, camera.Position.y, camera.Position.z);
        glUniform1f(glGetUniformLocation(shader, "Roughness"), Roughness);
        glUniform1f(glGetUniformLocation(shader, "Metallic"), 0.0);
        glUniform1f(glGetUniformLocation(shader, "samplingpattern"), samplingpattern);
        glUniform1f(glGetUniformLocation(shader, "time"), glfwGetTime());

        glActiveTexture(GL_TEXTURE0);
        glUniform1i(glGetUniformLocation(shader, "cubeMap"), 0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, CubeMap);

        glActiveTexture(GL_TEXTURE1);
        glUniform1i(glGetUniformLocation(shader, "normalMap"), 1);
        glBindTexture(GL_TEXTURE_2D, normalMap);

        glActiveTexture(GL_TEXTURE2);
        glUniform1i(glGetUniformLocation(shader, "AlbedoMap"), 2);
        glBindTexture(GL_TEXTURE_2D, AlbedoMap);

        glActiveTexture(GL_TEXTURE3);
        glUniform1i(glGetUniformLocation(shader, "noise"), 3);
        glBindTexture(GL_TEXTURE_2D, textureID);

        glActiveTexture(GL_TEXTURE4);
        glUniform1i(glGetUniformLocation(shader, "Height"), 4);
        glBindTexture(GL_TEXTURE_2D, normalMap);
        RenderQuad();
        glUseProgram(0);

        TwDraw();

        glfwSwapBuffers(window);
    }

    //glfwTerminate();
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

GLuint loadCubemap(vector<string> faces)
{
    GLuint textureID;
    glGenTextures(1, &textureID);
    glActiveTexture(GL_TEXTURE0);

    int width,height, channels;
    unsigned char* image;

    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
    glGenerateMipmap(GL_TEXTURE_2D);
    for(GLuint i = 0; i < faces.size(); i++)
    {
        image = SOIL_load_image(faces[i].c_str(), &width, &height, &channels, SOIL_LOAD_AUTO);
        glTexImage2D(
            GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0,
            GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image
        );
    }
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

    return textureID;
}

GLuint loadTexture(string path)
{
    GLuint textureID;
    glGenTextures(1, &textureID);

    int width,height;
    unsigned char* image;

    glBindTexture(GL_TEXTURE_2D, textureID);
    glGenerateMipmap(GL_TEXTURE_2D);

    image = SOIL_load_image(path.c_str(), &width, &height, 0, SOIL_LOAD_RGB);
    glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);

    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    return textureID;
}
