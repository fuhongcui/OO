#include <cstring>
#include <cmath>
#include <vector>
#include <memory>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>

constexpr float window_width = 1200.f;
constexpr float window_heigth = 600.f;

constexpr float view_port_x = 0;
constexpr float view_port_y = 0;
constexpr float view_port_w = window_width;
constexpr float view_port_h = window_heigth;

bool InitWindow(GLFWwindow*& window);
bool InitShader(GLuint& shderProgram);
void Display(GLFWwindow* window, GLuint shderProgram);

glm::mat4 MakeModelMatrix();
glm::mat4 MakeViewMatrix();
glm::mat4 MakePerspectiveProjectMatrix(float fov, float n, float f);
glm::mat4 MakeOrthographicProjectMatrix(float l, float r, float b, float t, float n, float f);
glm::mat4 MakeViewPortMatrix(float x, float y, float w, float h);

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
    while(!glfwWindowShouldClose(window))
    {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        Display(window, shderProgram);;
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}

void Display(GLFWwindow *window, GLuint shderProgram)
{
    glUseProgram(shderProgram);

    glm::mat4 matrix_model = MakeModelMatrix();
    glm::mat4 matrix_view = MakeViewMatrix();
    glm::mat4 matrix_project = MakePerspectiveProjectMatrix(30.f, 0.1f, 1000.f);
    glm::mat4 matrix_view_port = MakeViewPortMatrix(view_port_x, view_port_y, view_port_w, view_port_h);
    //设置屏幕坐标系
    glUniformMatrix4fv(glGetUniformLocation(shderProgram, "matrix_model"), 1, GL_FALSE, glm::value_ptr(glm::mat4(1.f)));
    glUniformMatrix4fv(glGetUniformLocation(shderProgram, "matrix_view"), 1, GL_FALSE, glm::value_ptr(glm::mat4(1.f)));
    glm::mat4 matrix_screen = glm::ortho(0.f, 0.f + window_width, 0.f + window_heigth, 0.f, -1.f, 1.f);
    glUniformMatrix4fv(glGetUniformLocation(shderProgram, "matrix_project"), 1, GL_FALSE, glm::value_ptr(matrix_screen));
    //矩形框
    constexpr float width = 26.79492f - 2.f;
    glm::vec3 v0(-width, -width, 0.f);
    glm::vec3 v1( width, -width, 0.f);
    glm::vec3 v2( width,  width, 0.f);
    glm::vec3 v3(-width,  width, 0.f);
    std::vector<glm::vec3> vertex = {v0, v1, v2, v3};
    //计算屏幕坐标
    std::vector<glm::vec3> screenCoord;
    for(auto& v : vertex)
    {
        glm::vec4 clip = matrix_project * matrix_view * matrix_model * glm::vec4(v, 1.f);//裁剪坐标
        glm::vec3 ndc = glm::vec3(clip.x / clip.w, clip.y / clip.w, clip.y / clip.w);//设备坐标
        glm::vec3 screen = matrix_view_port * glm::vec4(ndc, 1.f);//屏幕坐标
        screenCoord.emplace_back(screen);
    }
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, screenCoord.size() * sizeof(decltype(screenCoord)::value_type), screenCoord.data(), GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);
    glDrawArrays(GL_LINE_LOOP, 0, screenCoord.size());
    glDisableVertexAttribArray(0);
    glDeleteBuffers(1, &vbo);
}

glm::mat4 MakeModelMatrix()
{
    glm::mat4 matrix = glm::mat4(1.f);
    matrix = /*glm::mat4(1.f);*/glm::rotate(matrix, -glm::radians(70.f), glm::vec3(1.f, 0.f, 0.f));
    return matrix;
}
glm::mat4 MakeViewMatrix()
{
    glm::mat4 matrix = glm::mat4(1.f);
    matrix = glm::translate(matrix, glm::vec3(0.f, 0.f, -100.f));
    return matrix;
}

glm::mat4 MakePerspectiveProjectMatrix(float fov, float n, float f)
{
    /*
    ---------------------------------------------------------------------------------------------------------
    |    2 * n / (r - l)    |     0                   |    (r + l) / (r - l)     |  0                       |
    ---------------------------------------------------------------------------------------------------------
    |    0                  |     2 * n / (t - b)     |    (t + b) / (t - b)     |  0                       |
    ---------------------------------------------------------------------------------------------------------
    |    0                  |     0                   |    -(f + n) / (f - n)    |  -2 * f * n / (f - n)    |
    ---------------------------------------------------------------------------------------------------------
    |    0                  |     0                   |    -1                    |  0                       |
    ---------------------------------------------------------------------------------------------------------
    */
    float tanHalfFovy = std::tan(glm::radians(fov / 2.f));
    float aspect = window_width / window_heigth;
    float t = n * tanHalfFovy;
    float b = -t;
    float r = t * aspect;
    float l = -r;
    glm::mat4 matrix = glm::mat4(0.f);
    matrix[0][0] = 2 * n / (r - l);
    matrix[1][1] = 2 * n / (t - b);
    matrix[2][0] = (r + l) / (r - l);
    matrix[2][1] = (t + b) / (t - b);
    matrix[2][2] = -(f + n) / (f - n);
    matrix[2][3] = -1;
    matrix[3][2] = -2 * f * n / (f - n);
    return matrix;
}

glm::mat4 MakeOrthographicProjectMatrix(float l, float r, float b, float t, float n, float f)
{
    /*
    ---------------------------------------------------------------------------------------------------------
    |    2 / (r - l)    |     0             |    0              |           -(r + l) / (r -l)               |
    ---------------------------------------------------------------------------------------------------------
    |    0              |     2 / (t - b)   |    0              |           -(t + b) / (t - b)              |
    ---------------------------------------------------------------------------------------------------------
    |    0              |     0             |    -2 / (f - n)   |           -(f + n) / (f - n)              |
    ---------------------------------------------------------------------------------------------------------
    |    0              |     0             |    0              |           1                               |
    ---------------------------------------------------------------------------------------------------------
    */
    glm::mat4 matrix = glm::mat4(0.f);
    return matrix;
}

glm::mat4 MakeViewPortMatrix(float x, float y, float w, float h)
{
    /*
    -----------------------------------------------------------------------------
    |    w / 2      |     0             |    0              |  w / 2 + x        |
    -----------------------------------------------------------------------------
    |    0          |     -h / 2         |    0             |  h / 2 + y        |
    -----------------------------------------------------------------------------
    |    0          |     0             |    1              |  0                |
    -----------------------------------------------------------------------------
    |    0          |     0             |    0              |  1                |
    -----------------------------------------------------------------------------
    */
    glm::mat4 matrix = glm::mat4(1.f);
    matrix[0][0] = window_width / 2;
    matrix[1][1] = -window_heigth / 2;
    matrix[3][0] = window_width / 2 + view_port_x;
    matrix[3][1] = window_heigth / 2 + view_port_y;
    return matrix;
}

bool InitWindow(GLFWwindow*& window)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    window = glfwCreateWindow(window_width, window_heigth, "HAHA", nullptr, nullptr);
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
    glViewport(view_port_x, view_port_y, window_width, window_heigth);
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
        "   color = vec4(93.0 / 255.0, 114.0 / 255.0, 138.0 / 255.0, 1.0);"
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
