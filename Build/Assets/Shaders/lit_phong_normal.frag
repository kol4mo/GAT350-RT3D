#version 430
#define POINT 0
#define Directional 1
#define SPOT 2
#define ALBEDO_TEXTURE_MASK (1<<0)
#define SPECULAR_TEXTURE_MASK (1<<1)
#define NORMAL_TEXTURE_MASK (1<<2)
#define EMISSIVE_TEXTURE_MASK (1<<3)

in layout(location = 0) vec3 fposition;
in layout(location = 2) mat3 ftbn;
in layout(location = 1) vec2 ftexcoord;
//in layout(location = 3) vec4 fcolor;

out layout(location = 0) vec4 ocolor;


uniform struct Material
{
	uint params;
	vec3 albedo;
	vec3 specular;
	vec3 emissive;
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
	vec3 color;
} lights[3];

uniform vec3 ambientColor;

uniform int numLights = 3;
layout(binding = 0) uniform sampler2D albedoTexture;
layout(binding = 1) uniform sampler2D specularTexture;
layout(binding = 2) uniform sampler2D normalTexture;
layout(binding = 3) uniform sampler2D emissiveTexture;

void phong(in Light light, in vec3 position, in vec3 normal, out vec3 diffuse, out vec3 specular) {

	vec3 lightDir = (light.type == Directional) ? normalize(-light.direction) : normalize(light.position - fposition);//find light Direction
	
	float spotIntensity = 1;
	if (light.type == SPOT) {
		float angle = acos(dot(light.direction, -lightDir));
		spotIntensity = smoothstep(light.outerAngle, light.innerAngle, angle);
	}
	
	float intensity = max(dot(lightDir, normal), 0) * spotIntensity; //intensity based on power and distance
	diffuse = (light.color * intensity);//tge objects color value based on diffuse

	//Specular
	specular = vec3(0);
	if (intensity > 0) {
		vec3 viewDir = normalize(-fposition); //the direction from the pixel to the camera
		//vec3 reflection = reflect(-lightDir, normal);//reflection, where a direct beam of light would go
		//intensity = max(dot(reflection, viewDir), 0);
		vec3 h = normalize(viewDir + lightDir);
		intensity = max(dot(h,normal), 0);
		intensity = pow(intensity, material.shininess);
		specular = vec3(intensity * spotIntensity);//color value of specular
	}

}

float attenuation(in vec3 position1, in vec3 position2, in float range)
{
	float distanceSqr = dot(position1 - position2, position1 - position2);
	float rangeSqr = pow(range, 2.0);
	float attenuation = max(0, 1 - pow((distanceSqr / rangeSqr), 2.0));
	attenuation = pow(attenuation, 2.0);
 
	return attenuation;
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
	vec4 albedoColor = bool(material.params & ALBEDO_TEXTURE_MASK) ? texture(albedoTexture, ftexcoord) : vec4(material.albedo, 1);
	vec4 specularColor = bool(material.params & SPECULAR_TEXTURE_MASK) ? texture(specularTexture, ftexcoord) : vec4(material.specular, 1);
	vec4 emissiveColor = bool(material.params & EMISSIVE_TEXTURE_MASK) ? texture(emissiveTexture, ftexcoord) : vec4(material.emissive, 1);
	//vec4 texcolor = texture(tex, ftexcoord);

	ocolor = vec4(ambientColor, 1) * albedoColor + emissiveColor;

	for (int i= 0; i < numLights; i++) {
		vec3 diffuse;
		vec3 specular;
		float attenuation = (lights[i].type == Directional) ? 1 : attenuation (lights[i].position, fposition, lights[i].range);

		vec3 normal = texture(normalTexture, ftexcoord).rgb;
		normal = (normal * 2) - 1;
		normal = normalize(ftbn * normal);

		phong(lights[i], fposition, normal, diffuse, specular);
		ocolor += ((vec4(diffuse, 1) * albedoColor) + vec4(specular, 1) * specularColor) * attenuation * lights[i].intensity; 
	}

}
