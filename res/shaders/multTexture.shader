#shader vertex
#version 450 core
layout(location = 0) in vec2 position;
layout(location = 1) in vec4 color;
layout(location = 2) in vec2 texCoord;
layout(location = 3) in float texID;
out vec4 v_colors;
out vec2 v_TexCoord;
out float f_texID;
uniform mat4 u_MVP;
void main() {
	gl_Position = u_MVP * vec4(position, 0.0, 1.0);
	v_TexCoord = texCoord;
	v_colors = color;
	f_texID = texID;
};

#shader fragment
#version 450 core
layout(location = 0) out vec4 color;
in vec4 v_colors;
in vec2 v_TexCoord;
in float f_texID;
uniform vec4 u_color;
uniform sampler2D u_Textures[2];
void main() {
	int index = int(f_texID);
	color = texture(u_Textures[index],v_TexCoord);
};