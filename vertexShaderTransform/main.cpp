#include <iostream>
#define GLEW_STATIC

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SOIL.h>
#include <vector>
#include <glm/glm.hpp>

#include <shader.h>
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

Camera camera(glm::vec3(0,5,0));

struct vec3
{
    float x;
    float y;
    float z;
};

struct vec2
{
    float s;
    float t;
};

struct Vertex
{
    vec3 Position;
    vec2 TexCoords;
};

struct Face
{
    vec3 vertex0;
    vec3 vertex1;
    vec3 vertex2;
};

vector<Vertex> quadGrid;
vector<GLuint> indices;

const float HEIGHT = 128;

GLuint VAO, VBO, EBO;
GLuint shader;
GLuint texture, textured;

bool GenerateVertexBuffers()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    /** Vertex And UV buffer Attachments*/

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, quadGrid.size() * sizeof(Vertex), &quadGrid[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, TexCoords));

    /** Element buffer Attachments*/

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

    glBindVertexArray(0);
}

vector<GLfloat> quad;


void generateGrid()
{
    for(int i = 0 ; i < HEIGHT ; i++)
    {
        for(int j = 0 ; j < HEIGHT ; j++)
        {
            Vertex vert;

            vert.Position.x = (float)i - (HEIGHT);
            vert.Position.y = 0;
            vert.Position.z = (float)j - (HEIGHT);

            float fScaleC = float(j)/float(HEIGHT-1);
            float fScaleR = float(i)/float(HEIGHT-1);
            vert.TexCoords.s = HEIGHT*fScaleC/HEIGHT;
            vert.TexCoords.t = HEIGHT*fScaleR/HEIGHT;
            quadGrid.push_back(vert);

            //cout << "x: "<< vert.x << "y: "<< vert.y << "z: "<< vert.z << endl;

        }
    }

    int counter= 0;

    int totalVertices = HEIGHT * HEIGHT;
    cout << totalVertices << endl;
    for(int i = 0 ; i < totalVertices-HEIGHT+1 ; i++)
    {
        if(counter == HEIGHT-1)
        {
            counter = 0;
            continue;
        }

        Face face;

        face.vertex0 = quadGrid[i].Position;
        indices.push_back(i);
        face.vertex1 = quadGrid[i+1].Position;
        indices.push_back(i+1);
        face.vertex2 = quadGrid[i+HEIGHT+1].Position;
        indices.push_back(i+HEIGHT+1);

        face.vertex0 = quadGrid[i].Position;
        indices.push_back(i);
        face.vertex1 = quadGrid[i+HEIGHT+1].Position;
        indices.push_back(i+HEIGHT+1);
        face.vertex2 = quadGrid[i+HEIGHT].Position;
        indices.push_back(i+HEIGHT);

        counter++;



    }
}

void Render()
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glUniform1i(glGetUniformLocation(shader, "sampler"), 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, textured);
    glUniform1i(glGetUniformLocation(shader, "samplerd"), 1);

    glBindVertexArray(VAO);
    glDrawElements(GL_LINES, indices.size() - 5, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

int main()
{
    if(!glfwInit())
    {
        cout << "No se pudo iniciar GLFW" << endl;
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(1366, 768, "Ejemplo OpenGL", nullptr, nullptr);

    if(!window)
    {
        cout << "No se pudo abrir la ventana." << endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    glewExperimental = true;

    if(glewInit() != GLEW_OK)
    {
        cout << "No se pudo iniciar GLEW." << endl;
        glfwTerminate();
        return -1;
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glClearColor(0.0,1.0,1.0,1.0);
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    bool running = true;

    generateGrid();
    GenerateVertexBuffers();

    int width, height, channels;
    unsigned char* image = SOIL_load_image("water.png", &width, &height, &channels, SOIL_LOAD_RGB);
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


    unsigned char* image2 = SOIL_load_image("940-diffuse.jpg", &width, &height, &channels, SOIL_LOAD_RGB);
    glGenTextures(1, &textured);
    glBindTexture(GL_TEXTURE_2D, textured);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE,image2);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(image2);
    glBindTexture(GL_TEXTURE_2D, 0);

    shader = crearShader("vertex.glsl", "fragment.glsl");

    glm::mat4 projection = glm::perspective(90.0f, (float)800/(float)600, 0.1f, 10000.0f);

    glViewport(0,0,1366,768);
    glfwSwapInterval(1);

    float multiplier = 0.0;
    while(running)
    {
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        glfwSetWindowTitle(window, (string("fps: ") + to_string(1/deltaTime)).c_str());

        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        glfwPollEvents();
        Do_Movement();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            running = false;

        if(glfwGetKey(window, GLFW_KEY_KP_ADD) == GLFW_PRESS)
            multiplier += 0.005;

        if(glfwGetKey(window, GLFW_KEY_KP_SUBTRACT) == GLFW_PRESS)
            multiplier -= 0.005;
        glm::mat4 view;
        view = camera.GetViewMatrix();
        // Get the uniform locations
        GLint MVPID = glGetUniformLocation(shader, "MVP");
        // Pass the matrices to the shader

        glm::mat4 model = glm::mat4(1.0);

        glUseProgram(shader);
        glm::mat4 MVP = projection * view * model;
        glUniformMatrix4fv(MVPID, 1, GL_FALSE, &MVP[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, &model[0][0]);
        glUniform1f(glGetUniformLocation(shader, "time"), glfwGetTime());
        glUniform1f(glGetUniformLocation(shader, "multiplier"), multiplier);
        glUniform3f(glGetUniformLocation(shader, "viewPos"), camera.Position.x, camera.Position.y, camera.Position.z);

        Render();

        glfwSwapBuffers(window);
    }

    glfwTerminate();

    return 0;
}

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
