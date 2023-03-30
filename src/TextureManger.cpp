#include "TextureManger.h"

TextureManger::TextureManger()
{

}

TextureManger::~TextureManger()
{

}

std::shared_ptr<Texture> TextureManger::Create(const std::string& textureName, const TextureParam& param)
{
    TextureKey key;
    key.textureName = textureName;
    key.param = param;
    auto itrFind = m_texture.find(key);
    if(itrFind != m_texture.end())
    {
        return itrFind->second;
    }
    std::shared_ptr<Texture> texture = std::make_shared<Texture>();
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
