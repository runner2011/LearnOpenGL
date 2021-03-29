#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;

uniform float mix_tex;

void main()
{
    //FragColor =  mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2);
    FragColor =  texture(texture1, TexCoord) * mix_tex + texture(texture2, TexCoord) * (1.0 - mix_tex);
}