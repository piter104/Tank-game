#version 330


out vec4 pixelColor;
in vec4 n;
in vec4 v;
in vec4 l;

in float d;
in float d2;

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

    vec4 r = reflect(-ml, mn);

    float nl = clamp(dot(mn, ml), 0, 1);
    float rv = pow(clamp(dot(r, mv), 0, 1),50);

    vec4 mn2 = normalize(n2);
    vec4 mv2 = normalize(v2);
    vec4 ml2 = normalize(l2);

    vec4 r2 = reflect(-ml2, mn2);

    float nl2 = clamp(dot(mn2, ml2), 0, 1);
    float rv2 = pow(clamp(dot(r2, mv2), 0, 1), 50);

    vec4 ks = kd;

	pixelColor= vec4(kd.rgb * (nl*d) + kd.rgb*(nl2*d2), kd.a) + vec4(ks.rgb*rv, 0)*d + vec4(ks.rgb * rv2, 0)*d2;
}
