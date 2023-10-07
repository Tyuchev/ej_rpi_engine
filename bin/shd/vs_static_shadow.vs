#version 310 es
 
precision highp float;
precision highp int;


layout(location=0) in vec3 in_Position;
layout(location=3) in vec2 in_TexCoord_0;

layout(location=3) out vec2 out_TexCoords;

uniform mat4 ViewProjection;
uniform mat4 Model;
uniform bool IsRoad;
uniform vec3 PlayerPosition;
uniform vec3 ObjectPosition;
uniform float RoadScale;

invariant gl_Position;

void main()
{
	out_TexCoords = in_TexCoord_0;
	// BUG: this must be calculated EXACTLY the same way as in our vs_static shader, otherwise, we get zbuffer fighting since the write to gl_Position is not invariant.
	// 	    check out https://stackoverflow.com/a/46920273
	vec4 wPos;
	if (!IsRoad)
	{
		wPos = (Model * vec4(in_Position, 1.0f));
	}
	else
	{
		const float curveDistance = 10.0f;
		const float curveStrength = 1.5f;
		float dist = ObjectPosition.z / RoadScale + in_Position.z - PlayerPosition.z / RoadScale;
		dist = clamp(dist, 0.0f, curveDistance - 0.5f);
		float newHeight = curveStrength * (log(curveDistance - dist) - log(curveDistance));
		wPos = (Model * vec4(in_Position + vec3(0.0f, newHeight, 0.0f), 1.0f));
	}
	gl_Position = ViewProjection * wPos;
}
