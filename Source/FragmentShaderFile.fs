#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec3 Pos;

void main()
{
    // Here, ourColor maybe have more than ** 50000 ** colors!! This is the MAGIC position. Because fragments are much more than vertex. The vertexes' 
    // output value will be interpolated by fragments. For example, there's three ourColor value, fragment interpolate ourColor by distance to the three vertex positions.
    FragColor =  vec4(ourColor, 1.0f);
}