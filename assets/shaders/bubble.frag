#version 330 core

in vec2 TexCoord;
in float AlphaPulse;

uniform vec3 waterColor;

out vec4 FragColor;

void main() {
    vec2 centered = TexCoord * 2.0 - 1.0;
    float r = length(centered);
    if (r > 1.0) {
        discard;
    }

    float rim = smoothstep(0.72, 0.98, r);
    float core = 1.0 - smoothstep(0.0, 0.95, r);
    vec3 color = mix(waterColor + vec3(0.08, 0.22, 0.22), vec3(0.76, 0.96, 0.92), rim);
    float alpha = (rim * 0.52 + core * 0.08) * AlphaPulse;

    FragColor = vec4(color, alpha);
}
