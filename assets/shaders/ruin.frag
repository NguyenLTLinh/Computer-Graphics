#version 330 core

struct Material {
    vec3 diffuseColor;
    vec3 specularColor;
    float shininess;
};

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
} fs_in;

uniform sampler2D diffuseMap;
uniform Material material;
uniform PointLight pointLight;
uniform vec3 viewPos;
uniform vec3 waterColor;
uniform float fogDensity;

out vec4 FragColor;

void main() {
    vec3 normal = normalize(fs_in.Normal);
    vec3 lightDir = normalize(pointLight.position - fs_in.FragPos);
    vec3 viewDir = normalize(viewPos - fs_in.FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);

    float distanceToLight = length(pointLight.position - fs_in.FragPos);
    float attenuation = 1.0 / (
        pointLight.constant +
        pointLight.linear * distanceToLight +
        pointLight.quadratic * distanceToLight * distanceToLight
    );

    vec3 albedo = texture(diffuseMap, fs_in.TexCoord * 2.0).rgb * material.diffuseColor;
    float diffuseStrength = max(dot(normal, lightDir), 0.0);
    float specularStrength = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    vec3 ambient = pointLight.ambient * albedo;
    vec3 diffuse = pointLight.diffuse * diffuseStrength * albedo;
    vec3 specular = pointLight.specular * specularStrength * material.specularColor;
    vec3 litColor = ambient + (diffuse + specular) * attenuation;

    float fogFactor = 1.0 - exp(-fogDensity * fogDensity * fs_in.ViewDistance * fs_in.ViewDistance);
    fogFactor = clamp(fogFactor, 0.0, 0.88);
    vec3 finalColor = mix(litColor, waterColor, fogFactor);

    FragColor = vec4(finalColor, 1.0);
}
