#version 330

//Zmienne jednorodne
uniform mat4 P;
uniform mat4 V;
uniform mat4 M;

//Atrybuty
in vec4 vertex; //wspolrzedne wierzcholka w przestrzeni modelu
in vec4 color;
in vec4 normal;
out vec4 iC;
out vec4 l;
out vec4 n;
out vec4 v;

out vec4 l2;
out vec4 n2;
out vec4 v2;

out float d;
out float d2;


uniform vec4 lp;
uniform vec4 lp2;

in vec2 aTexCoord;

out vec2 TexCoord;

void main(void) {
    // float d;

    l = normalize(V * lp - V * M * vertex);
    n = normalize(V * M * normal);

    //vec4 r = reflect(-l, n);
    v = normalize(vec4(0, 0, 0, 1) - V * M * vertex);

    l2 = normalize(V * lp2 - V * M * vertex);
    n2 = normalize(V * M * normal);

    //vec4 r = reflect(-l, n);
    v2 = normalize(vec4(0, 0, 0, 1) - V * M * vertex);

    //float nl = clamp(dot(n, l), 0, 1);
    //float rv = pow(clamp(dot(r, v), 0, 1),25);
    d = distance(vertex, M*vec4(0, 0, 0, 1));
    d = 1-(d/30.0f);

    d2 = distance(vertex, M*vec4(0,0,0,1));
    d2 = 1-(d2 / 30.0f);

    //iC = vec4(color.rgb*nl, color.a)+vec4(rv,rv,rv,0);
    iC = color;
    gl_Position=P*V*M*vertex;
    TexCoord = aTexCoord;

}
