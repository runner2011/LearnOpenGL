#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 objectColor;
uniform vec3 viewPos;

uniform sampler2D texture_diffuse1;

void main()
{
    float ambientStrength = 0.3;
    vec3 ambient = ambientStrength * lightColor;

    float specularStrength = 0.9f;

    //diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.f);
    vec3 diffuse = diff * lightColor;

    //specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0), 64);

    vec3 specular = specularStrength * spec * lightColor;  

    vec4 tex = texture(texture_diffuse1, TexCoords);
    vec3 texRGB = vec3(tex.r, tex.g, tex.b);

    vec3 result = (ambient + diffuse + specular) * texRGB;
    
    FragColor =  vec4(result, 1.f);
}