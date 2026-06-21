#version 330 core

struct PointLight {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float constant;
    float linear;
    float quadratic;
};

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoord;
    float ViewDistance;
    float BendMask;
} fs_in;

uniform sampler2D diffuseMap;
uniform PointLight pointLight;
uniform vec3 viewPos;
uniform vec3 waterColor;
uniform float fogDensity;

out vec4 FragColor;

void main() {
    vec3 normal = normalize(fs_in.Normal);
    vec3 lightDir = normalize(pointLight.position - fs_in.FragPos);
    float distanceToLight = length(pointLight.position - fs_in.FragPos);
    float attenuation = 1.0 / (
        pointLight.constant +
        pointLight.linear * distanceToLight +
        pointLight.quadratic * distanceToLight * distanceToLight
    );

    vec3 base = texture(diffuseMap, fs_in.TexCoord).rgb;
    vec3 albedo = mix(base * vec3(0.05, 0.28, 0.16), base * vec3(0.12, 0.45, 0.24), fs_in.BendMask);
    float diffuseStrength = max(dot(normal, lightDir), 0.0);
    vec3 color = pointLight.ambient * albedo + pointLight.diffuse * diffuseStrength * albedo * attenuation;

    float fogFactor = clamp(1.0 - exp(-fogDensity * fs_in.ViewDistance), 0.0, 0.75);
    FragColor = vec4(mix(color, waterColor, fogFactor), 1.0);
}
