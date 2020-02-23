#version 330 core


in vec3 Normal; //Данные меняются с каждой вершиной и загружены в VBO т.е. память GPU

in vec3 FragPos;

//А это неизменяющаяся (для каждой вершины) переменная
//uniform sampler2D ourTexture1;
//uniform sampler2D ourTexture2;

out vec4 color;


uniform vec3 viewPos;


struct Material {
  sampler2D diffuse; 
  vec3 specular;
  float shininess;
};

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Material material;
uniform Light light;

in vec2 TexCoords;

void main(void)
{
   // float ambientStrength = 0.1f;
   // vec3  ambient = ambientStrength  * lightColor; //для данного шейдера константа
    vec3  ambient = light.ambient * vec3(texture(material.diffuse,TexCoords));

  

    vec3 normal = normalize(Normal);
   

    //нам важно только направление ни размер ни местоположение не интересны поэтому нормализация
    //которая упростит скалярное произведение для нахождения угла между векторами

     //Вектор направления между источником света и фрагментом
     vec3 lightDir = normalize(light.position - FragPos);
    //Это уже величина воздействия диффузного освещения
    //max нам нужен чтобы убрать отрицательнцю состоявляющую после умножения
     float diff = max(dot(normal, lightDir), 0.0);
    //Компонента диффузного освещения становиться темнее с ростом угла между векторами
      vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse,TexCoords));

      //среднее значение чтобы он не оказыва слишком сильного воздействия
      // float specularStrength = 0.5f;
       vec3 viewDir = normalize(viewPos - FragPos);

       vec3 reflectDir = reflect(-lightDir, normal);

    //32 константное значение задающее силу блеска
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * material.specular);

     //vec3 result = (ambient + diffuse+ specular) * objectColor;

     //В ambient мы заложили цвет источника цвета
      vec3 result = ambient + diffuse + specular;

      color = vec4(result,1.0f);
};