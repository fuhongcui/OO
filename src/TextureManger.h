#ifndef TEXTUREMANGER_H
#define TEXTUREMANGER_H
#include <glad/glad.h>
#include <stb_image.h>
#include <string>
#include <map>
#include <memory>

struct TextureParam
{
    bool operator < (const TextureParam& param) const
    {
        return (TextureWrapS < param.TextureWrapS) ||
                ((TextureWrapS == param.TextureWrapS) && (TextureWrapT < param.TextureWrapT)) ||
                ((TextureWrapS == param.TextureWrapS) && (TextureWrapT == param.TextureWrapT) && (TextureMinFilter < param.TextureMinFilter)) ||
                 ((TextureWrapS == param.TextureWrapS) && (TextureWrapT == param.TextureWrapT) && (TextureMinFilter == param.TextureMinFilter) && (TextureMagFilter < param.TextureMagFilter));

    }
    int TextureWrapS {GL_REPEAT};
    int TextureWrapT {GL_REPEAT};
    int TextureMinFilter {GL_LINEAR};
    int TextureMagFilter {GL_LINEAR};
};

struct TextureKey
{
    bool operator < (const TextureKey& key) const
    {
        return (textureName < key.textureName) ||
                ((textureName == key.textureName) && (param < key.param));
    }
    std::string textureName;
    TextureParam param;
};


struct Texture
{
    Texture()
    {
        glGenTextures(1, &textureID);
    }
    ~Texture()
    {
        glDeleteTextures(1, &textureID);
    }
    Texture(const Texture& t) = delete;
    void Bind(unsigned int uiTextureUnit)
    {
        glActiveTexture(uiTextureUnit);
        glBindTexture(GL_TEXTURE_2D, textureID);
    }
    bool Upload(const std::string& texturePathName, const TextureParam& textureParam)
    {
        int width = 0;
        int height = 0;
        int nrChannels = 0;
        stbi_set_flip_vertically_on_load(true);
        unsigned char *dataTexture = stbi_load(texturePathName.c_str(), &width, &height, &nrChannels, 0);
        if(!dataTexture)
        {
            return false;
        }

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, textureParam.TextureWrapS);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, textureParam.TextureWrapT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, textureParam.TextureMinFilter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, textureParam.TextureMagFilter);

        glTexImage2D(GL_TEXTURE_2D, 0, nrChannels == 4 ? GL_RGBA : GL_RGB, width, height, 0, nrChannels == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, dataTexture);
        glGenerateMipmap(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);
        stbi_image_free(dataTexture);

        return true;
    }
    unsigned int textureID {0};
};

class TextureManger
{
public:
    TextureManger();
    ~TextureManger();
public:
    std::shared_ptr<Texture> Create(const std::string& textureName, const TextureParam& param);
private:
    std::map<TextureKey, std::shared_ptr<Texture>> m_texture;
};

#endif // TEXTUREMANGER_H
