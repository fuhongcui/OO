#ifndef _SHADER_OBJ_H
#define _SHADER_OBJ_H
#include <string>

class ShaderProgram
{
public:
    ShaderProgram(const std::string& v, const std::string& f);
    ~ShaderProgram();
    ShaderProgram(const ShaderProgram& rObj) = delete;
    ShaderProgram& operator = (const ShaderProgram& rObj) = delete;
public:
    bool StartUse();
    void SetIntValue(const std::string& strName, int iValue);
    void SetMatrixValue(const std::string& strName, const float* fvalue);
private:
    bool Compile();
private:
    unsigned int m_ProgramID;
    std::string m_strVertex;
    std::string m_strFragment;
    bool m_bHaveInit;
};
#endif
