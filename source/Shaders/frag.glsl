#version 430

#define OBJ 256
#define UI 32

in vec2 uv;
out vec3 color;

uniform ivec2 rdims;
uniform ivec2 sdims;
uniform ivec2 ddims;
uniform vec3 colors[OBJ];

uniform vec4 rects[UI];
uniform vec4 uicols[UI];
uniform int nui;
uniform vec3 pos;

vec3 bg = vec3(0);

float maxDist = 15;

layout(rgba32f, binding = 0) uniform sampler2D tex;
layout(rgba32f, binding = 1) uniform sampler2D shadow;

vec4 fetchTexture(sampler2D tex, ivec2 dims, vec2 nuv, vec2 fetchDist, ivec2 fetches) {
	vec4 color = vec4(0);
	for (int x = 0; x < fetches.x; x++) {
		for (int y = 0; y < fetches.y; y++) {
			vec2 percent = vec2(float(x) / fetches.x, float(y) / fetches.y) * 2 - 1;
			vec2 uv = percent * fetchDist;
			color += texelFetch(tex, ivec2((nuv + uv) * dims), 0);
		}
	}
	return color / (fetches.x * fetches.y);
}

vec3 getColor(vec2 nuv) {
	vec4 data = texelFetch(tex, ivec2(nuv * rdims), 0);
	int id = int(data.w);
	vec3 color;
	if (id < 0) {
		return bg;
	} else {
		float d = clamp(length(data.xyz), 0, maxDist);
		//return colors[id] * texture(shadow, nuv);
		color = colors[id] * texelFetch(shadow, ivec2(nuv * sdims), 0).xyz;

		if (length(pos + data.xyz) > 50 && length(pos + data.xyz) < 50.5) {
			color = color*0.5 + vec3(0, 1, 1)*0.5;
		}

		return color*(1-d/maxDist) + (d/maxDist)*bg;
	}
}

bool inRect(vec4 rect, vec2 uv) {
	return (uv.x >= rect.x && uv.x <= rect.z && uv.y >= rect.y && uv.y <= rect.w);
}

vec3 drawUI(vec3 start, vec2 uv) {
	vec3 color = start;
	for (int i = 0; i < nui; i++) {
		if (rects[i].z - rects[i].x <= 0 || rects[i].w - rects[i].y <= 0) continue;
		if (inRect(rects[i], uv)) {
			float alpha = uicols[i].a;
			color = color*(1-alpha) + uicols[i].rgb*alpha;
		}
	}

	return color;
}

vec3 fetch(vec2 nuv, vec2 fetchDist, ivec2 fetches) {
	vec3 color = vec3(0);
	for (int x = 0; x < fetches.x; x++) {
		for (int y = 0; y < fetches.y; y++) {
			vec2 percent = vec2(float(x) / fetches.x, float(y) / fetches.y) * 2 - 1;
			vec2 uv = percent * fetchDist;
			color += getColor(nuv + uv);
		}
	}
	return color / (fetches.x * fetches.y);
}

void main(){
	vec2 nuv = uv / 2 + 0.5;
	color = drawUI(getColor(nuv), uv);
	//color = texture(shadow, nuv).xyz;
}