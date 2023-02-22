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

    string texture2Path = textureBasePath + "2.png";
    unsigned char *dataTexture2 = stbi_load(texture2Path.c_str(), &width, &height, &nrChannels, 0);
    if(!dataTexture2)
    {
        return -1;
    }
    unsigned int texture2;
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, nrChannels == 4 ? GL_RGBA : GL_RGB, width, height, 0, nrChannels == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, dataTexture2);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(dataTexture2);



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
        glDepthFunc(GL_LESS);

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
        float n = 0.5f;
        float f = 1.f;
//vertex
        float vvZ = 0.79f;
        vector<PointCoord> vertex1 =
        {
            {0.f, 0.f,          vvZ},
            {1280.f, 0.f,       vvZ},
            {1280.f, 640.f,     vvZ},
            {0.f, 640.f,        vvZ}
        };

        float v2Z = 0.8f;
        vector<PointCoord> vertex2 =
        {
            {320.f, 160.f, v2Z},
            {960.f, 160.f, v2Z},
            {960.f, 480.f, v2Z},
            {320.f, 480.f, v2Z}
        };

        vector<TextureCoord> textureCoord =
        {
            {0, 1},
            {1, 1},
            {1, 0},
            {0, 0}
        };
//ortho
        auto Ortho = [l, r, b, t, n, f](float x, float y, float z) -> PointCoord
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
//perspective
        auto Perspective = [l, r, b, t, n, f](float Xe, float Ye, float Ze) -> PointCoord
        {
            PointCoord oPoint;

            float Xp = n * Xe / -Ze;
            float Yp = n * Ye / -Ze;

            //Xp -> Xn : l -> -1 r -> 1
            float Xn = (2 * Xp) / (r - l) - (r + l) / (r - l);
            //Yp -> Yn : b -> -1 t -> 1
            float Yn = (2 * Yp) / (t - b) - (t + b) / (t - b);

            float Zn = (- ((f + n) / (f - n) *  Ze) - (2 * f * n) / (f - n)) / -Ze;

            oPoint.x = Xn;
            oPoint.y = Yn;
            oPoint.z = Zn;
            return oPoint;
        };
        vector<PointCoord> vproject;
#if 1
//calculate test
        for(auto& vertex : vertex1)
        {
            vproject.emplace_back(Perspective(vertex.x, vertex.y, vertex.z));
        }
        vertex1.swap(vproject);
        vproject.clear();
        for(auto& vertex : vertex2)
        {
            vproject.emplace_back(Perspective(vertex.x, vertex.y, vertex.z));
        }
        vertex2.swap(vproject);
#else
//multiply matrix test
        glm::mat4 frustumMatrix = glm::frustum(l, r, b, t, n, f);
        for(auto & vertex : vertex1)
        {
            PointCoord p;
            glm::vec4 vp = frustumMatrix * glm::vec4(vertex.x, vertex.y, vertex.z, 1.f);
            p.x = vp.x / vp.w;
            p.y = vp.y / vp.w;
            p.z = vp.z / vp.w;
            vproject.emplace_back(p);
        }
        vertex1.swap(vproject);
        vproject.clear();
        for(auto& vertex : vertex2)
        {
            PointCoord p;
            glm::vec4 vp = frustumMatrix * glm::vec4(vertex.x, vertex.y, vertex.z, 1.f);
            p.x = vp.x / vp.w;
            p.y = vp.y / vp.w;
            p.z = vp.z / vp.w;
            vproject.emplace_back(p);
        }
        vertex2.swap(vproject);
#endif
//matrix
        //projectMatrix = glm::frustum(l, r, b, t, n, f);
        //projectMatrix = glm::ortho(l, r, b, t, n, f);



//render
        shader.SetMatrixValue("modelMatrix", glm::value_ptr(modelMatrix));
        shader.SetMatrixValue("viewMatrix", glm::value_ptr(viewMatrix));
        shader.SetMatrixValue("projectMatrix", glm::value_ptr(projectMatrix));

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);

        unsigned int VBOvertex1;
        glGenBuffers(1, &VBOvertex1);
        glBindBuffer(GL_ARRAY_BUFFER, VBOvertex1);
        glBufferData(GL_ARRAY_BUFFER, vertex1.size() * sizeof(vertex1[0]), (void*)&vertex1[0], GL_DYNAMIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex1[0]), (void*)0);

        unsigned int VBOtextureCoord;
        glGenBuffers(1, &VBOtextureCoord);
        glBindBuffer(GL_ARRAY_BUFFER, VBOtextureCoord);
        glBufferData(GL_ARRAY_BUFFER, textureCoord.size() * sizeof(textureCoord[0]), (void*)&textureCoord[0], GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(textureCoord[0]), (void*)0);
        glDrawArrays(GL_TRIANGLE_FAN, 0, vertex1.size());

        glBindTexture(GL_TEXTURE_2D, texture2);
        unsigned int VBOvertex2;
        glGenBuffers(1, &VBOvertex2);
        glBindBuffer(GL_ARRAY_BUFFER, VBOvertex2);
        glBufferData(GL_ARRAY_BUFFER, vertex2.size() * sizeof(vertex2[0]), (void*)&vertex2[0], GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex2[0]), (void*)0);
        glDrawArrays(GL_TRIANGLE_FAN, 0, vertex2.size());

        glBindTexture(GL_TEXTURE_2D, 0);
        glDeleteBuffers(1, &VBOtextureCoord);
        glDeleteBuffers(1, &VBOvertex1);
        glDeleteBuffers(1, &VBOvertex2);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    //clean
    glDeleteVertexArrays(1, &VAO);
    glDeleteTextures(1, &texture1);

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

