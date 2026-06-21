#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float time;
uniform float phase;

out vec2 TexCoord;
out float AlphaPulse;

void main() {
    vec3 pos = aPos;
    pos.x += sin(time * 2.4 + phase + aTexCoord.y * 1.7) * 0.025;
    pos.y += sin(time * 3.0 + phase) * 0.018;

    TexCoord = aTexCoord;
    AlphaPulse = 0.65 + 0.25 * sin(time * 1.9 + phase);
    gl_Position = projection * view * model * vec4(pos, 1.0);
}
