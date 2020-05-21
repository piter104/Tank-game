#version 330

//Zmienne jednorodne
uniform mat4 P;
uniform mat4 V;
uniform mat4 M;

in vec4 vertex;
in vec2 aTexCoord;

out vec2 TexCoord;

void main(void) {

    gl_Position=P*V*M*vertex;
    TexCoord = aTexCoord;

}
