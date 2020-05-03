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
out float p;

uniform vec4 lp;

void main(void) {
    float d;

    l = normalize(V * lp - V * M * vertex);
    n = normalize(V * M * normal);

    //vec4 r = reflect(-l, n);
    v = normalize(vec4(0, 0, 0, 1) - V * M * vertex);

    //float nl = clamp(dot(n, l), 0, 1);
    //float rv = pow(clamp(dot(r, v), 0, 1),25);
    d = distance(vertex, M*vec4(0, 0, 0, 1));
    d = 1-(d/ 100.0f);
    p = d;
    //iC = vec4(color.rgb*nl, color.a)+vec4(rv,rv,rv,0);
    iC = color;
    gl_Position=P*V*M*vertex;

}
