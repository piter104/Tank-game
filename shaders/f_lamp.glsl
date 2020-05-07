#version 330


out vec4 pixelColor;

in vec2 TexCoord;

uniform sampler2D ourTexture;

void main(void) {
    vec4 kd = texture(ourTexture, TexCoord);

	pixelColor= vec4(kd.rgb, kd.a);

}
