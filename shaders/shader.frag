#version 330 core


in vec3 Normal;
in vec3 FragPos;
//uniform sampler2D ourTexture1;
//uniform sampler2D ourTexture2;

out vec4 color;

uniform vec3 objectColor;
uniform vec3 lightColor;

uniform vec3 lightPos;




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

     vec3 result = (ambient + diffuse) * objectColor;

     color = vec4(result,1.0f);
};