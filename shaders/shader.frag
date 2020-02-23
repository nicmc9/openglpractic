#version 330 core


in vec3 Normal; //������ �������� � ������ �������� � ��������� � VBO �.�. ������ GPU

in vec3 FragPos;

//� ��� �������������� (��� ������ �������) ����������
//uniform sampler2D ourTexture1;
//uniform sampler2D ourTexture2;

out vec4 color;


uniform vec3 viewPos;


struct Material {
  sampler2D diffuse; 
  sampler2D specular;
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
   // vec3  ambient = ambientStrength  * lightColor; //��� ������� ������� ���������
    vec3  ambient = light.ambient * vec3(texture(material.diffuse,TexCoords));

  

    vec3 normal = normalize(Normal);
   

    //��� ����� ������ ����������� �� ������ �� �������������� �� ��������� ������� ������������
    //������� �������� ��������� ������������ ��� ���������� ���� ����� ���������

     //������ ����������� ����� ���������� ����� � ����������
     vec3 lightDir = normalize(light.position - FragPos);
    //��� ��� �������� ����������� ���������� ���������
    //max ��� ����� ����� ������ ������������� ������������� ����� ���������
     float diff = max(dot(normal, lightDir), 0.0);
    //���������� ���������� ��������� ����������� ������ � ������ ���� ����� ���������
      vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse,TexCoords));

      //������� �������� ����� �� �� ������� ������� �������� �����������
      // float specularStrength = 0.5f;
       vec3 viewDir = normalize(viewPos - FragPos);

       vec3 reflectDir = reflect(-lightDir, normal);

    //32 ����������� �������� �������� ���� ������
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    vec3 specular = light.specular * spec * vec3(texture(material.specular,TexCoords));


     //� ambient �� �������� ���� ��������� �����
      vec3 result = ambient + diffuse + specular;

      color = vec4(result,1.0f);
};