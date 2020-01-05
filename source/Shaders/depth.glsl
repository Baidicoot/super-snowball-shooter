#version 430

#define OBJ 256

layout(local_size_x = 1, local_size_y = 1) in;
layout(rgba32f, binding = 0) uniform image2D img_out;

float maxDist = 10;

uniform float time;
uniform vec3 pos;
uniform vec3 dir;
uniform ivec2 rdims;
uniform vec4[OBJ] objs;
uniform int nobj;

vec2 fov = vec2(1.3, 0.7);

struct RayResult {
	float t1;
	float t2;
};

struct Intersection {
	bool exists;
	int id;
	float d;
};

struct Ray {
	vec3 o;
	vec3 d;
};

struct Sphere {
	vec3 c;
	float r;
};

mat3 yaw(float a) {
	return mat3(
		cos(a), -sin(a), 0,
		sin(a), cos(a), 0,
		0, 0, 1
	);
}

mat3 pitch(float b) {
	return mat3(
		cos(b), 0, sin(b),
		0, 1, 0,
		-sin(b), 0, cos(b)
	);
}

mat3 roll(float c) {
	return mat3(
		1, 0, 0,
		0, cos(c), -sin(c),
		0, sin(c), cos(c)
	);
}

RayResult intersectSphere(Ray r, vec4 s) {
	float t = dot(s.xyz - r.o, r.d);
	//if (t < 0) return RayResult(maxDist, maxDist);
	float y = distance(r.o + t*r.d, s.xyz);
	if (y >= s.w || t < 0) return RayResult(maxDist, maxDist);
	float x = sqrt(s.w*s.w - y*y);
	return RayResult(t - x, t + x);
}

Intersection castRay(Ray r, vec4[OBJ] s) {
	Intersection minI = Intersection(false, 0, maxDist);

	for (int i = 0; i < nobj; i++) {
		RayResult r = intersectSphere(r, s[i]);
		if (r.t1 < minI.d) {
			minI = Intersection(true, i, r.t1);
		}
	}

	return minI;
}

void main() {
	vec4 pixel = vec4(0);
	ivec2 pixel_coords = ivec2(gl_GlobalInvocationID.xy);

	float max_x = 5.0;
	float max_y = 5.0;
	vec2 uv = (vec2(pixel_coords) / vec2(rdims) - vec2(0.5));
	vec3 ray_t = vec3(uv.x * fov.x, uv.y * fov.y, 1);
	vec3 ray_d = normalize(ray_t) * pitch(dir.x) * roll(dir.y) * yaw(dir.z);
	Ray r = Ray(pos, ray_d);

	Intersection o = castRay(r, objs);

	if (o.exists) {
		pixel = vec4(normalize(r.d) * o.d, o.id);
	} else {
		pixel = vec4(normalize(r.d) * maxDist, -1);
	}

	imageStore(img_out, pixel_coords, pixel);
}