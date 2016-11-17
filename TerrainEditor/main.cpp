#include <iostream>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <shader.h>
#include "camera.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <EpsilonModel.h>

void Do_Movement();

glm::vec3 RayPick(float);
void mouse_callback();
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;
float mouseScrollY = 10;
void getScroll(GLFWwindow* window, double x, double y);
glm::vec3 FindPointInTerrain(glm::vec3 RayCast);
GLFWwindow* window;
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

Camera camera(glm::vec3(0,0,0));

using namespace std;

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

typedef vector<Vertex> VTerrain;
typedef vector<unsigned int> IndexList;

struct Terrain
{
    VTerrain Terrain;
    IndexList Indices;
};

bool running = true;

Terrain generateTerrain(size_t SIZE)
{

    VTerrain l_Terrain;
    IndexList indices;
    for(int i = 0 ; i < SIZE ; i++)
    {
        for(int j = 0 ; j < SIZE ; j++)
        {
            Vertex vert;

            vert.Position.x = (float)i;
            vert.Position.y = 0;
            vert.Position.z = (float)j;

            float fScaleC = float(j)/float(SIZE-1);
            float fScaleR = float(i)/float(SIZE-1);
            vert.TexCoords.s = SIZE*fScaleC/SIZE;
            vert.TexCoords.t = SIZE*fScaleR/SIZE;
            l_Terrain.push_back(vert);

        }
    }

    int mod = 1;
    int counter = 0;

    for(int i = 0 ; i < (SIZE * SIZE) - SIZE ; i++)
    {

        if(counter == SIZE - 1)
        {
            counter = 0;
            mod++;
            continue;
        }

        //triangle 1
        indices.push_back(i+1);//cout << i+1 << ", ";

        indices.push_back(i+SIZE); //cout << i+SIZE << ", ";

        indices.push_back(i+SIZE-SIZE); //cout << i+SIZE-SIZE << endl;

        //triangle 2
        indices.push_back(i+1); //cout << i+1 << ", ";

        indices.push_back(i+SIZE+1); //cout << i+SIZE+1 << ", ";

        indices.push_back(i+SIZE+1-1); //cout << i+SIZE+1-1 << endl;

        counter++;

    }

    Terrain rTerr;
    rTerr.Indices = indices;
    rTerr.Terrain = l_Terrain;

    return rTerr;

}

class TerrainObject
{
public:

    TerrainObject(Terrain terrain)
    {
        m_terrain = terrain;
        createVAO();
    }

    virtual ~TerrainObject() {}

public:

    void Draw(GLuint shader)
    {
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, m_terrain.Indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

private:

    void createVAO()
    {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        /** Vertex And UV buffer Attachments*/

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, m_terrain.Terrain.size() * sizeof(Vertex), &m_terrain.Terrain[0], GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, TexCoords));

        /** Element buffer Attachments*/

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_terrain.Indices.size() * sizeof(GLuint), &m_terrain.Indices[0], GL_STATIC_DRAW);

        glBindVertexArray(0);
    }


private:
    GLuint VAO;
    GLuint VBO;
    GLuint EBO;
    Terrain m_terrain;

};
const float width = 640, height = 480;

glm::mat4 projection = glm::perspective(90.0f, width/height, 0.1f, 1000.0f);


