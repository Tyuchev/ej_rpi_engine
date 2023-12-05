#define TILE_SIZE 32

struct VisibleIndex 
{
	int index;
};


layout(location=16) uniform sampler2D GlobalShadowMap;
layout(location=17) uniform mat4 GlobalShadowMatrix;
layout(location=18) uniform vec3 GlobalLightDirection;
layout(location=19) uniform vec3 GlobalLightColor;

// num tiles (work groups) in x,y for tiled forward binning
layout(location=20) uniform ivec2 NumTiles;


