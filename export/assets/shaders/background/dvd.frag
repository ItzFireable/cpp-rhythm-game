varying vec4 color;
varying vec2 texCoord;

uniform vec2 resolution;
uniform sampler2D tex;
uniform vec4 colorOverride;
uniform float time;

#define PI 3.14159265359

float vmin(vec2 v) {
	return min(v.x, v.y);
}

float vmax(vec2 v) {
	return max(v.x, v.y);
}

float ellip(vec2 p, vec2 s) {
    float m = vmin(s);
	return (length(p / s) * m) - m;
}

float halfEllip(vec2 p, vec2 s) {
    p.x = max(0., p.x);
    float m = vmin(s);
	return (length(p / s) * m) - m;
}


float fBox(vec2 p, vec2 b) {
	return vmax(abs(p) - b);
}

float dvd_d(vec2 p) {
    float d = halfEllip(p, vec2(.8, .5));
    d = max(d, -p.x - .5);
    float d2 = halfEllip(p, vec2(.45, .3));
    d2 = max(d2, min(-p.y + .2, -p.x - .15));
    d = max(d, -d2);
    return d;
}

float dvd_v(vec2 p) {
    vec2 pp = p;
    p.y += .7;
    p.x = abs(p.x);
    vec2 a = normalize(vec2(1,-.55));
    float d = dot(p, a);
    float d2 = d + .3;
    p = pp;
    d = min(d, -p.y + .3);
    d2 = min(d2, -p.y + .5);
    d = max(d, -d2);
    d = max(d, abs(p.x + .3) - 1.1);
	return d;
}


float dvd_c(vec2 p) {
    p.y += .95;
	float d = ellip(p, vec2(1.8,.25));
    float d2 = ellip(p, vec2(.45,.09));
    d = max(d, -d2);
    return d;
}

float dvd(vec2 p) {
    p.y -= .345;
    p.x -= .035;
    p *= mat2(1,-.2,0,1);
	float d = dvd_v(p);
    d = min(d, dvd_c(p));
    p.x += 1.3;
    d = min(d, dvd_d(p));
    p.x -= 2.4;
    d = min(d, dvd_d(p));
    return d;
}



float range(float vmin, float vmax, float value) {
  return (value - vmin) / (vmax - vmin);
}

float rangec(float a, float b, float t) {
    return clamp(range(a, b, t), 0., 1.);
}

// https://www.shadertoy.com/view/ll2GD3
vec3 pal( in float t, in vec3 a, in vec3 b, in vec3 c, in vec3 d ) {
    return a + b*cos( 6.28318*(c*t+d) );
}
vec3 spectrum(float n) {
    return pal( n, vec3(0.5,0.5,0.5),vec3(0.5,0.5,0.5),vec3(1.0,1.0,1.0),vec3(0.0,0.33,0.67) );
}


void drawHit(inout vec4 col, vec2 p, vec2 hitPos, float hitDist) {

    float d = length(p - hitPos);

    #ifdef DEBUG
    col = mix(col, vec4(0,1,1,0), step(d, .1)); return;
    #endif

    float wavefront = d - hitDist * 1.5;
    float freq = 2.;

    vec3 spec = (1. - spectrum(-wavefront * freq + hitDist * freq));
    float ripple = sin((wavefront * freq) * PI*2. - PI/2.);

    float blend = smoothstep(3., 0., hitDist);
    blend *= smoothstep(.2, -.5, wavefront);
    blend *= rangec(-4., .0, wavefront);

    col.rgb *= mix(vec3(1), spec, pow(blend, 4.));
    float height = (ripple * blend);
    col.a -= height * 1.9 / freq;
}

vec2 ref(vec2 p, vec2 planeNormal, float offset) {
	float t = dot(p, planeNormal) + offset;
	p -= (2. * t) * planeNormal;
    return p;
}

void drawReflectedHit(inout vec4 col, vec2 p, vec2 hitPos, float hitDist, vec2 screenSize) {
    col.a += length(p) * .0001; // fix normal when flat
	//drawHit(col, p, hitPos, hitDist); return;
    drawHit(col, p, ref(hitPos, vec2(0,1), 1.), hitDist);
    drawHit(col, p, ref(hitPos, vec2(0,-1), 1.), hitDist);
    drawHit(col, p, ref(hitPos, vec2(1,0), screenSize.x/screenSize.y), hitDist);
    drawHit(col, p, ref(hitPos, vec2(-1,0), screenSize.x/screenSize.y), hitDist);
}


