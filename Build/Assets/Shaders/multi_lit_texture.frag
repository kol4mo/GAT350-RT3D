#version 430
#define POINT 0
#define Directional 1
#define SPOT 2

in layout(location = 0) vec3 fposition;
in layout(location = 1) vec3 fnormal;
in layout(location = 2) vec2 ftexcoord;
//in layout(location = 3) vec4 fcolor;

out layout(location = 0) vec4 ocolor;


uniform struct Material
{
	vec3 diffuse;
	vec3 specular;
	float shininess;
		vec2 offset;
	vec2 tiling;
} material;

uniform struct Light {//GUI light changes
	int type;
	vec3 position;
	vec3 direction;
	float innerAngle;
	float outerAngle;
	float intensity;
	float range;
	vec3 Dcolor;
} lights[3];

vec3 ambientColor;

uniform int numLights = 3;
layout(binding = 0) uniform sampler2D tex;

void phong(in Light light, in vec3 position, in vec3 normal, out vec3 diffuse, out vec3 specular) {

vec3 lightDir = (light.type == Directional) ? normalize(-light.direction) : normalize(light.position - fposition);//find light Direction
	
	float spotIntensity = 1;
	if (light.type == SPOT) {
		float angle = acos(dot(light.direction, -lightDir));
		spotIntensity = smoothstep(light.outerAngle, light.innerAngle, angle);
	}
	
	float intensity = max(dot(lightDir, fnormal), 0) * spotIntensity; //intensity based on power and distance
	diffuse = material.diffuse * (light.Dcolor * intensity);//tge objects color value based on diffuse

	//Specular
	specular = vec3(0);
	if (intensity > 0) {
		vec3 reflection = reflect(-lightDir, fnormal);//reflection, where a direct beam of light would go
		vec3 viewDir = normalize(-fposition); //the direction from the pixel to the camera
		intensity = max(dot(reflection, viewDir), 0);
		intensity = pow(intensity, material.shininess);
		specular = material.specular * intensity * spotIntensity;//color value of specular
	}

}

/*
vec3 ads(in vec3 position, in vec3 normal) {
	vec3 ambient = light.Acolor;//Ambient lighting

	//Attenuation
	float attenuation = 1;
	if (light.type != Directional) {
		float distanceSqr = dot(light.position - position, light.position - position);
		float rangeSqr = light.range * light.range;
		attenuation = max(0, 1-pow((distanceSqr / rangeSqr), 2.0));
		attenuation = attenuation * attenuation;
	}


	return ambient + (diffuse + specular) * light.intensity * attenuation;//total lighting value
}
*/
void main()
{
	vec4 texcolor = texture(tex, ftexcoord);

	ocolor = vec4(ambientColor, 1);

	for (int i= 1; i < numLights; i++) {
		vec3 diffuse;
		vec3 specular;

		phong(lights[i], fposition, fnormal, diffuse, specular);
		ocolor += (vec4(diffuse, 1) * texcolor) + vec4(specular, 1); 
	}
}
