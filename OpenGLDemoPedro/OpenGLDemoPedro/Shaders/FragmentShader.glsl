#version 330 core

in vec3 Color;                     
in vec2 TexCoord;                  
in vec3 Normal_cameraspace;        
in vec3 vertexPosition_cameraspace;
in vec3 LightDirection_cameraspace;

out vec4 outColor;                

uniform sampler2D ourTexture;   
uniform vec3 LightColor;          
uniform float LightPower;         
uniform vec3 MaterialDiffuseColor;
uniform vec3 MaterialAmbientColor;
uniform vec3 MaterialSpecularColor;  
uniform vec3 EyePosition_cameraspace;

void main()
{
    vec4 texColor = texture(ourTexture, TexCoord);

    vec3 n = normalize(Normal_cameraspace);
    vec3 l = normalize(LightDirection_cameraspace);
    float cosTheta = max(dot(n, l), 0.0);
    float distance = length(LightDirection_cameraspace);
	vec3 diffuseColor = MaterialDiffuseColor * LightColor * LightPower * cosTheta / (distance * distance);

    vec3 E = normalize(EyePosition_cameraspace - vertexPosition_cameraspace);
    vec3 R = reflect(-l, n); // Vetor refletido
	float cosAlpha = clamp(dot(E, R), 0.0, 1.0);
    vec3 specularColor = MaterialSpecularColor * LightColor * LightPower * pow(cosAlpha, 5.0) / (distance * distance);

   outColor = vec4(MaterialAmbientColor, 1.0) + vec4(diffuseColor, 1.0) + vec4(specularColor, 1.0);

   outColor = outColor * texColor;
    outColor.a = 0.5;
}