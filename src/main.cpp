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
    float x {0.f};
    float y {0.f};
    float z {0.f};
};
struct TextureCoord
{
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
    ShaderProgram MyShaderProgram(VERTEX_SHADER, FRAGMENT_SHADER);
    if(!MyShaderProgram.StartUse())
    {
        glfwTerminate();
        return -1;
    }
    MyShaderProgram.SetIntValue("Texture", 0);

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


    //render
    while(!glfwWindowShouldClose(window))
    {
        UserInput(window);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 modelMatrix(1.0);
//        modelMatrix = glm::rotate(modelMatrix, glm::radians((float)glfwGetTime() * 60.f), glm::vec3(0.f, 0.f, 1.f));
//        modelMatrix = glm::scale(modelMatrix, glm::vec3(2.f, 2.f, 2.f));

        glm::mat4 viewMatrix(1.0);
        glm::mat4 projectMatrix(1.0);

//        //z
//        glm::vec3 cameraPos = glm::vec3(0.f, -2.f, 2.f);
//        glm::vec3 cameraTarget = glm::vec3(0.f, 0.f, 0.f);
//        glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);
//        //x
//        glm::vec3 up = glm::vec3(0.f, 0.f, 1.f);
//        glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));
//        //y
//        glm::vec3 cameraUP = glm::normalize(glm::cross(cameraDirection, cameraRight));

//        viewMatrix = glm::lookAt(cameraPos, cameraTarget, up);


//        projectMatrix = glm::perspective(glm::radians(fov), float(WINDOW_WIDTH / WINDOW_HEIGHT), 0.001f, 10000.f);

//        //test vertex
//        {
//            glm::vec4 testVertex = glm::vec4(0.f, 0.f, 0.f, 1.f);
//            glm::mat4 viewMatrixTest(1.0);
//            //move camera to origin
//            viewMatrixTest = glm::translate(viewMatrixTest, glm::normalize(glm::vec3(0.f, 0.f, 0.f) - cameraPos));
//        }

        {
//            viewMatrix = glm::translate(viewMatrix, glm::vec3(0.f, 0.f, -200.f));
//            projectMatrix = glm::perspective(glm::radians(30.f), float(WINDOW_WIDTH / WINDOW_HEIGHT), 100.f, 200.f);
            //projectMatrix = glm::frustum(-640.f, 640.f, -320.f, 320.f,100.f, 200.f);
            //screen coordinate

            //test
            float l = 0.f;
            float r = 0.f + WINDOW_WIDTH;
            float b = 0.f + WINDOW_HEIGHT;
            float t = 0.f;
            float n = 0.f;
            float f = 1.f;
            projectMatrix = glm::ortho(l, r, b, t, n, f);
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


            PointCoord LB = Pxyz(0.f, 0.f, -0.5f);
            PointCoord RB = Pxyz(1280.f, 0.f, 0.f);
            PointCoord RT = Pxyz(1280, 640.f, 0.f);
            PointCoord LT = Pxyz(0, 640.f, 0.f);
            //glm::vec4 PxyzO = projectMatrix * glm::vec4(0.f, 640.f, -1.f, 1.f);

            int iCui = 0;
            ++iCui;
        }
        MyShaderProgram.SetMatrixValue("modelMatrix", glm::value_ptr(modelMatrix));
        MyShaderProgram.SetMatrixValue("viewMatrix", glm::value_ptr(viewMatrix));
        MyShaderProgram.SetMatrixValue("projectMatrix", glm::value_ptr(projectMatrix));


        {

            vector<PointCoord> vv;
            vector<TextureCoord> vt;
            PointCoord v;
            TextureCoord t;
            v.x = 0.f;
            v.y = 0.f;
            v.z = -0.5f;

            t.u = 0.f;
            t.v = 1.f;
            vv.emplace_back(v);
            vt.emplace_back(t);

            v.x = 1280.f;
            v.y = 0.f;
            v.z = -0.5f;

            t.u = 1.f;
            t.v = 1.f;
            vv.emplace_back(v);
            vt.emplace_back(t);

            v.x = 1280.f;
            v.y = 640.f;
            v.z = -0.5f;

            t.u = 1.f;
            t.v = 0.f;
            vv.emplace_back(v);
            vt.emplace_back(t);

            v.x = 0.f;
            v.y = 640.f;
            v.z = -0.5f;

            t.u = 0.f;
            t.v = 0.f;
            vv.emplace_back(v);
            vt.emplace_back(t);

            //glEnable(GL_DEPTH_TEST);
            //glDepthFunc(GL_GREATER);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, Texture);


            unsigned int buffers[2];
            glGenBuffers(2, buffers);
            glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
            glBufferData(GL_ARRAY_BUFFER, vv.size() * sizeof (vv[0]), (void*)&vv[0], GL_DYNAMIC_DRAW);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vv.at(0)), (void*)0);

            glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
            glBufferData(GL_ARRAY_BUFFER, vt.size() * sizeof (vt[0]), (void*)&vt[0], GL_DYNAMIC_DRAW);
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vt.at(0)), (void*)0);

            glDrawArrays(GL_TRIANGLE_FAN, 0, vv.size());

            modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f, 0.5f, 0.5f));
            MyShaderProgram.SetMatrixValue("modelMatrix", glm::value_ptr(modelMatrix));
            glDrawArrays(GL_TRIANGLE_FAN, 0, vv.size());


            glBindTexture(GL_TEXTURE_2D, 0);
            glDeleteBuffers(2, buffers);
        }
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

