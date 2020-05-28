#version 330

out vec4 pixelColor; //Zmienna wyjsciowa fragment shadera. Zapisuje sie do niej ostateczny (prawie) kolor piksela
in vec4 n;
in vec4 v;
in vec4 l;

in vec4 n2;
in vec4 v2;
in vec4 l2;

in vec2 TexCoord;

uniform sampler2D ourTexture;

void main(void) {
    vec4 kd = texture(ourTexture, TexCoord);
    vec4 mn = normalize(n);
    vec4 mv = normalize(v);
    vec4 ml = normalize(l);

    float nl = clamp(dot(mn, ml)+0.1, 0, 1);

    vec4 mn2 = normalize(n2);
    vec4 mv2 = normalize(v2);
    vec4 ml2 = normalize(l2);

    float nl2 = clamp(dot(mn2, ml2)+0.1, 0, 1);

    pixelColor = vec4(kd.rgb * nl + kd.rgb * nl2, kd.a);

}
