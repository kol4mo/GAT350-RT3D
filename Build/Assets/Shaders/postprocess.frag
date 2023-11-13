#version 430
#define INVERT_MASK (1<<0)
#define GRAYSCALE_MASK (1<<1)
#define COLORTINT_MASK (1<<2)
#define GRAIN_MASK (1<<3)
#define SCANLINE_MASK (1<<4)
#define KERNEL_MASK (1<<5)
#define BLUR_MASK (1<<6)
in layout(location = 0) vec2 ftexcoord;

out layout(location = 0) vec4 ocolor;

uniform float blend = 0;
uniform uint params = 0;
uniform vec4 Tint = {1, 1, 1, 1};
uniform float time = 0;
const float offset = 1.0/300.0;


layout(binding = 0) uniform sampler2D screenTexture;

vec4 invert(in vec4 color) {
	return vec4(vec3(1) - color.rgb, color.a);
}

vec4 grayscale(in vec4 color) {
	return vec4(vec3((0.2126 * color.r + 0.7152 * color.g + 0.0722 * color.b)), color.a);
}

vec4 colorTint(in vec4 color, in vec4 tint) {
	//return vec4((tint.a * color.rgb) + ((1.0 - color.a) * tint.rgb), tint.a);
	return vec4((color.r + tint.r)/2, (color.g + tint.g)/2,(color.b + tint.b)/2, (color.a + tint.a)/2); 
}
float random(vec2 st) 
{
	return fract(sin(dot(st.xy, vec2(12.9898, 78.233))) * 43758.5453);
}
vec4 Grain(in vec4 color) {
	return color * random(gl_FragCoord.xy + time);
}
vec4 Scanline(in vec4 color) {
	return (mod(int(gl_FragCoord.y), 4) != 0) ? vec4(0) : color;
}

vec4 Kernel(in vec4 color) {
    vec2 offsets[9] = vec2[](
        vec2(-offset,  offset), // top-left
        vec2( 0.0f,    offset), // top-center
        vec2( offset,  offset), // top-right
        vec2(-offset,  0.0f),   // center-left
        vec2( 0.0f,    0.0f),   // center-center
        vec2( offset,  0.0f),   // center-right
        vec2(-offset, -offset), // bottom-left
        vec2( 0.0f,   -offset), // bottom-center
        vec2( offset, -offset)  // bottom-right    
    );
	
    float kernel[9] = float[](
        -1, -1, -1,
        -1, 8, -1, 
        -1, -1, -1
    );

	vec3 sampleTex[9];
	for(int i = 0; i < 9; i++)
    {
        sampleTex[i] = vec3(texture(screenTexture, ftexcoord.st + offsets[i]));
    }
	vec3 col = vec3(0.0);
    for(int i = 0; i < 9; i++)
        col += sampleTex[i] * kernel[i];

	return vec4(col, 1.0);
}
vec4 blur(in vec4 color) {
    vec2 offsets[9] = vec2[](
        vec2(-offset,  offset), // top-left
        vec2( 0.0f,    offset), // top-center
        vec2( offset,  offset), // top-right
        vec2(-offset,  0.0f),   // center-left
        vec2( 0.0f,    0.0f),   // center-center
        vec2( offset,  0.0f),   // center-right
        vec2(-offset, -offset), // bottom-left
        vec2( 0.0f,   -offset), // bottom-center
        vec2( offset, -offset)  // bottom-right    
    );
	
    float kernel[9] = float[](
    1.0/9.0, 1.0/9.0, 1.0/9.0,
    1.0/9.0, 1.0/9.0, 1.0/9.0,
    1.0/9.0, 1.0/9.0, 1.0/9.0
    );

	vec3 sampleTex[9];
	for(int i = 0; i < 9; i++)
    {
    vec2 sampledCoord = clamp(ftexcoord.st + offsets[i], 0.0, 1.0);
    sampleTex[i] = vec3(texture(screenTexture, sampledCoord));
    }
	vec3 col = vec3(0.0);
    for(int i = 0; i < 9; i++)
        col += sampleTex[i] * kernel[i];

	return vec4(col, 1.0);
}
void main()
{


	vec4 basecolor = texture(screenTexture,(ftexcoord));
	vec4 postprocess = basecolor;
	if (bool(params & KERNEL_MASK)) postprocess = Kernel(postprocess);
	if (bool(params & BLUR_MASK)) postprocess = blur(postprocess);
	if (bool(params & INVERT_MASK)) postprocess = invert(postprocess);
	if (bool(params & GRAYSCALE_MASK)) postprocess = grayscale(postprocess);
	if (bool(params & COLORTINT_MASK)) postprocess = colorTint(postprocess, Tint);
	if (bool(params & GRAIN_MASK)) postprocess = Grain(postprocess);
	if (bool(params & SCANLINE_MASK)) postprocess = Scanline(postprocess);
	ocolor = mix(basecolor, postprocess, blend);
}
	