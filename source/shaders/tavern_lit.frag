#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 fragPos;
layout(location = 1) in vec3 fragNormal;
layout(location = 2) in vec2 fragUV;

layout(location = 0) out vec4 outColor;

layout(binding = 1, set = 1) uniform sampler2D albedoMap;

layout(binding = 0, set = 0) uniform GlobalUniformBufferObject {
    vec4 eyePos;
    vec4 ambientColor;
    vec4 directionalLightDir;
    vec4 directionalLightColor;
    vec4 pointLightPositions[6];
    vec4 pointLightColors[6];
    vec4 materialParams;
    ivec4 lightCounts;
} gubo;

vec3 applyLight(vec3 albedo, vec3 normal, vec3 viewDir, vec3 lightDir, vec3 lightColor, float attenuation) {
    float diffuseFactor = max(dot(normal, lightDir), 0.0);

    vec3 halfwayDir = normalize(viewDir + lightDir);
    float specularFactor = pow(max(dot(normal, halfwayDir), 0.0), gubo.materialParams.y);

    vec3 diffuse = albedo * diffuseFactor;
    vec3 specular = vec3(gubo.materialParams.x * specularFactor);

    return (diffuse + specular) * lightColor * attenuation;
}

vec3 applyPointLight(vec3 albedo, vec3 normal, vec3 viewDir, int index) {
    vec3 lightVector = gubo.pointLightPositions[index].xyz - fragPos;
    float distanceToLight = length(lightVector);
    vec3 lightDir = lightVector / max(distanceToLight, 0.001);

    float attenuation = 1.0 / (1.0 + 0.45 * distanceToLight + 0.22 * distanceToLight * distanceToLight);
    return applyLight(albedo, normal, viewDir, lightDir, gubo.pointLightColors[index].rgb, attenuation);
}

void main() {
    vec3 N = normalize(fragNormal);
    vec3 V = normalize(gubo.eyePos.xyz - fragPos);
    vec3 albedo = pow(texture(albedoMap, fragUV).rgb, vec3(2.2));

    vec3 color = albedo * gubo.ambientColor.rgb;

    vec3 sunDir = normalize(-gubo.directionalLightDir.xyz);
    color += applyLight(albedo, N, V, sunDir, gubo.directionalLightColor.rgb, 1.0);

    int pointLightCount = min(gubo.lightCounts.x, 6);
    for(int i = 0; i < pointLightCount; i++) {
        color += applyPointLight(albedo, N, V, i);
    }

    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0 / 2.2));

    outColor = vec4(color, 1.0);
}
