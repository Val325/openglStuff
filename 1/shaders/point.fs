#version 330 core
out vec4 FragColor;
in vec3 Normal;  
in vec3 FragPos;  
  
uniform vec3 lightPos; 
uniform vec3 lightColor;
uniform vec3 objectColor;


void main()
{
    // Окружающая составляющая
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;
  	
    // Диффузная составляющая 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
            
    vec3 result = (ambient + diffuse) * objectColor;
    
    FragColor = vec4(0.5); // устанавливаем значения всех 4-х компонентов вектора равным 1.0
}
