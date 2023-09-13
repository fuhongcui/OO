#include <cstring>
#include <memory>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>


void CheckShader(GLuint shader);
void CheckProgram(GLuint program);

int main(int argc, char* agrv[])
{
    //init
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    auto window = glfwCreateWindow(800, 600, "HAHA", nullptr, nullptr);
    if (window == nullptr)
    {
        glfwTerminate();
        return 0;
    }
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        glfwTerminate();
        return 0;
    }
    //shader
    const char* vss =
        "void main()"
        "{"
        "   gl_Position = vec4(0.0, 0.0, 0.0, 1.0);"
        "}";
    const char* fss =
        "void main()"
        "{"
        "   gl_FragColor = vec4(0.0, 0.0, 1.0, 1.0);"
        "}";
    auto vs = glCreateShader(GL_VERTEX_SHADER);
    auto fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(vs, 1, &vss, nullptr);
    glShaderSource(fs, 1, &fss, nullptr);
    glCompileShader(vs);
    CheckShader(vs);
    glCompileShader(fs);
    CheckShader(fs);
    auto program = glCreateProgram();
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    CheckProgram(program);
    //VAO
    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    while(!glfwWindowShouldClose(window))
    {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(program);
        glPointSize(30.f);
        glDrawArrays(GL_POINTS, 0, 1);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}

void CheckShader(GLuint shader)
{
    GLint size = 0;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &size);
    if(size > 0)
    {
        char* info = new char[size];
        std::memset(info, 0x0, size);
        GLsizei realSize = 0;
        glGetShaderInfoLog(shader, size, &realSize, info);
        std::cout << info << std::endl;
        delete [] info;
    }
}

void CheckProgram(GLuint program)
{
    GLint size = 0;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &size);
    if(size > 0)
    {
        char* info = new char[size];
        std::memset(info, 0x0, size);
        GLsizei realSize = 0;
        glGetProgramInfoLog(program, size, &realSize, info);
        std::cout << info << std::endl;
        delete [] info;
    }
}
