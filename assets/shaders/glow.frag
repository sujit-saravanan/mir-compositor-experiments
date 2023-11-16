precision mediump float;
varying vec2 vTexCoords;
uniform sampler2D uSampler;
uniform float uFadeGlow;
void main()
{
    vec4 col = texture2D(uSampler, vTexCoords);
    col = col * uFadeGlow;
    gl_FragColor = col;
}