#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 FragPos;
out vec3 Normal;
out vec3 LightingColor; // resulting color from lighting calculations

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;



void main()
{
    //Model space pos
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = aNormal;  

    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    //diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.f);
    vec3 diffuse = diff * lightColor;

   
    //specular
    float specularStrength = 0.5;

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);

    float spec = pow(max(dot(viewDir, reflectDir), 0), 32);

    vec3 specular = specularStrength * spec * lightColor;    
    

    LightingColor = ambient + diffuse + specular;
    gl_Position = projection * view * vec4(FragPos, 1.0) ;
}

