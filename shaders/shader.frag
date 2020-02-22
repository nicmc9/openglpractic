#version 330 core


in vec3 Normal; //Данные меняются с каждой вершиной и загружены в VBO т.е. память GPU

in vec3 FragPos;

//А это неизменяющаяся (для каждой вершины) переменная
//uniform sampler2D ourTexture1;
//uniform sampler2D ourTexture2;

out vec4 color;

uniform vec3 objectColor; //ТУТ тоже можно перевести на константы но это не гибкое решение
uniform vec3 lightColor;

uniform vec3 lightPos;
uniform vec3 viewPos;



void main(void)
{
    float ambientStrength = 0.1f;
    vec3  ambient = ambientStrength  * lightColor; //для данного шейдера константа
  
  

    vec3 normal = normalize(Normal);
   

    //нам важно только направление ни размер ни местоположение не интересны поэтому нормализация
    //которая упростит скалярное произведение для нахождения угла между векторами

     //Вектор направления между источником света и фрагментом
     vec3 lightDir = normalize(lightPos - FragPos);
    //Это уже величина воздействия диффузного освещения
    //max нам нужен чтобы убрать отрицательнцю состоявляющую после умножения
     float diff = max(dot(normal, lightDir), 0.0);
    //Компонента диффузного освещения становиться темнее с ростом угла между векторами
      vec3 diffuse = diff * lightColor;

      //среднее значение чтобы он не оказыва слишком сильного воздействия
       float specularStrength = 0.5f;
       vec3 viewDir = normalize(viewPos - FragPos);

       vec3 reflectDir = reflect(-lightDir, normal);

    //32 константное значение задающее силу блеска
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;

     //vec3 result = (ambient + diffuse+ specular) * objectColor;

     //В ambient мы заложили цвет источника цвета
      vec3 result = (ambient + diffuse + specular) * objectColor;

     color = vec4(result,1.0f);
};