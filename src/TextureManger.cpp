#include "TextureManger.h"

#if defined(_WIN32)
#define SEPERATOR ("\\")
#else
#define SEPERATOR ("/")
#endif

TextureManger::TextureManger(const char* executePath)
    :m_exe_path(executePath)
{

}

TextureManger::~TextureManger()
{

}

std::shared_ptr<Texture> TextureManger::Get(const std::string& textureName, const TextureParam& param)
{
    TextureKey key;
    std::string texturePath = m_exe_path;
    auto dotPos = m_exe_path.find_last_of(SEPERATOR);
    if (dotPos != std::string::npos)
    {
        texturePath = m_exe_path.substr(0, dotPos + 1);
    }
    texturePath += "res";
    texturePath += SEPERATOR;
    std::string textureFilePath = texturePath + "1.jpg";
    
    key.textureName = textureFilePath;
    key.param = param;
    auto itrFind = m_texture.find(key);
    if(itrFind != m_texture.end())
    {
        return itrFind->second;
    }
    std::shared_ptr<Texture> texture(new Texture())/* = std::make_shared<Texture>()*/;
    if(!texture)
    {
        return nullptr;
    }
    if(!texture->Upload(key.textureName, key.param))
    {
        return nullptr;
    }
    m_texture.insert(std::make_pair(key, texture));
    return texture;
}
