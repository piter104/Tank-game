#version 330


out vec4 pixelColor; //Zmienna wyjsciowa fragment shadera. Zapisuje sie do niej ostateczny (prawie) kolor piksela
in vec4 iC;
in vec4 n;
in vec4 v;
in vec4 l;



//in vec4 n2;
//in vec4 v2;
//in vec4 l2;

in vec2 TexCoord;

uniform sampler2D ourTexture;

void main(void) {
    vec4 kd = texture(ourTexture, TexCoord);
    vec4 mn = normalize(n);
    vec4 mv = normalize(v);
    vec4 ml = normalize(l);

    //vec4 mn2 = normalize(n2);
    //vec4 mv2 = normalize(v2);
    //vec4 ml2 = normalize(l2);

    vec4 r = reflect(-ml, mn)*0;
    //vec4 r2 = reflect(-ml2, mn2) * 0;

    float nl = clamp(pow(dot(mn, ml),0.7), 0, 1);
    float rv = pow(clamp(dot(r, mv), 0, 1),32);

    /*float nl2 = clamp(dot(mn2, ml2), 0, 1);
    float rv2 = pow(clamp(dot(r2, mv2), 0, 1), 32);*/
    //d = distance(V * M * vertex, vec4(0, 0, 0, 1));
   // d = 1-((d - 3.3) / 1.7);

    //iC = vec4(color.rgb*nl, color.a)+vec4(rv,rv,rv,0);
	pixelColor= vec4(kd.rgb * nl, kd.a) + vec4(rv, rv, rv, 0);
    //mix(texture(ourTexture1, TexCoord), texture(ourTexture2, TexCoord), 0.2)
}