int main()
{

    float terrainSize = 32;
float posx = terrainSize/2, posy = terrainSize/2;
    glfwInit();
    glfwWindowHint(GLFW_SAMPLES, 8);
    window = glfwCreateWindow(width, height, "Terrain Editor", nullptr, nullptr);

    glfwMakeContextCurrent(window);

    glewExperimental = true;
    glewInit();

    eModel m = EpsilonModel::ImportModel("cube.eml");
    std::vector<EpsilonModel::float3> vertices = m.getVertices();
    std::vector<EpsilonModel::uint> indices = m.getIndices();

    glfwSetWindowPos(window, width/2, height/2);

    glEnable(GL_DEPTH_TEST);
    glClearColor(.5, .5, .5, 1.0);

    Terrain terrainData = generateTerrain(terrainSize);

    TerrainObject terrain(terrainData);
    glfwSetScrollCallback(window, (GLFWscrollfun)getScroll);

    GLuint shader = crearShader("vertex.glsl", "fragment.glsl");
    glViewport(0,0,width,height);

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glLineWidth(1.5);

    vector<glm::vec3> points = {glm::vec3(1,1,0), glm::vec3(1.0, -1.0, 0.0)};

    GLuint VAO, VBO, EBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), &points[0], GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (GLvoid*)0);
    glBindVertexArray(0);

    glm::mat4 model = glm::mat4();
    while(running)
    {
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        glfwPollEvents();
        Do_Movement();

                //cout << "Mouse picking: " << RayPick(mouseScrollY).x << ", " << RayPick(mouseScrollY).y << ", " << RayPick(mouseScrollY).z << endl;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            running = false;

        if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        else
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

        glUseProgram(shader);
        glm::mat4 MVP = projection * camera.GetViewMatrix();

        glUniformMatrix4fv(glGetUniformLocation(shader, "MVP"), 1, GL_FALSE, &MVP[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, &model[0][0]);
        glUniform1f(glGetUniformLocation(shader, "time"), glfwGetTime());
        glUniform1i(glGetUniformLocation(shader, "ispoint"), false);
        glUniform1f(glGetUniformLocation(shader, "deltaTime"), deltaTime);

        glUniform1f(glGetUniformLocation(shader, "lastposx"), posx);
        glUniform1f(glGetUniformLocation(shader, "lastposy"), posy);
        if(glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
            posx += 1;
        if(glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
            posx -= 1;

        if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
            posy += 1;
        if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
            posy -= 1;
        glUniform1f(glGetUniformLocation(shader, "posx"), posx);
        glUniform1f(glGetUniformLocation(shader, "posy"), posy);


        glLineWidth(3);
        terrain.Draw(shader);
        cout << "Mouse picking: " << RayPick(mouseScrollY).x << ", " << RayPick(mouseScrollY).y << ", " << RayPick(mouseScrollY).z << endl;


        glBindVertexArray(VAO);
        if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
        {
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            points = {camera.Position, RayPick(mouseScrollY)};
            glBufferSubData(GL_ARRAY_BUFFER, 0,sizeof(points), &points[0]);
            //model = glm::translate(glm::mat4(), RayPick(mouseScrollY));
            //cout << "Mouse picking: " << RayPick(mouseScrollY).x << ", " << RayPick(mouseScrollY).y << ", " << RayPick(mouseScrollY).z << endl;
        }

        //cout << points.size() << endl;

        MVP = projection * camera.GetViewMatrix() * model;
        glUniformMatrix4fv(glGetUniformLocation(shader, "MVP"), 1, GL_FALSE, &MVP[0][0]);
        glUniform1i(glGetUniformLocation(shader, "ispoint"), true);

        glDrawArrays(GL_LINES, 0, 6);
        glBindVertexArray(0);
        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);

    glfwTerminate();

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

glm::vec3 RayPick(float lenght)
{
    double mouse_x, mouse_y;

    glfwGetCursorPos(window, &mouse_x, &mouse_y);

    float x = (2.0f * mouse_x) / width - 1.0f;
    float y = 1.0f - (2.0f * mouse_y) / height;
    float z = 1.0f;
    glm::vec3 ray_nds = glm::vec3 (x, y, z);

    glm::vec4 ray_clip = glm::vec4 (ray_nds.x, ray_nds.y, -1.0, 1.0);

    glm::vec4 ray_eye = glm::inverse (projection) * ray_clip;
    ray_eye = glm::vec4 (ray_eye.x, ray_eye.y, -1.0, 0.0);

    glm::vec3 ray_wor = glm::vec3( glm::inverse(camera.GetViewMatrix()) * ray_eye);
    // don't forget to normalise the vector at some point
    return glm::normalize (ray_wor) * lenght + camera.Position;
}

void getScroll(GLFWwindow* window, double x, double y)
{
    mouseScrollY += 0.5 * y;
}

glm::vec3 FindPointInTerrain(glm::vec3 RayCast)
{

}
