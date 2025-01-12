#version 310 es
#extension GL_GOOGLE_include_directive : enable

precision highp float;
precision highp int;

#include "shd/lights.glsl"

layout(location=0) in vec3 in_WorldSpacePos;
layout(location=1) in vec3 in_Normal;
layout(location=3) in vec2 in_TexCoords;

layout(location=0) out vec4 out_Color;

layout(location=0) uniform sampler2D BaseColorTexture;
layout(location=1) uniform sampler2D NormalTexture;
// GLTF 2.0 spec: metallicRoughness = Its green channel contains roughness values and its blue channel contains metalness values.
layout(location=2) uniform sampler2D MetallicRoughnessTexture;
layout(location=3) uniform sampler2D EmissiveTexture;
layout(location=4) uniform sampler2D OcclusionTexture;

layout(location=5) uniform vec4 BaseColorFactor;
layout(location=6) uniform vec4 EmissiveFactor;
layout(location=7) uniform float MetallicFactor;
layout(location=8) uniform float RoughnessFactor;
layout(location=9) uniform float AlphaCutoff;

layout(location=10) uniform vec4 CameraPosition;


const float FOG_DENSITY = 0.015f;
const vec4 FOG_COLOR = vec4(0.0f, 0.0f, 0.0f, 1.0f);


// V = view vector, N = surface normal, P = fragment point in world space
vec3 CalculateGlobalLight(vec3 V, vec3 N, vec3 P, vec4 diffuseColor)
{
    float diffuse = max(dot(GlobalLightDirection, N), 0.0f);
    vec4 shadowCoords = GlobalShadowMatrix * vec4(P, 1.0f);
    shadowCoords.xyzw /= shadowCoords.w;
    shadowCoords = shadowCoords * 0.5f + 0.5f;
    float shadowDepth = texture(GlobalShadowMap,shadowCoords.xy).r;
    float geoDepth = shadowCoords.z;
    // bias based on incidence angle
    float bias = max(0.0003f * (1.0f - diffuse), 0.00035f);  

    // simple PCF with 3x3 kernel for now
    float shadow = 0.0f;
    vec2 texelSize = 1.0f / vec2(textureSize(GlobalShadowMap, 0));
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float d = texture(GlobalShadowMap, shadowCoords.xy + vec2(float(x), float(y)) * texelSize).r; 
            shadow += geoDepth - bias > d ? 1.0f : 0.0f;
        }    
    }
    shadow /= 9.0f;

    float shadowFactor = 1.0f - shadow;
    return shadowFactor * (GlobalLightColor * diffuse * diffuseColor.rgb);
}

const vec3 AmbientLight = vec3(0.15f);

float getFog()
{
    float fogDist = length(in_WorldSpacePos - CameraPosition.xyz);
    float fogAmount = 1.0f - exp2(-FOG_DENSITY * FOG_DENSITY * fogDist * fogDist * 1.442695);
    return fogAmount;
}

void main()
{
    vec3 modPos = in_WorldSpacePos;

    vec4 baseColor = texture(BaseColorTexture, in_TexCoords).rgba * BaseColorFactor;
    baseColor = pow(baseColor, vec4(1.0f/2.2f));
    vec2 metallicRoughness = texture(MetallicRoughnessTexture, in_TexCoords).xy;
	vec3 emissive = texture(EmissiveTexture, in_TexCoords).xyz;
	vec3 occlusion = texture(OcclusionTexture, in_TexCoords).xyz;
    
    vec3 V = normalize(CameraPosition.xyz - modPos.xyz);
    vec3 N = in_Normal;

    vec3 light = AmbientLight;

    light += CalculateGlobalLight(V, N, modPos, baseColor);

    float fogAmount = getFog();

    vec4 col = vec4(light.rgb * baseColor.rgb + emissive, 1.0f);
    out_Color = mix(col, FOG_COLOR, fogAmount);
}