#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>
#include "BasicShader.h"
#include "GLConv.h"
#include "TextureManger.h"

static const int WINDOW_WIDTH = 1280;
static const int WINDOW_HEIGHT = 640;

using namespace std;

void WindowSizeCallback(GLFWwindow* window, int width, int height);
void UserInput(GLFWwindow *window);
void DrawCenterLine(BasicShader& shader);


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

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    {
        glfwTerminate();
        return -1;
    }
    
    //shder
    BasicShader shader;
    //texture
    TextureManger textureManger(agrv[0]);
    auto texture = textureManger.Get("1.jpg");
    if(!texture)
    {
        glfwTerminate();
        return -1;
    }
    //摄像机坐标;
    vector<Point3DUV> vertex =
    {
        {-100, 100, -500, 0, 1},
        { 100, 100, -500, 1, 1},
        { 100, 0,   -500, 1, 0},
        {-100, 0,   -500, 0, 0}
    };

    //摄像机坐标到NDC
    //两种定义摄像机方式
#if 1
    float frustumNear = 0.1f;
    float frustumfar = 1000.f;
    float frustumfov = 30.f;
    float aspectRatio = static_cast<float>(WINDOW_WIDTH) / WINDOW_HEIGHT;
    //将摄像机坐标投影为NDC
    for(auto& v : vertex)
    {
        GLConv::Perspective(v.x, v.y, v.z, frustumfov, frustumNear, frustumfar, aspectRatio);
    }
#else
    float frustumnear = 0.1f;
    float frustumfar = 1000.f;
    float frustumleft = -0.0535898395f;
    float frustumright = 0.0535898395f;
    float frustumbottom = -0.0267949197f;
    float frustumtop = 0.0267949197f;
    //将摄像机坐标投影为NDC
    for(auto& v : vertex)
    {
        GLConv::Perspective(v.x, v.y, v.z, frustumleft, frustumright, frustumbottom, frustumtop, frustumnear, frustumfar);
    }
#endif
    
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO); 
    unsigned int VBOvertex;
    glGenBuffers(1, &VBOvertex);
    glBindBuffer(GL_ARRAY_BUFFER, VBOvertex);
    glBufferData(GL_ARRAY_BUFFER, vertex.size() * sizeof(vertex[0]), (void*)&vertex[0], GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex[0]), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vertex[0]), (void*)(3 * sizeof (float)));
    glDrawArrays(GL_TRIANGLE_FAN, 0, vertex.size());
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    //render loop
    while(!glfwWindowShouldClose(window))
    {
        UserInput(window);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);

        //matrix
        glm::mat4 mvp(1.0);//NDC坐标直接描画

        shader.Bind();
        shader.SetIntValue("UseTexture", 1);
        shader.SetMatrixValue("mvp", glm::value_ptr(mvp));;
        
        glBindVertexArray(VAO); 
        texture->Bind(GL_TEXTURE0);
        glDrawArrays(GL_TRIANGLE_FAN, 0, vertex.size());
        glBindVertexArray(0);
        
        DrawCenterLine(shader);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glDeleteBuffers(1, &VBOvertex);
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

void DrawCenterLine(BasicShader& shader)
{
    glm::mat4 mvp(1.0);
    mvp = glm::ortho(0.f, static_cast<float>(WINDOW_WIDTH), static_cast<float>(WINDOW_HEIGHT), 0.f, -1.f, 1.f);
    shader.Bind();
    shader.SetIntValue("UseTexture", 0);
    shader.SetMatrixValue("mvp", glm::value_ptr(mvp));

    //水平线
    float LineH[] =
    {
        0, WINDOW_HEIGHT / 2.f, 0,
        WINDOW_WIDTH, WINDOW_HEIGHT / 2.f, 0
    };
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    unsigned int VBOLineH;
    glGenBuffers(1, &VBOLineH);
    glBindBuffer(GL_ARRAY_BUFFER, VBOLineH);
    glBufferData(GL_ARRAY_BUFFER, sizeof(LineH), (void*)LineH, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof (float) * 3, 0);
    glDrawArrays(GL_LINES, 0, 2);
    //垂直线
    float LineV[] =
    {
        WINDOW_WIDTH / 2.f, 0, 0,
        WINDOW_WIDTH / 2.f, WINDOW_HEIGHT, 0
    };
    unsigned int VBOLineV;
    glGenBuffers(1, &VBOLineV);
    glBindBuffer(GL_ARRAY_BUFFER, VBOLineV);
    glBufferData(GL_ARRAY_BUFFER, sizeof(LineV), (void*)LineV, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof (float) * 3, 0);
    glDrawArrays(GL_LINES, 0, 2);


    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glDeleteBuffers(1, &VBOLineH);
    glDeleteBuffers(1, &VBOLineV);
    glDeleteVertexArrays(1, &VAO);
}
