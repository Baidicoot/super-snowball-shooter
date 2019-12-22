#version 430

layout(location = 0) in vec3 vertexPos;

out vec2 uv;

void main() {
	gl_Position = vec4(vertexPos, 1.0);
	uv = vertexPos.xy;
}