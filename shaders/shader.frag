#version 330 core


in vec3 Normal;
in vec3 FragPos;
//uniform sampler2D ourTexture1;
//uniform sampler2D ourTexture2;

out vec4 color;

uniform vec3 objectColor;
uniform vec3 lightColor;

uniform vec3 lightPos;
uniform vec3 viewPos;



void main(void)
{
    float ambientStrength = 0.1f;
    vec3  ambient = ambientStrength  * lightColor;
  
  

    vec3 norm = normalize(Normal);
    //Вектор направления между источником света и фрагментом
    vec3 lightDir = normalize(lightPos - FragPos);
    
    //Это уже величина воздействия диффузного освещения
    //max нам нужен чтобы убрать отрицательнцю состоявляющую после умножения
    float diff = max(dot(norm, lightDir), 0.0);
    //Компонента диффузного освещения становиться темнее с ростом угла между векторами
    vec3 diffuse = diff * lightColor;

      //среднее значение чтобы он не оказыва слишком сильного воздействия
    float specularStrength = 0.5f;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);

    //32 константное значение задаюдее силу блеска
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;

     vec3 result = (ambient + diffuse+ specular) * objectColor;

     color = vec4(result,1.0f);
};