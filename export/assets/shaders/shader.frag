varying vec4 color;
varying vec2 texCoord;

uniform sampler2D tex;
uniform vec4 colorOverride;
uniform float time;
uniform vec2 resolution;

void main(void)
{
    vec2 uv = texCoord;
    vec4 c = color;// * colorOverride;

    gl_FragColor = vec4(0.,0.,1.,1.);

    /* baseline frag :)
    vec2 uv = texCoord;
    vec4 c = color;

    gl_FragColor = texture2D(tex, uv) * c;
    */
}