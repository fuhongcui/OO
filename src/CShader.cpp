#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "CShader.h"
ShaderProgram::ShaderProgram(const std::string& v, const std::string& f)
:m_strVertex(v)
,m_strFragment(f)
,m_ProgramID(0)
{

}

ShaderProgram::~ShaderProgram()
{
    
}

const bool ShaderProgram::Compile()
{
     const char* vertex_src = m_strVertex.c_str();
    const char* fragment_src = m_strFragment.c_str();
    unsigned int vertexShader = 0;
    unsigned int fragmentShader = 0;
    m_ProgramID = 0;
    int  success = 0;
    char infoLog[512];
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertex_src, NULL);
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        return false;
    }

    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragment_src, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
     if(!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        return false;
    }
    m_ProgramID = glCreateProgram();
    glAttachShader(m_ProgramID, vertexShader);
    glAttachShader(m_ProgramID, fragmentShader);
    glLinkProgram(m_ProgramID);
    glGetProgramiv(m_ProgramID, GL_LINK_STATUS, &success);
    if(!success)
    {
        glGetProgramInfoLog(m_ProgramID, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER:::LINK_FAILED\n" << infoLog << std::endl;
        return false;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    return true;
}

 const bool ShaderProgram::StartUse()
 {
    if(!Compile())
    {
        return false;
    }
    glUseProgram(m_ProgramID);
    return true;
 }