#include "BasicShader.h"

const std::string VERTEX_SHADER = R"(
        #version 330 core
        layout (location = 0) in vec3 aPos;
        layout (location = 1) in vec2 aTextureCoord;
        uniform mat4 mvp;
        out vec2 TexCoord;
        void main()
        {
           gl_Position = mvp * vec4(aPos.x, aPos.y, aPos.z, 1.0);
           TexCoord = aTextureCoord;
        }
)";
const std::string FRAGMENT_SHADER = R"(;
        #version 330 core
        out vec4 FragColor;
        uniform int UseTexture;
        in vec2 TexCoord;
        uniform sampler2D Texture;
        void main()
        {
            if(UseTexture >= 1)
            {
                FragColor = texture(Texture, TexCoord);
            }
            else
            {
                FragColor = vec4(1.0, 1.0, 0.0, 1.0);
            }
        }
)";

BasicShader::BasicShader()
    :ShaderProgram("BasicShader", VERTEX_SHADER, FRAGMENT_SHADER)
{
    
}

BasicShader::~BasicShader()
{
    
}