// Flip every second cell to create reflection
void flip(inout vec2 pos) {
    vec2 flip = mod(floor(pos), 2.);
    pos = abs(flip - mod(pos, 1.));
}

float stepSign(float a) {
    //return sign(a);
	return step(0., a) * 2. - 1.;
}

vec2 compassDir(vec2 p) {
    //return sign(p - sign(p) * vmin(abs(p))); // this caused problems on some GPUs
    vec2 a = vec2(stepSign(p.x), 0);
    vec2 b = vec2(0, stepSign(p.y));
    float s = stepSign(p.x - p.y) * stepSign(-p.x - p.y);
    return mix(a, b, s * .5 + .5);
}

vec2 calcHitPos(vec2 move, vec2 dir, vec2 size) {
    vec2 hitPos = mod(move, 1.);
    vec2 xCross = hitPos - hitPos.x / (size / size.x) * (dir / dir.x);
    vec2 yCross = hitPos - hitPos.y / (size / size.y) * (dir / dir.y);
   	hitPos = max(xCross, yCross);
    hitPos += floor(move);
    return hitPos;
}

void main(void) {
    vec2 p = (-resolution.xy + 2.0*gl_FragCoord.xy)/resolution.y;

    vec2 screenSize = vec2(resolution.x/resolution.y, 1.) * 2.;

    float t = time;
    vec2 dir = normalize(vec2(9.,16) * screenSize );
    vec2 move = dir * t / 1.5;
    float logoScale = .1;
    vec2 logoSize = vec2(2.,.85) * logoScale * 1.;

    vec2 size = screenSize - logoSize * 2.;

    // Remap so (0,0) is bottom left, and (1,1) is top right
    move = move / size + .5;

    // Calculate the point we last crossed a cell boundry
    vec2 lastHitPos = calcHitPos(move, dir, size);
    vec4 col = vec4(1,1,1,0);
    vec4 colFx = vec4(1,1,1,0);
    vec4 colFy = vec4(1,1,1,0);
    vec2 e = vec2(.8,0)/resolution.y;

   	const int limit = 5;

    for (int i = 0; i < limit; i++) {
        vec2 hitPos = lastHitPos;

        if (i > 0) {
            // Nudge it before the boundry to find the previous hit point
            hitPos = calcHitPos(hitPos - .00001/size, dir, size);
        }

        lastHitPos = hitPos;

    	// How far are we from the hit point
    	float hitDist = distance(hitPos, move);

        // Flip every second cell to create reflection
        flip(hitPos);

		// Remap back to screen space
    	hitPos = (hitPos - .5) * size;

        // Push the hits to the edges of the screen
        hitPos += logoSize * compassDir(hitPos / size);
    }

    // Flip every second cell to create reflection
    flip(move);

    // Remap back to screen space
    move = (move - .5) * size;

    // Calc normals
	float bf = .1; // Bump factor
    float fx = (col.a - colFx.a) * 2.; // Nearby horizontal samples.
    float fy = (col.a - colFy.a) * 2.; // Nearby vertical samples.
    vec3 nor = normalize(vec3(fx, fy, e.x/bf)); // Bumped normal.
    float ff = length(vec2(fx, fy));
    float ee = rangec(0., 10./resolution.y, ff);
    nor = normalize(vec3(vec2(fx, fy)*ee, ff));

    // invert colours
    col.rgb = clamp(1. - col.rgb, vec3(0), vec3(1));
    col.rgb /= 3.;


    // lighting
    // iq https://www.shadertoy.com/view/Xds3zN
    vec3 lig = normalize(vec3(1,2,2.));
    vec3 rd = normalize(vec3(p, -10.));
    vec3  hal = normalize( lig - rd );

    float dif = clamp(dot(lig, nor), 0., 1.);
    float spe = pow( clamp( dot( nor, hal ), 0.0, 1.0 ),16.0)*
                    dif *
                    (0.04 + 0.96*pow( clamp(1.0+dot(hal,rd),0.0,1.0), 5.0 ));

    vec3 lin = vec3(0.);
    lin += 5. * dif;
    lin += .2;
    col.rgb = col.rgb * lin;
    col.rgb += 5. * spe;

    // dvd logo
	float d = dvd((p - move) / logoScale);
    d /= fwidth(d);
    d = 1. - clamp(d, 0., 1.);
    col.rgb = mix(col.rgb, vec3(1.), d);

    // gamma
    col.rgb = pow(col.rgb, vec3(1./1.5));
    col.a = 1.;

    // make black and similar colors transparent
    col.a = 0. + smoothstep(0., .1, col.r + col.g + col.b);

    gl_FragColor = col;
}