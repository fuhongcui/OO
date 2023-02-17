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
"layout (location = 1) in vec2 aTextureCoord; \n"
"uniform mat4 modelMatrix; \n"
"uniform mat4 viewMatrix; \n"
"uniform mat4 projectMatrix; \n"
"out vec2 TexCoord;"
"void main() \n"
"{ \n"
"   gl_Position = projectMatrix * viewMatrix * modelMatrix * vec4(aPos.x, aPos.y, aPos.z, 1.0); \n"
"   TexCoord = aTextureCoord; \n"
"} \n"
;
const string FRAGMENT_SHADER =
"#version 330 core \n"
"out vec4 FragColor; \n"
"in vec2 TexCoord; \n"
"uniform sampler2D Texture; \n"
"void main() \n"
"{ \n"
"   FragColor = texture(Texture, TexCoord); \n"
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

    int width = 0;
    int height = 0;
    int nrChannels = 0;
    //Texture
    string texturePath = agrv[0];
    auto dotPos = texturePath.find_last_of(SEPERATOR);
    if (dotPos != string::npos)
    {
        texturePath = texturePath.substr(0, dotPos + 1);
    }
    texturePath += "res";
    texturePath += SEPERATOR;
    texturePath += "1.png";
    stbi_set_flip_vertically_on_load(true);
    unsigned char *dataTexture = stbi_load(texturePath.c_str(), &width, &height, &nrChannels, 0);
    if(!dataTexture)
    {
        return -1;
    }
    unsigned int Texture;
    glGenTextures(1, &Texture);
    glBindTexture(GL_TEXTURE_2D, Texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, nrChannels == 4 ? GL_RGBA : GL_RGB, width, height, 0, nrChannels == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, dataTexture);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(dataTexture);


    //VAO
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    //render loop
    while(!glfwWindowShouldClose(window))
    {
        UserInput(window);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);

        glm::mat4 modelMatrix(1.0);
        glm::mat4 viewMatrix(1.0);
        glm::mat4 projectMatrix(1.0);
#if 0
            //z
            glm::vec3 cameraPos = glm::vec3(0.f, -2.f, 2.f);
            glm::vec3 cameraTarget = glm::vec3(0.f, 0.f, 0.f);
            glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);
            //x
            glm::vec3 up = glm::vec3(0.f, 0.f, 1.f);
            glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));
            //y
            glm::vec3 cameraUP = glm::normalize(glm::cross(cameraDirection, cameraRight));

            viewMatrix = glm::lookAt(cameraPos, cameraTarget, up);
            projectMatrix = glm::perspective(glm::radians(fov), float(WINDOW_WIDTH / WINDOW_HEIGHT), 0.001f, 10000.f);
#endif

        float l = 0.f;
        float r = 0.f + WINDOW_WIDTH;
        float b = 0.f + WINDOW_HEIGHT;
        float t = 0.f;
        float n = -1.f;
        float f = 1.f;
//vertex
        float vvZ = 1.f;
        vector<PointCoord> vv =
        {
            {0.f, 0.f,          vvZ},
            {1280.f, 0.f,       vvZ},
            {1280.f, 640.f,     vvZ},
            {0.f, 640.f,        vvZ}
        };

        float v2Z = 0.5f;
        vector<PointCoord> vv2 =
        {
            {320.f, 160.f, v2Z},
            {960.f, 160.f, v2Z},
            {960.f, 480.f, v2Z},
            {320.f, 480.f, v2Z}
        };

        vector<TextureCoord> tt =
        {
            {0, 1},
            {1, 1},
            {1, 0},
            {0, 0}
        };
        //test
        auto Pxyz = [l, r, b, t, n, f](float x, float y, float z) -> PointCoord
        {
            PointCoord oPoint;
            float Sx = 2 / (r - l);
            float Sy = 2 / (t - b);
            float Sz = 2 / (n - f);

            float Tx = - (r + l) / (r - l);
            float Ty = - (t + b) / (t - b);
            float Tz = - (n + f) / (n - f);

            oPoint.x = Sx * x + Tx;
            oPoint.y = Sy * y + Ty;
            oPoint.z = Sz * z + Tz;
            return oPoint;
        };
        vector<PointCoord> vproject;
        for(auto& vertex : vv)
        {
            vproject.emplace_back(Pxyz(vertex.x, vertex.y, vertex.z));
        }
        vv.swap(vproject);
        vproject.clear();
        for(auto& vertex : vv2)
        {
            vproject.emplace_back(Pxyz(vertex.x, vertex.y, vertex.z));
        }
        vv2.swap(vproject);
//matrix
        //projectMatrix = glm::ortho(l, r, b, t, n, f);
        projectMatrix = glm::mat4(1.f);


//render
        shader.SetMatrixValue("modelMatrix", glm::value_ptr(modelMatrix));
        shader.SetMatrixValue("viewMatrix", glm::value_ptr(viewMatrix));
        shader.SetMatrixValue("projectMatrix", glm::value_ptr(projectMatrix));

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, Texture);

        unsigned int VBO_v;
        glGenBuffers(1, &VBO_v);
        glBindBuffer(GL_ARRAY_BUFFER, VBO_v);
        glBufferData(GL_ARRAY_BUFFER, vv.size() * sizeof(vv[0]), (void*)&vv[0], GL_DYNAMIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vv[0]), (void*)0);

        unsigned int VBO_t;
        glGenBuffers(1, &VBO_t);
        glBindBuffer(GL_ARRAY_BUFFER, VBO_t);
        glBufferData(GL_ARRAY_BUFFER, tt.size() * sizeof(tt[0]), (void*)&tt[0], GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(tt[0]), (void*)0);
        //glDrawArrays(GL_TRIANGLE_FAN, 0, vv.size());

        unsigned int VBO_v2;
        glGenBuffers(1, &VBO_v2);
        glBindBuffer(GL_ARRAY_BUFFER, VBO_v2);
        glBufferData(GL_ARRAY_BUFFER, vv2.size() * sizeof(vv2[0]), (void*)&vv2[0], GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vv2[0]), (void*)0);
        glDrawArrays(GL_TRIANGLE_FAN, 0, vv2.size());

        glBindTexture(GL_TEXTURE_2D, 0);
        glDeleteBuffers(1, &VBO_v);
        glDeleteBuffers(1, &VBO_t);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    //clean
    glDeleteVertexArrays(1, &VAO);
    glDeleteTextures(1, &Texture);

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

