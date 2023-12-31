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
	vec3 Acolor;
	vec3 Dcolor;
} light;

layout(binding = 0) uniform sampler2D tex;

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

	//Diffuse
	vec3 lightDir = (light.type == Directional) ? normalize(-light.direction) : normalize(light.position - fposition);//find light Direction
	
	float spotIntensity = 1;
	if (light.type == SPOT) {
		float angle = acos(dot(light.direction, -lightDir));
		spotIntensity = smoothstep(light.outerAngle, light.innerAngle, angle);
	}
	
	float intensity = max(dot(lightDir, fnormal), 0) * spotIntensity; //intensity based on power and distance
	vec3 diffuse = material.diffuse * (light.Dcolor * intensity);//tge objects color value based on diffuse

	//Specular
	vec3 specular = vec3(0);
	if (intensity > 0) {
		vec3 reflection = reflect(-lightDir, fnormal);//reflection, where a direct beam of light would go
		vec3 viewDir = normalize(-fposition); //the direction from the pixel to the camera
		intensity = max(dot(reflection, viewDir), 0);
		intensity = pow(intensity, material.shininess);
		specular = material.specular * intensity * spotIntensity;//color value of specular
	}

	return ambient + (diffuse + specular) * light.intensity * attenuation;//total lighting value
}
void main()
{
	vec4 texcolor = texture(tex, ftexcoord);
	ocolor = texcolor * vec4(ads(fposition, fnormal), 1); // fcolor;
}
