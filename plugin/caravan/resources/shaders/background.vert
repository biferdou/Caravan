#version 120

attribute vec4 position;
attribute vec2 texureCoordIn;

varying vec2 textureCoordOut;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;

void main() {
    textureCoordOut = texureCoordIn;
    gl_Position = projectionMatrix * viewMatrix * position;
}