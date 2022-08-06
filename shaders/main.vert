#version 330

layout(location = 0) in vec3 v_position;
layout(location = 1) in vec2 v_texCoord;

out vec4 a_color;
out vec2 a_texCoord;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

void main() {
    a_color = vec4(0.6f, 0.6f, 0.6f, 1.0f);
    a_texCoord = v_texCoord;
    gl_Position = projection * view * model * vec4(v_position, 1.0f);
}
