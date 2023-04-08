#ifndef SHADER_OBJ_H
#define SHADER_OBJ_H
#include <string>

class ShaderProgram
{
public:
    ShaderProgram(const std::string& name, const std::string& v, const std::string& f);
    virtual ~ShaderProgram();
    ShaderProgram(const ShaderProgram& rObj) = delete;
    ShaderProgram& operator = (const ShaderProgram& rObj) = delete;
public:
    void Bind();
    void SetIntValue(const std::string& strName, int iValue);
    void SetMatrixValue(const std::string& strName, const float* fvalue);
private:
    bool Compile();
private:
    unsigned int m_ProgramID { 0 };
    std::string m_strVertex { "" };
    std::string m_strFragment { "" };
    std::string m_strName {""};
};

#endif
