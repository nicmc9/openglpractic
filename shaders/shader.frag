#version 330 core


in vec3 Normal; //������ �������� � ������ �������� � ��������� � VBO �.�. ������ GPU

in vec3 FragPos;

//� ��� �������������� (��� ������ �������) ����������
//uniform sampler2D ourTexture1;
//uniform sampler2D ourTexture2;

out vec4 color;

uniform vec3 objectColor; //��� ���� ����� ��������� �� ��������� �� ��� �� ������ �������
uniform vec3 lightColor;

uniform vec3 lightPos;
uniform vec3 viewPos;



void main(void)
{
    float ambientStrength = 0.1f;
    vec3  ambient = ambientStrength  * lightColor; //��� ������� ������� ���������
  
  

    vec3 normal = normalize(Normal);
   

    //��� ����� ������ ����������� �� ������ �� �������������� �� ��������� ������� ������������
    //������� �������� ��������� ������������ ��� ���������� ���� ����� ���������

     //������ ����������� ����� ���������� ����� � ����������
     vec3 lightDir = normalize(lightPos - FragPos);
    //��� ��� �������� ����������� ���������� ���������
    //max ��� ����� ����� ������ ������������� ������������� ����� ���������
     float diff = max(dot(normal, lightDir), 0.0);
    //���������� ���������� ��������� ����������� ������ � ������ ���� ����� ���������
      vec3 diffuse = diff * lightColor;

      //������� �������� ����� �� �� ������� ������� �������� �����������
       float specularStrength = 0.5f;
       vec3 viewDir = normalize(viewPos - FragPos);

       vec3 reflectDir = reflect(-lightDir, normal);

    //32 ����������� �������� �������� ���� ������
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;

     //vec3 result = (ambient + diffuse+ specular) * objectColor;

     //� ambient �� �������� ���� ��������� �����
      vec3 result = (ambient + diffuse + specular) * objectColor;

     color = vec4(result,1.0f);
};