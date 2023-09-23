#include <cstring>
#include <memory>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>


bool InitWindow(GLFWwindow*& window);
bool InitShader(GLuint& shderProgram);
void MakeRenderData(GLuint& dataVBO);
void Display(GLFWwindow* window, GLuint shderProgram, GLuint dataVBO);
int main(int argc, char* agrv[])
{
    GLFWwindow* window = nullptr;
    if(!InitWindow(window))
    {
        return 0;
    }
    GLuint shderProgram = 0;
    if(!InitShader(shderProgram))
    {
        return 0;
    }
    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    GLuint dataVBO = 0;
    MakeRenderData(dataVBO);
    while(!glfwWindowShouldClose(window))
    {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        Display(window, shderProgram, dataVBO);;
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}

void Display(GLFWwindow *window, GLuint shderProgram, GLuint dataVBO)
{
    glUseProgram(shderProgram);
    glm::mat4 matrix_model(1.f);
    matrix_model = glm::translate(matrix_model, glm::vec3(0.f, -2.f, 0.f));
    glUniformMatrix4fv(glGetUniformLocation(shderProgram, "matrix_model"), 1, GL_FALSE, glm::value_ptr(matrix_model));

    glm::mat4 matrix_view(1.f);
    matrix_view = glm::translate(matrix_view, glm::vec3(0.f, 0.f, -8.f));
    glUniformMatrix4fv(glGetUniformLocation(shderProgram, "matrix_view"), 1, GL_FALSE, glm::value_ptr(matrix_view));

    glm::mat4 matrix_project(1.f);
    int windowWidth = 0;
    int windowheight = 0;
    glfwGetFramebufferSize(window, &windowWidth, &windowheight);
    float aspect = (float)windowWidth / (float)windowheight;
    matrix_project = glm::perspective(glm::radians(60.f), aspect, 0.1f, 1000.0f);
    glUniformMatrix4fv(glGetUniformLocation(shderProgram, "matrix_project"), 1, GL_FALSE, glm::value_ptr(matrix_project));

    glBindBuffer(GL_ARRAY_BUFFER, dataVBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void MakeRenderData(GLuint& dataVBO)
{
    float vertexPositions[108] = {
        -1.0f, 1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f,
        1.0f, -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, -1.0f,
        1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f,
        1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, 1.0f,
        -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,
        -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, -1.0f
    };
    glGenBuffers(1, &dataVBO);
    glBindBuffer(GL_ARRAY_BUFFER, dataVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), vertexPositions, GL_STATIC_DRAW);
}


bool InitWindow(GLFWwindow*& window)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    window = glfwCreateWindow(600, 300, "HAHA", nullptr, nullptr);
    if (window == nullptr)
    {
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        glfwTerminate();
        return false;
    }
    return true;
}

bool InitShader(GLuint& shderProgram)
{
    const char* vss =
        "#version 430\n"
        "layout (location = 0) in vec3 position;"
        "uniform mat4 matrix_model;"
        "uniform mat4 matrix_view;"
        "uniform mat4 matrix_project;"
        "void main()"
        "{"
        "   gl_Position = matrix_project * matrix_view * matrix_model * vec4(position, 1.0);"
        "}";
    const char* fss =
        "#version 430\n"
        "out vec4 color;"
        "void main()"
        "{"
        "   color = vec4(0.0, 0.0, 1.0, 1.0);"
        "}";
    auto CheckShader = [](GLuint shader)
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
            return false;
        }
        return true;
    };
    auto CheckProgram = [](GLuint program)
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
            return false;
        }
        return true;
    };
    auto vs = glCreateShader(GL_VERTEX_SHADER);
    auto fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(vs, 1, &vss, nullptr);
    glShaderSource(fs, 1, &fss, nullptr);
    glCompileShader(vs);
    if(!CheckShader(vs))
    {
        std::cout << "vs error !!!!!!!!" << std::endl;
        return false;
    }
    glCompileShader(fs);
    if(!CheckShader(fs))
    {
        std::cout << "fs error !!!!!!!!" << std::endl;
        return false;
    }
    shderProgram = glCreateProgram();
    glAttachShader(shderProgram, vs);
    glAttachShader(shderProgram, fs);
    glLinkProgram(shderProgram);
    if(!CheckProgram(shderProgram))
    {
        return false;
    }
    return true;
}
