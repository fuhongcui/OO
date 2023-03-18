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
"uniform mat4 mvp; \n"
"out vec2 TexCoord;"
"void main() \n"
"{ \n"
"   gl_Position = mvp * vec4(aPos.x, aPos.y, aPos.z, 1.0); \n"
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

static const unsigned int WINDOW_WIDTH = 1280;
static const unsigned int WINDOW_HEIGHT = 640;
static float FOV = 30.f;
struct Point2D
{
    Point2D() = default;
    Point2D(float x, float y)
        :x(x),y(y)
    {

    }
    float x {0.f};
    float y {0.f};
};

struct PointCoord
{
    PointCoord(float x, float y, float z, float u, float v)
        :x(x), y(y), z(z), u(u), v(v)
    {

    }
    float x {0.f};
    float y {0.f};
    float z {0.f};
    float u {0.f};
    float v {0.f};

};




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
//    glfwSetScrollCallback(window, ScrollCallback);

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
    unsigned char *dataTexture = stbi_load(texture1Path.c_str(), &width, &height, &nrChannels, 0);
    if(!dataTexture)
    {
        return -1;
    }
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
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
//matrix
        glm::mat4 mvp(1.0);
        glm::mat4 world(1.0);
        glm::mat4 frustum(1.0);

        float cameraDistance = 500.f;
        float surveyAngle = 0;
        float rFov = std::tan((FOV * 3.14159265 / 180.0) / 2.0);
        float aspectRatio = WINDOW_WIDTH / WINDOW_HEIGHT;
        Point2D screenCenterPos(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);


        float frustumNear = 100.f;
        float frustumFar = 1000.f;
        float frustumLeft = -frustumNear * rFov * aspectRatio;
        float frustumRight = frustumNear * rFov * aspectRatio;
        float frustumBottum = -frustumNear * rFov;
        float frustumTop = frustumNear * rFov;

        float EyeDistanceY = 0;
        float EyeDistanceZ = cameraDistance;

        frustum = glm::frustum(frustumLeft, frustumRight, frustumBottum, frustumTop, frustumNear, frustumFar);

        world = glm::translate(world, glm::vec3(0.f, EyeDistanceY, -EyeDistanceZ));
        world = glm::rotate(world, glm::radians(-surveyAngle), glm::vec3(1.f, 0.f, 0.f));

        mvp = frustum * world;

//算点东西
        //摄像机坐标 -> 屏幕坐标 输入：frustum上下左右 摄像机距离 屏幕高度
        auto Y_View_TO_Y_window = [](float Y_view, float frustumBottum, float frustumTop, float frustumNear, float cameraDistance, float screenHeight) -> float
        {
            //摄像机Y_view 投影到近景面 Y_near
            float Y_near = frustumNear * Y_view / cameraDistance;
            //近景面坐标Y_near映射到Y_clip[-1, 1]
            float Y_clip = ( 2.f * Y_near / (frustumTop - frustumBottum) ) - ( (frustumTop + frustumBottum) / (frustumTop - frustumBottum) );
            //裁剪坐标Y_clip映射到Y_windows
            float Y_window = (1.f - Y_clip) / 2.f * screenHeight;
            return Y_window;
        };
        //摄像机坐标 -> 屏幕坐标 输入：视野角度 摄像机距离 屏幕高度
        auto Y_View_TO_Y_window_2 = [](float Y_view, float fov, float cameraDistance, float screenHeight) -> float
        {
            float rFov = std::tan((fov * 3.14159265 / 180.0) / 2.0);
            float Y_clip = Y_view / (rFov * cameraDistance);
            float Y_window = (1.f - Y_clip) / 2.f * screenHeight;
            return Y_window;
        };
        //屏幕坐标 ->摄像机坐标 输入：frustum上下左右 摄像机距离 屏幕高度
        auto Y_window_TO_Y_View = [](float Y_window, float frustumBottum, float frustumTop, float frustumNear, float cameraDistance, float screenHeight) -> float
        {
            //屏幕坐标Y_windows映射到裁剪坐标Y_clip
            float Y_clip = 1.f - Y_window / screenHeight * 2.f;
            //裁剪坐标Y_clip[-1, 1]映射到近景面坐标Y_near
            float Y_near = ( Y_clip + ( (frustumTop + frustumBottum) / (frustumTop - frustumBottum) ) ) * (frustumTop - frustumBottum) / 2.f;
            //近景面坐标Y_near逆映射到摄像机Y_view
            float Y_view = Y_near * cameraDistance / frustumNear;
            return Y_view;
        };
        //屏幕坐标 ->摄像机坐标 输入：视野角度 摄像机距离 屏幕高度
        auto Y_window_TO_Y_View_2 = [](float Y_window, float fov, float cameraDistance, float screenHeight) -> float
        {
            float rFov = std::tan((fov * 3.14159265 / 180.0) / 2.0);
            float Y_clip = 1.f - (Y_window / screenHeight * 2.f);
            float Y_view = Y_clip * (cameraDistance * rFov);
            return Y_view;
        };
        auto ff = Y_View_TO_Y_window_2(100, 30, 500, 640);
//vertex
        PointCoord lt = {-100,  Y_window_TO_Y_View_2(0, FOV, cameraDistance, WINDOW_HEIGHT), 0, 0, 1};
        PointCoord rt = {100,   Y_window_TO_Y_View_2(0, FOV, cameraDistance, WINDOW_HEIGHT), 0, 1, 1};
        PointCoord rb = {100,   Y_window_TO_Y_View_2(640, FOV, cameraDistance, WINDOW_HEIGHT), 0, 1, 0};
        PointCoord lb = {-100,  Y_window_TO_Y_View_2(640, FOV, cameraDistance, WINDOW_HEIGHT), 0, 0, 0};

        vector<PointCoord> vertex;
        vertex.reserve(4);
        vertex.emplace_back(lb);
        vertex.emplace_back(rb);
        vertex.emplace_back(rt);
        vertex.emplace_back(lt);

        unsigned int VBOvertex;
        glGenBuffers(1, &VBOvertex);
        glBindBuffer(GL_ARRAY_BUFFER, VBOvertex);
        glBufferData(GL_ARRAY_BUFFER, vertex.size() * sizeof(vertex[0]), (void*)&vertex[0], GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
//render
        shader.SetIntValue("Texture", 0);
        shader.SetMatrixValue("mvp", glm::value_ptr(mvp));;

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);

        glBindBuffer(GL_ARRAY_BUFFER, VBOvertex);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex[0]), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vertex[0]), (void*)(3 * sizeof (float)));
        glBindTexture(GL_TEXTURE_2D, texture);
        glDrawArrays(GL_TRIANGLE_FAN, 0, vertex.size());
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDeleteBuffers(1, &VBOvertex);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    //clean
    glDeleteVertexArrays(1, &VAO);
    glDeleteTextures(1, &texture);

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
  if(FOV >= 1.0f && FOV <= 45.0f)
    FOV -= yoffset;
  if(FOV <= 1.0f)
    FOV = 1.0f;
  if(FOV >= 45.0f)
    FOV = 45.0f;
}

