#version 430

#define OBJ 256
#define UI 64

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

float maxDist = 10;

layout(rgba32f, binding = 0) uniform sampler2D tex;
layout(rgba32f, binding = 1) uniform sampler2D sh;
/*
vec3 getShadow(vec2 nuv, vec2 fetchDist, ivec2 fetches) {
	vec3 shadow = vec3(0);
	float sum_inv_dist = 0;
	vec4 data = texelFetch(tex, ivec2(nuv * rdims), 0);
	if (data.w < 0) {
		return vec3(0);
	}
	vec3 mid = data.xyz;
	for (int x = 0; x < fetches.x; x++) {
		for (int y = 0; y < fetches.y; y++) {
			vec2 percent = vec2(float(x) / fetches.x, float(y) / fetches.y) * 2 - 1;
			vec2 uv = percent * fetchDist;
			vec3 pos = texture(tex, nuv, 0).xyz;
			float inv_dist = clamp(1 / pow(distance(mid, pos), 2), 0, 1);
			sum_inv_dist += inv_dist;
			vec4 data = texture(sh, nuv + uv);
			shadow += data.xyz * inv_dist;
		}
	}
	return shadow / sum_inv_dist;
}
*/
/*
vec3 getShadow(vec2 nuv, vec2 fetchDist, ivec2 fetches) {
	vec3 shadow = texture(sh, nuv).xyz;
	int nshadows = 1;
	vec4 data = texelFetch(tex, ivec2(nuv * rdims), 0);
	if (data.w < 0) {
		return vec3(0);
	}
	vec3 mid = data.xyz;
	for (float x = -fetches.x; x < fetches.x; x++) {
		for (float y = -fetches.y; y < fetches.y; y++) {
			if (x == 0 && y == 0) {
				continue;
			}
			vec2 percent = vec2(x / fetches.x, y / fetches.y) * 2 - 1;
			vec2 uv = percent * fetchDist;
			vec3 pos = texture(tex, nuv + uv, 0).xyz;
			if (distance(pos, mid) < maxShadowMixDist) {
				vec4 data = texture(sh, nuv + uv);
				shadow += data.xyz;
				nshadows += 1;
			}
		}
	}
	return shadow / nshadows;
}
*/
vec3 getShadow(vec2 nuv, vec2 offset, ivec2 fetches) {
	vec4 data = texelFetch(tex, ivec2(nuv * rdims), 0);
	if (data.w < 0) return vec3(0);

	vec3 shadow = texelFetch(sh, ivec2(nuv * sdims), 0).xyz;
	float sum_inv_dist = 0.1;
	
	int id = int(data.w);
	vec3 mid = data.xyz;
	for (int x = -fetches.x; x < fetches.x; x++) {
		for (int y = -fetches.y; y < fetches.y; y++) {
			if (x == 0 && y == 0) {
				continue;
			}
			vec2 samoff = vec2(x, y) * offset;
			vec4 samp = texelFetch(tex, ivec2((nuv + samoff) * rdims), 0);
			if (int(samp.w) == id) {
				vec2 sampuv = samoff + nuv;
				float inv_dist = 1 / distance(sampuv, nuv);
				vec4 data = texelFetch(sh, ivec2((nuv + samoff) * sdims), 0);
				shadow += data.xyz * inv_dist;
				sum_inv_dist += inv_dist;
			}
		}
	}
	return shadow / sum_inv_dist;
	//return float(nshadows) / (fetches.x*fetches.y);
}

vec3 getColor(vec2 nuv) {
	vec4 data = texelFetch(tex, ivec2(nuv * rdims), 0);
	int id = int(data.w);
	vec3 color;
	if (id < 0) {
		return bg;
	} else {
		float d = length(data.xyz);
		//return colors[id] * texture(shadow, nuv);
		color = colors[id] * getShadow(nuv, vec2(0.001, 0.0005), ivec2(1));

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
		if (inRect(rects[i], uv)) {
			float alpha = uicols[i].a;
			color = color*(1-alpha) + uicols[i].rgb*alpha;
		}
	}

	return color;
}

void main(){
	vec2 nuv = uv / 2 + 0.5;
	color = drawUI(getColor(nuv), uv);
	//color = texture(shadow, nuv).xyz;
	//color = getShadow(nuv, ivec2(6));
	//color = fetchTexture(shadow, sdims, nuv, vec2(0.0025), ivec2(10)).xyz;
}