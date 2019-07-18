#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <iostream>
#include "CTrace.h"
#include "CShader.h"

using namespace std;
using namespace Namspace_Trace;

const string VERTEX_SHADER =
"#version 330 core \n"
"layout (location = 0) in vec3 aPos; \n"
"layout (location = 1) in vec2 aTextureCoord; \n"
"out vec2 TexCoord;"
"void main() \n"
"{ \n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0); \n"
"   TexCoord = aTextureCoord; \n"
"} \n"
; 
const string FRAGMENT_SHADER =
"#version 330 core \n"
"out vec4 FragColor; \n"
"in vec2 TexCoord; \n"
"uniform sampler2D TextureA; \n"
"uniform sampler2D TextureB; \n"
"void main() \n"
"{ \n"
"   FragColor = mix(texture(TextureA, TexCoord), texture(TextureB, TexCoord), 0.2); \n"
"} \n" 
;

void WindowSizeCallback(GLFWwindow* window, int width, int height);
void UserInput(GLFWwindow *window);

const unsigned int WINDOW_WIDTH = 800;
const unsigned int WINDOW_HEIGHT = 600;

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "", NULL, NULL);
    if (window == NULL)
    {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, WindowSizeCallback);

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
    MyShaderProgram.SetIntValue("TextureA", 0);
    MyShaderProgram.SetIntValue("TextureB", 1);
    float vertices[] = 
    {
        -0.5f, -0.5f,0.0f,	0.0f,0.0f,
        0.5f, -0.5f,0.0f,	1.0f,0.0f,
        0.5f, 0.5f,0.0f,	1.0f,1.0f,
        -0.5f, 0.5f,0.0f,	0.0f,1.0f
    };
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    unsigned int indices[] = 
    {
        0, 1, 2,
        0, 2, 3
    };
    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    int width = 0;
    int height = 0;
    int nrChannels = 0;
    //TextureA
    const char* strTextureA = "./res/res.jpg";
    stbi_set_flip_vertically_on_load(true);
    unsigned char *dataTextureA = stbi_load(strTextureA, &width, &height, &nrChannels, 0);
    if(!dataTextureA)
    {
        TraceLevel(LOG_ERROR, "Image load  failed, src=%s", strTextureA);
        return -1;
    }
    unsigned int TextureA;
    glGenTextures(1, &TextureA);
    glBindTexture(GL_TEXTURE_2D, TextureA);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    if(nrChannels == 3)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, dataTextureA);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else if(nrChannels == 4)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, dataTextureA);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
         TraceLevel(LOG_ERROR, "Image bit = %d", nrChannels);
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(dataTextureA);
    //TextureB
    const char* strTextureB = "./res/res.png";
    stbi_set_flip_vertically_on_load(true);
    unsigned char *dataTextureB = stbi_load(strTextureB, &width, &height, &nrChannels, 0);
    if(!dataTextureB)
    {
        TraceLevel(LOG_ERROR, "Image load  failed, src=%s", dataTextureB);
        return -1;
    }
    unsigned int TextureB;
    glGenTextures(1, &TextureB);
    glBindTexture(GL_TEXTURE_2D, TextureB);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    if(nrChannels == 3)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, dataTextureB);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else if(nrChannels == 4)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, dataTextureB);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
         TraceLevel(LOG_ERROR, "Image bit = %d", nrChannels);
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(dataTextureB);

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, TextureA);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, TextureB);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glActiveTexture(GL_TEXTURE0);

    glBindVertexArray(0);

    while (!glfwWindowShouldClose(window))
    {
	    UserInput(window);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteTextures(1, &TextureA);

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


   
