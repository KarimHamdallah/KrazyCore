#version 460 core

out vec4 o_color;
uniform vec3 u_LightColor;
void main()
{
   o_color = vec4(u_LightColor, 1.0);
}