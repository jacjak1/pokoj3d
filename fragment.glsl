#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec2 TexCoord;
in vec3 Normal;

uniform sampler2D texSampler;

struct SpotLight {
    vec3 position;
    vec3 direction;
    vec3 color;
    vec3 specular;
    float cutOff;
    float outerCutOff;
    bool isOn;
};

uniform vec3 viewPos;
uniform vec3 ambientColor;
uniform SpotLight spotlights[2];

uniform bool useTexture;
uniform int fixtureIndex; // 0: lampa sufitowa, 1: lampa scienna

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    if (!light.isOn) { return vec3(0.0); }
    vec3 lightDir = normalize(light.position - fragPos);

    // Stozek reflektora
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    // Diffuse
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.color * diff;

    // Specular (Blinn-Phong)
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
    vec3 specular = light.specular * spec;

    return (diffuse + specular) * intensity;
}

void main()
{
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    
    if (!useTexture) // Obudowa lampy
    {
        if (spotlights[fixtureIndex].isOn) { 
            FragColor = vec4(1.0, 1.0, 0.9, 1.0); 
        } else { 
            FragColor = vec4(vec3(0.3) * ambientColor, 1.0);
        }
    }
    else 
    {
        // Ambient
        vec3 finalLight = ambientColor;

        // Sumowanie swiatla z reflektorow
        for (int i = 0; i < 2; i++) {
            finalLight += CalcSpotLight(spotlights[i], norm, FragPos, viewDir);
        }

        // Finalny kolor
        vec4 baseColor = texture(texSampler, TexCoord);
        vec3 result = baseColor.rgb * finalLight;
        FragColor = vec4(result, baseColor.a);
    }
}
