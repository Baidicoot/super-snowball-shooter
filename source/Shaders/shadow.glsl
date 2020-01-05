#version 430

#define OBJ 256
#define LIGHT 32

layout(local_size_x = 1, local_size_y = 1) in;
layout(rgba32f, binding = 0) uniform sampler2D depth;
layout(rgba32f, binding = 1) uniform image2D out_img;

uniform vec4[OBJ] objs;
uniform int nobj;

uniform vec3[LIGHT] lights;
uniform vec4[LIGHT] lcols;
uniform int nlight;

uniform ivec2 rdims;
uniform ivec2 sdims;
uniform vec3 pos;

struct Ray {
	vec3 o;
	vec3 d;
};

struct RayResult {
	bool intersected;
	float t1;
	float t2;
};

struct Intersection {
	bool exists;
	int id;
	float d;
};

vec3 getNormal(vec4 s, vec3 surf) {
	return normalize(surf - s.xyz);
}

vec4 getData(vec2 nuv) {
	return texelFetch(depth, ivec2(nuv * rdims), 0);
}

RayResult intersectSphere(Ray r, vec4 s) {
	float t = dot(s.xyz - r.o, r.d);
	if (t < 0) return RayResult(false, 0, 0);
	float y = distance(r.o + t*r.d, s.xyz);
	if (y >= s.w) return RayResult(false, 0, 0);
	float x = sqrt(s.w*s.w - y*y);
	return RayResult(true, t - x, t + x);
}

bool castRay(Ray r, vec4[OBJ] s, float maxDist) {
	for (int i = 0; i < nobj; i++) {
		if (s[i].w <= 0) continue;
		RayResult r = intersectSphere(r, s[i]);
		if (r.intersected && r.t1 < maxDist) {
			return true;
		}
	}

	return false;
}

vec3 shadowRay(vec3 player, vec3 pos, int id, float lightLim) {
	vec3 playerDir = normalize(player - pos);
	vec3 normal = getNormal(objs[id], pos);
	float alignment = abs(dot(playerDir, normal));

	vec3 gatheredLight = vec3(0);

	for (int i = 0; i < nlight; i++) {
		if (lcols[i].w <= 0) continue;
		vec3 lightDir = normalize(lights[i].xyz - pos);
		float lightDist = distance(lights[i].xyz, pos);
		float light = lcols[i].w / (lightDist * lightDist);
		if (light < lightLim) continue;
		bool shadow = castRay(Ray(pos, lightDir), objs, lightDist);
		if (!shadow) {
			vec3 normal = getNormal(objs[id], pos);
			float alignment = max(0, dot(lightDir, normal));
			gatheredLight += light * alignment * lcols[i].xyz;
		}
	}

	return gatheredLight;
}

void main() {
	ivec2 pixel_coords = ivec2(gl_GlobalInvocationID.xy);
	vec2 nuv = vec2(pixel_coords) / sdims;

	vec4 data = getData(nuv);
	vec3 hitPos = data.xyz + pos;
	int id = int(data.w);

	if (id == -1) {
		imageStore(out_img, pixel_coords, vec4(0));
	} else {
		vec3 shadow = clamp(shadowRay(pos, hitPos, id, 0.1), -2, 2);
		imageStore(out_img, pixel_coords, vec4(shadow, 1));
	}
}