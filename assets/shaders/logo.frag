precision mediump float;
varying vec2 vTexCoords;
uniform sampler2D uSampler;
uniform float uFadeLogo;
void main()
{
    vec4 col = texture2D(uSampler, vTexCoords);
    col = col * uFadeLogo;
    gl_FragColor = col;
}
