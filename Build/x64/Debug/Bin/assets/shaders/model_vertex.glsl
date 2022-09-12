#version 460 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_Texcoord;

out vec2 v_Texcoord;
out vec3 v_FragPos;
out vec3 v_Normal;

uniform mat4 View;
uniform mat4 Projection;
uniform mat4 Model;

void main()
{
   v_FragPos = vec3(Model * vec4(a_Position, 1.0));
   v_Normal = mat3(transpose(inverse(Model))) * a_Normal;
   v_Texcoord = a_Texcoord;
   
   gl_Position = Projection * View * vec4(v_FragPos, 1.0);
}