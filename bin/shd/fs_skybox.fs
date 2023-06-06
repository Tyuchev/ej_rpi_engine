#version 310 es
 
precision highp float;
precision highp int;

layout(location=0) in vec3 in_SampleDir;

layout(location=0) uniform samplerCube SkyboxTexture;

out vec4 out_Color;

void main()
{
	vec4 sky_Color = vec4(0.44f, 0.65f, 1.0f, 1.0f);
	vec4 ground_Color = vec4(0.81f, 0.66f, 0.0f, 1.0f);

	if (dot(vec3(0.0f, -1.0f, 0.0f), in_SampleDir) > 0.0f) {
		out_Color = ground_Color;
	}
	else {
		out_Color = sky_Color;
	}

	//out_Color = vec4(texture(SkyboxTexture, in_SampleDir).rgb, 1.0f);
}
