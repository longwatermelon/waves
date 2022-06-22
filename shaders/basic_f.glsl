#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
 
void main()
{
    vec3 norm = normalize(Normal);
    vec3 lightDir = vec3(0.0, -1.0, 0.0);

    float diff = max(dot(norm, lightDir), 0.0);
    FragColor = vec4(diff * vec3(0.3, 0.3, 0.8), 1.0);
}

