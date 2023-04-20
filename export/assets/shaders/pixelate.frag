varying vec4 color;
varying vec2 texCoord;

uniform sampler2D tex;
uniform vec4 colorOverride;
uniform float time;
uniform vec2 resolution;

uniform float pixels = 2;

vec3 sampleVideo(vec2 uv)
{
    return texture2D(tex, uv).rgb;
}

void main(void)
{
   	vec2 BLOCK_SIZE = vec2(pixels, pixels) / resolution.xy;

    vec2 uvOriginal = texCoord;

    vec2 uvPixels = vec2(0.0, 0.0);
    uvPixels.x = floor(uvOriginal.x / BLOCK_SIZE.x) * BLOCK_SIZE.x;
    uvPixels.y = floor(uvOriginal.y / BLOCK_SIZE.y) * BLOCK_SIZE.y;

    if (pixels > 0.0)
    {
        gl_FragColor = vec4(sampleVideo(uvPixels), 1.0);
    }
    else
    {
        gl_FragColor = vec4(sampleVideo(uvOriginal), 1.0);
    }
}