#version 330


out vec4 pixelColor; //Zmienna wyjsciowa fragment shadera. Zapisuje sie do niej ostateczny (prawie) kolor piksela
in vec4 iC;
in vec4 n;
in vec4 v;
in vec4 l;

in vec2 TexCoord;

uniform sampler2D ourTexture;

void main(void) {
    vec4 kd = texture(ourTexture, TexCoord);
    vec4 mn = normalize(n);
    vec4 mv = normalize(v);
    vec4 ml = normalize(l);

    vec4 r = reflect(-ml, mn)*0;

    float nl = clamp(pow(dot(mn, ml), 0.7f), 0, 1);
    float rv = pow(clamp(dot(r, mv), 0, 1),32);
    //d = distance(V * M * vertex, vec4(0, 0, 0, 1));
   // d = 1-((d - 3.3) / 1.7);

    //iC = vec4(color.rgb*nl, color.a)+vec4(rv,rv,rv,0);
	pixelColor= vec4(kd.rgb * nl, kd.a) + vec4(rv, rv, rv, 0);
    //mix(texture(ourTexture1, TexCoord), texture(ourTexture2, TexCoord), 0.2)
}
