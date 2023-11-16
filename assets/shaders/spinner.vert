attribute vec4 vPosition;
attribute vec2 aTexCoords;
uniform float theta;
varying vec2 vTexCoords;
void main()
{
    float c = cos(theta);
    float s = sin(theta);
    mat2 m;
    m[0] = vec2(c, s);
    m[1] = vec2(-s, c);
    vTexCoords = m * aTexCoords + vec2 (0.5, 0.5);
    gl_Position = vec4(vPosition.xy, -1.0, 1.0);
}