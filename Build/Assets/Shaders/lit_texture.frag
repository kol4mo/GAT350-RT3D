#version 430

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
	vec3 position;
	vec3 Acolor;
	vec3 Dcolor;
} light;

layout(binding = 0) uniform sampler2D tex;

vec3 ads(in vec3 position, in vec3 normal) {
	vec3 ambient = light.Acolor;//Ambient lighting

	vec3 lightDir = normalize(light.position - fposition);//find light Direction
	float intensity = max(dot(lightDir, fnormal), 0); //intensity based on power and distance
	vec3 diffuse = material.diffuse * (light.Dcolor * intensity);//tge objects color value based on diffuse

	//Specular
	vec3 specular = vec3(0);
	if (intensity > 0) {
		vec3 reflection = reflect(-lightDir, fnormal);//reflection, where a direct beam of light would go
		vec3 viewDir = normalize(-fposition); //the direction from the pixel to the camera
		intensity = max(dot(reflection, viewDir), 0);
		intensity = pow(intensity, material.shininess);
		specular = material.specular * intensity;//color value of specular
	}

	return ambient + diffuse + specular;//total lighting value
}
void main()
{
	vec4 texcolor = texture(tex, ftexcoord);
	ocolor = texcolor * vec4(ads(fposition, fnormal), 1); // fcolor;
}
