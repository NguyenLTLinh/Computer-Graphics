#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float time;
uniform float phase;

out VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoord;
    float ViewDistance;
    float BendMask;
} vs_out;

void main() {
    float heightMask = clamp(aTexCoord.y, 0.0, 1.0);
    float delayedWave = sin(time * 1.35 + phase + heightMask * 3.7);
    float secondaryRipple = sin(time * 2.1 + phase * 0.6 + heightMask * 7.5) * 0.35;
    float bend = (delayedWave + secondaryRipple) * heightMask * heightMask * 0.22;

    vec3 displaced = aPos;
    displaced.x += bend;
    displaced.z += sin(time * 0.75 + phase + heightMask * 2.4) * heightMask * 0.05;

    vec4 worldPosition = model * vec4(displaced, 1.0);
    vec4 viewPosition = view * worldPosition;

    vs_out.FragPos = worldPosition.xyz;
    vs_out.Normal = mat3(transpose(inverse(model))) * normalize(aNormal + vec3(-bend, 0.0, 0.0));
    vs_out.TexCoord = aTexCoord;
    vs_out.ViewDistance = length(viewPosition.xyz);
    vs_out.BendMask = heightMask;

    gl_Position = projection * viewPosition;
}
