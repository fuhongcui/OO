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

const string VERTEX_SHADER = R"(
        #version 330 core
        layout (location = 0) in vec3 aPos;
        layout (location = 1) in vec2 aTextureCoord;
        uniform mat4 mvp;
        out vec2 TexCoord;
        void main()
        {
           gl_Position = mvp * vec4(aPos.x, aPos.y, aPos.z, 1.0);
           TexCoord = aTextureCoord;
        }
)";
const string FRAGMENT_SHADER = R"(;
        #version 330 core
        out vec4 FragColor;
        uniform int UseTexture;
        in vec2 TexCoord;
        uniform sampler2D Texture;
        void main()
        {
            if(UseTexture >= 1)
            {
                FragColor = texture(Texture, TexCoord);
            }
            else
            {
                FragColor = vec4(1.0, 1.0, 0.0, 1.0);
            }
        }
)";

void WindowSizeCallback(GLFWwindow* window, int width, int height);
void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
void UserInput(GLFWwindow *window);
void DrawCenterLine(ShaderProgram& shader);


static const float WINDOW_WIDTH = 1280.f;
static const float WINDOW_HEIGHT = 640.f;
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

        //vertex
        PointCoord lt = {-100,  100, 0, 0, 1};
        PointCoord rt = {100,   100, 0, 1, 1};
        PointCoord rb = {100,   0, 0, 1, 0};
        PointCoord lb = {-100,  0, 0, 0, 0};
        vector<PointCoord> vertex;
        vertex.reserve(4);
        vertex.emplace_back(lt);
        vertex.emplace_back(rt);
        vertex.emplace_back(rb);
        vertex.emplace_back(lb);

//摄像机坐标到屏幕坐标
//==================================================================================================================================================================================
        auto ViewTOWindow_X_LRN = [](float view, float frustumLeft, float frustumRight, float frustumNear, float cameraDistance, float screenWidth) -> float
        {
            //摄像机 -> 近景面
            float near = frustumNear * view / cameraDistance;
            //近景面 -> 裁剪[-1, 1]
            float clip = ( 2.f * near / (frustumLeft - frustumRight) ) - ( (frustumLeft + frustumRight) / (frustumLeft - frustumRight) );
            //裁剪 -> 屏幕
            float window = (1.f - clip) / 2.f * screenWidth;
            return window;
        };
        auto ViewTOWindow_X_FOV = [](float view, float fov, float cameraDistance, float screenWidth, float aspectRatio) -> float
        {
            /*
             *
             * 摄像机 -> 近景面
             * near = frustumNear * view / cameraDistance
             * 已知 近景面 [frustumLeft, frustumRight] -> 裁剪 [-1, 1] 满足下面等式
             * clip = (2.f * near / (frustumLeft - frustumRight)) - ((frustumLeft + frustumRight) / (frustumLeft - frustumRight))
             * 因为 frustumLeftRight = frustumLeft = -frustumRight，带入上面等式，得到
             * clip = near / frustumLeftRight;
             * 将 near 带入上面的等式，得到
             * clip = (frustumNear / frustumLeftRight) * (view / cameraDistance)
             * 因为 frustumLeftRight = frustumBottomTop * aspectRatio，带入上面等式，得到
             * clip = (frustumNear / frustumBottomTop * aspectRatio) * (view / cameraDistance)
             * 因为 frustumNear / frustumBottomTop = 1 / tan(fov / 2.f)， 带入上面的等式， 得到
             * clip = view / (tan(fov / 2.f) * aspectRatio * cameraDistance)
            */
            float clip = view / (tan(glm::radians(fov / 2.f)) * aspectRatio * cameraDistance);
            float window = (1.f - clip) / 2.f * screenWidth;
            return window;
        };
        auto ViewTOWindow_Y_BTN = [](float view, float frustumBottum, float frustumTop, float frustumNear, float cameraDistance, float screenHeight) -> float
        {
            //摄像机 -> 近景面
            float near = frustumNear * view / cameraDistance;
            //近景面 -> 裁剪[-1, 1]
            float clip = (2.f * near / (frustumTop - frustumBottum)) - ((frustumTop + frustumBottum) / (frustumTop - frustumBottum));
            //裁剪 -> 屏幕
            float window = (1.f - clip) / 2.f * screenHeight;
            return window;
        };
        auto ViewTOWindow_Y_FOV = [](float view, float fov, float cameraDistance, float screenHeight) -> float
        {
            /*
             * 摄像机 -> 近景面
             * near = frustumNear * view / cameraDistance
             * 已知 近景面 [frustumBottum, frustumTop] -> 裁剪 [-1, 1] 满足下面等式
             * clip = (2.f * near / (frustumTop - frustumBottum)) - ((frustumTop + frustumBottum) / (frustumTop - frustumBottum))
             * 因为 frustumBottomTop = frustumTop = -frustumBottum，带入上面的等式，得到
             * clip = near / frustumBottomTop
             * 将 near 带入上面的等式，得到
             * clip = (frustumNear / frustumBottomTop) * (view / cameraDistance)
             * 因为 frustumNear / frustumBottomTop = 1 / tan(fov / 2.f)， 带入上面的等式， 得到
             * clip = view / (tan(fov / 2.f) * cameraDistance)
            */
            float clip = view / (tan(glm::radians(fov / 2.f)) * cameraDistance);
            float window = (1.f - clip) / 2.f * screenHeight;
            return window;
        };
        mvp = glm::ortho(0.f, WINDOW_WIDTH, WINDOW_HEIGHT, 0.f, -1.f, 1.f);//使用正交直接按照屏幕坐标描画
        //两种定义摄像机方式
