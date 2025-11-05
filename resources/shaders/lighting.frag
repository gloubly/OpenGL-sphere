# version 460 core
in vec3 FragPos;
in vec3 Normal;  

out vec4 FragColor;

uniform vec3 object_color;
uniform vec3 light_color;
uniform vec3 light_pos;
uniform vec3 view_pos;

void main() {
	// ambient
	float ambient_strength = 0.1;
	vec3 ambient = ambient_strength * light_color;

	// difuse
	vec3 norm = normalize(Normal);
	vec3 light_direction = normalize(light_pos - FragPos);
	vec3 diffuse = max(dot(norm, light_direction), 0.0) * light_color;

	// specular
	float specular_strength = 0.5;
	vec3 view_direction = normalize(view_pos - FragPos);
	vec3 reflect_direction = reflect(-light_direction, norm);
	vec3 specular = pow(max(dot(view_direction, reflect_direction), 0.0), 32) * light_color * specular_strength;


	vec3 result = (ambient + diffuse + specular) * object_color;
	FragColor = vec4(result, 1.0);
}