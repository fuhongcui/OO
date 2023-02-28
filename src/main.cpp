#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>
#include "Shader.h"

#if defined(_WIN32)
#define SEPERATOR ("\\")
#else
#define SEPERATOR ("/")
#endif

using namespace std;

const string VERTEX_SHADER =
"#version 330 core \n"
"layout (location = 0) in vec3 aPos; \n"
"uniform mat4 modelMatrix; \n"
"uniform mat4 viewMatrix; \n"
"uniform mat4 projectMatrix; \n"
"void main() \n"
"{ \n"
"   gl_Position = projectMatrix * viewMatrix * modelMatrix * vec4(aPos.x, aPos.y, aPos.z, 1.0); \n"
"} \n"
;
const string FRAGMENT_SHADER =
"#version 330 core \n"
"out vec4 FragColor; \n"
"void main() \n"
"{ \n"
"   FragColor = vec4(0.0, 1.0, 1.0, 1.0); \n"
"} \n"
;

void WindowSizeCallback(GLFWwindow* window, int width, int height);
void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
void UserInput(GLFWwindow *window);



const unsigned int WINDOW_WIDTH = 1280;
const unsigned int WINDOW_HEIGHT = 640;

struct PointCoord
{
    PointCoord()
        :x(0.f), y(0.f), z(0.f)
    {

    }
    PointCoord(float x, float y, float z)
        :x(x), y(y), z(z)
    {

    }
    float x {0.f};
    float y {0.f};
    float z {0.f};

};
struct TextureCoord
{
    TextureCoord()
        :u(0.f), v(0.f)
    {

    }
    TextureCoord(float u, float v)
        :u(u), v(v)
    {

    }
    float u {0.f};
    float v {0.f};
};


static float fov = 45.f;

bool GetPos(PointCoord& currentPos);

int main(int argc, char* agrv[])
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "", nullptr, nullptr);
    if (window == nullptr)
    {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, WindowSizeCallback);
    glfwSetScrollCallback(window, ScrollCallback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        glfwTerminate();
        return -1;
    }
    ShaderProgram shader(VERTEX_SHADER, FRAGMENT_SHADER);
    if(!shader.StartUse())
    {
        glfwTerminate();
        return -1;
    }
    shader.SetIntValue("Texture", 0);
#if 0
    //Texture
    int width = 0;
    int height = 0;
    int nrChannels = 0;
   
    string textureBasePath = agrv[0];
    auto dotPos = textureBasePath.find_last_of(SEPERATOR);
    if (dotPos != string::npos)
    {
        textureBasePath = textureBasePath.substr(0, dotPos + 1);
    }
    textureBasePath += "res";
    textureBasePath += SEPERATOR;


    string texture1Path = textureBasePath + "1.jpg";
    stbi_set_flip_vertically_on_load(true);
    unsigned char *dataTexture1 = stbi_load(texture1Path.c_str(), &width, &height, &nrChannels, 0);
    if(!dataTexture1)
    {
        return -1;
    }
    unsigned int texture1;
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, nrChannels == 4 ? GL_RGBA : GL_RGB, width, height, 0, nrChannels == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, dataTexture1);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(dataTexture1);
#endif
    //VAO
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    vector<PointCoord> vertex;
    //render loop
    while(!glfwWindowShouldClose(window))
    {
        UserInput(window);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 modelMatrix(1.0);
        glm::mat4 viewMatrix(1.0);
        glm::mat4 projectMatrix(1.0);

        float l = 0.f;
        float r = 0.f + WINDOW_WIDTH;
        float b = 0.f + WINDOW_HEIGHT;
        float t = 0.f;
        float n = -1.f;
        float f = 1.f;
       
        PointCoord pos;
        bool bSuccess = GetPos(pos);
        if (bSuccess)
        {
            vertex.emplace_back(pos);
        }
        if (!vertex.empty())
        {
//matrix
            //projectMatrix = glm::frustum(l, r, b, t, n, f);
            projectMatrix = glm::ortho(l, r, b, t, n, f);
            //projectMatrix = glm::mat4(1.f);


//render
            shader.SetMatrixValue("modelMatrix", glm::value_ptr(modelMatrix));
            shader.SetMatrixValue("viewMatrix", glm::value_ptr(viewMatrix));
            shader.SetMatrixValue("projectMatrix", glm::value_ptr(projectMatrix));

            unsigned int VBO;
            glGenBuffers(1, &VBO);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, vertex.size() * sizeof(vertex[0]), (void*)&vertex[0], GL_DYNAMIC_DRAW);

            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex[0]), (void*)0);

            //glPolygonMode(GL_FRONT, GL_POINT);
            glPointSize(4.f);
            glDrawArrays(GL_POINTS, 0, vertex.size());

            glBindTexture(GL_TEXTURE_2D, 0);
            glDeleteBuffers(1, &VBO);
            glfwSwapBuffers(window);;
        }
        glfwPollEvents();
    }
    //clean
    glDeleteVertexArrays(1, &VAO);

    glfwTerminate();
    return 0;
}


void UserInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }

}

void WindowSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
  if(fov >= 1.0f && fov <= 45.0f)
    fov -= yoffset;
  if(fov <= 1.0f)
    fov = 1.0f;
  if(fov >= 45.0f)
    fov = 45.0f;
}

bool GetPos(PointCoord& currentPos)
{
    static double lastTime = glfwGetTime();
    static bool lastInit = false;
    static PointCoord lastPos = {0.f, 0.f, 0.f};
    static bool bStart = false;
    static double StartTime = 0.0;
    if (!bStart)
    {
        bStart = true;
        StartTime = glfwGetTime();
    }
    
    double CurrentTime = glfwGetTime();
    if((CurrentTime - lastTime) < 0.06)
    {
        return false;
    }
    lastTime = CurrentTime;


    auto t = CurrentTime - StartTime;


    PointCoord StartPos = { 0.f, 320.f, 0.f };
    float fSpeed = 50.f;
    PointCoord v0 = { 0.f, 320.f, 0.f };
    PointCoord v1 = { 10.f, 320.f, 0.f };
    float distance = sqrt(pow((v1.x - v0.x), 2) + pow((v1.y - v0.y), 2));

    float cosTheta = (v1.x  - v0.x) / distance;
    float sinTheta = (v1.y - v0.y) / distance;



    float a = -1.f;


    currentPos.x = StartPos.x + fSpeed * cosTheta * t + 0.5 * a * cosTheta * pow(t, 2);
    currentPos.y = StartPos.y + fSpeed * sinTheta * t + 0.5 * a * sinTheta * pow(t, 2);

    if (lastInit)
    {
        float moveDistance = sqrt(pow((currentPos.x - lastPos.x), 2) + pow((currentPos.y - lastPos.y), 2));
        if (moveDistance < 1.f)
        {
            return false;
        }
    }
    lastInit = true;
    lastPos = currentPos;

    return true;
}