#shader vertex
#version 450 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 color;

out vec4 v_colors;

uniform mat4 u_MVP;
void main(){
   gl_Position = u_MVP * position;
   v_colors = color;
};


#shader fragment
#version 450 core

layout(location = 0) out vec4 color;

in vec4 v_colors;
uniform vec4 u_color;
uniform sampler2D u_Texture;

void main(){
	color = v_colors;
};