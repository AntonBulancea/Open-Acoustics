#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec4 Color;
  
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 color;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0f);
    Color = vec4(color, 1.0);
} 