#if 0
        //A => ①距离 ②视野开角
        float distance = 500.f;
        float fov = 30.f;
        //将摄像机坐标映射为屏幕坐标
        for(auto& v : vertex)
        {
            v.x = ViewTOWindow_X_FOV(v.x, fov, distance, WINDOW_WIDTH, WINDOW_WIDTH / WINDOW_HEIGHT);
            v.y = ViewTOWindow_Y_FOV(v.y, fov, distance, WINDOW_HEIGHT);
        }
#else
        //A => ①距离 ②近景面距离 ③近景面上 ④近景面下 ⑤近景面左 ⑥近景面右
        float distance = 500.f;
        float near = 0.1f;
        float left = -0.0535898395f;
        float right = 0.0535898395f;
        float bottom = -0.0267949197f;
        float top = 0.0267949197f;
        //将摄像机坐标映射为屏幕坐标
        for(auto& v : vertex)
        {
            v.x = ViewTOWindow_X_LRN(v.x, left, right, near, distance, WINDOW_WIDTH);
            v.y = ViewTOWindow_Y_BTN(v.y, bottom, top, near, distance, WINDOW_HEIGHT);
        }
#endif
//==================================================================================================================================================================================
        unsigned int VAO;
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);
        unsigned int VBOvertex;
        glGenBuffers(1, &VBOvertex);
        glBindBuffer(GL_ARRAY_BUFFER, VBOvertex);
        glBufferData(GL_ARRAY_BUFFER, vertex.size() * sizeof(vertex[0]), (void*)&vertex[0], GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        //render
        shader.SetIntValue("UseTexture", 1);
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
        glBindVertexArray(0);
        glDeleteBuffers(1, &VBOvertex);
        glDeleteVertexArrays(1, &VAO);
        DrawCenterLine(shader);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    //clean

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
//  if(FOV >= 1.0f && FOV <= 45.0f)
//    FOV -= yoffset;
//  if(FOV <= 1.0f)
//    FOV = 1.0f;
//  if(FOV >= 45.0f)
//    FOV = 45.0f;
}

void DrawCenterLine(ShaderProgram& shader)
{
    glm::mat4 mvp(1.0);
    mvp = glm::ortho(0.f, WINDOW_WIDTH, WINDOW_HEIGHT, 0.f, -1.f, 1.f);
    shader.StartUse();
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
