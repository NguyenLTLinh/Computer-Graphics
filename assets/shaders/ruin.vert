#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoord;
    float ViewDistance;
} vs_out;

void main() {
    vec4 worldPosition = model * vec4(aPos, 1.0);
    vec4 viewPosition = view * worldPosition;

    vs_out.FragPos = worldPosition.xyz;
    vs_out.Normal = mat3(transpose(inverse(model))) * aNormal;
    vs_out.TexCoord = aTexCoord;
    vs_out.ViewDistance = length(viewPosition.xyz);

    gl_Position = projection * viewPosition;
}
