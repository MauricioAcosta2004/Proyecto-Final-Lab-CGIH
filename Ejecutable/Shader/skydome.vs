#version 330 core
layout (location = 0) in vec3 position;
layout (location = 2) in vec2 texCoords;

out vec2 TexCoords;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main() {
    TexCoords = texCoords;
    // Multiplicar matriz de modelo, vista y proyección
    vec4 pos = projection * view * model * vec4(position, 1.0);
    // xyww asegura que la profundidad (Z) sea máxima (1.0) para quedar al fondo
    gl_Position = pos.xyww; 
}