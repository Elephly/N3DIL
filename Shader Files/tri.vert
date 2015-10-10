#version 150


// Doron Nussbaum
// Vertex Shader example

attribute vec3 vtxPos;
attribute vec4 vtxColor;

varying vec4 vColor;

void main(void)

{
	vColor = vtxColor;
	gl_Position = vec4(vtxPos, 1.0);
}