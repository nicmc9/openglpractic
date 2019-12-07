// learngl.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

#include <SOIL.h>

//glm
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader_m.h"

GLuint VBO, VAO, EBO;

GLuint texture1,texture2;
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
auto cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 upWorld = glm::vec3(0.0f, 1.0f, 0.0f);


GLfloat deltaTime = 0.0f;	// Время, прошедшее между последним и текущим кадром
GLfloat lastFrame = 0.0f;  	// Время вывода последнего кадра

GLfloat yaw = -90.0f;
GLfloat pitch = 0.0f;

bool keys[1024];
//bool firstMouse = true;
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

GLfloat lastX = 400 , lastY = 300;
GLfloat fov = 45.0f;


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (action == GLFW_PRESS)
		keys[key] = true;
	else if (action == GLFW_RELEASE)
		keys[key] = false;


	
	// Когда пользователь нажимает ESC, мы устанавливаем свойство WindowShouldClose в true, 
	// и приложение после этого закроется
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {


	///Замененно на glfwSetCursorPos
	//if (firstMouse) // эта переменная была проинициализирована значением true
	//{
	//	lastX = xpos;
	//	lastY = ypos;
	//	firstMouse = false;
	//}

	
	GLfloat xoffset = xpos - lastX;
	GLfloat yoffset = lastY - ypos; // Обратный порядок вычитания потому что оконные Y-координаты возрастают с верху вниз 
	lastX = xpos;
	lastY = ypos;

	GLfloat sensitivity = 0.05f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
	front.y = sin(glm::radians(pitch));
	front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
	cameraFront = glm::normalize(front);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	if (fov >= 1.0f && fov <= 45.0f)
		fov -= yoffset;
	if (fov <= 1.0f)
		fov = 1.0f;
	if (fov >= 45.0f)
		fov = 45.0f;
}
///Теперь проверки на каждой итерации цикла
void do_movement()
{
	GLfloat cameraSpeed = 5.0f * deltaTime;

	if (keys[GLFW_KEY_W]) {
		auto temp = cameraSpeed * cameraFront;
		std::cout << "temp == vec3(" << temp.x << ", " << temp.y << ", " << temp.z << ") " << '\n';
		std::cout << "cameraPos == vec3(" << cameraPos.x << ", " << cameraPos.y << ", " << cameraPos.z << ") " << '\n';
		cameraPos += temp;
		std::cout << "cameraPos == vec3(" << cameraPos.x << ", " << cameraPos.y << ", " << cameraPos.z << ") " << '\n';

		auto pf = cameraPos + cameraFront;
		std::cout << "pf == vec3(" << pf.x << ", " << pf.y << ", " << pf.z << ") " << '\n';
	}
	if (keys[GLFW_KEY_S]) {
		cameraPos -= cameraSpeed * cameraFront;
	}
	if (keys[GLFW_KEY_A]) {
		auto temp = glm::normalize(glm::cross(cameraFront, upWorld)) * cameraSpeed;
		std::cout << "temp == vec3(" << temp.x << ", " << temp.y << ", " << temp.z << ") " << '\n';
		std::cout << "cameraPos == vec3(" << cameraPos.x << ", " << cameraPos.y << ", " << cameraPos.z << ") " << '\n';
		//Без нормализации скорость была бы постоянной
		cameraPos -= temp;
	}
	if (keys[GLFW_KEY_D]) {
		cameraPos += glm::normalize(glm::cross(cameraFront, upWorld)) * cameraSpeed;
	}

	
}

void init() {

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glEnable(GL_DEPTH_TEST);


	GLfloat vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};
	

	GLint width = 8;
	GLint height = 8;
//	GLubyte* image = SOIL_load_image("brick.jpg", &width, &height, 0, SOIL_LOAD_RGB);
	GLubyte* image = SOIL_load_image("res/container.jpg", &width, &height, 0, SOIL_LOAD_RGB);
	
	
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//Теперь сгенерируем объект текстуры из наших данных
	if (image)
	{
	//	glTextureStorage2D(texture, 4, GL_R8, width, height);
    //glTextureSubImage2D(texture, 0, 0, 0, width, height, GL_RED, GL_UNSIGNED_BYTE, image2);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}

	//SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	GLubyte* image2 = SOIL_load_image("res/awesomeface.png", &width, &height, 0, SOIL_LOAD_RGB);


	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//Теперь сгенерируем объект текстуры из наших данных
	if (image2)
	{
		//	glTextureStorage2D(texture, 4, GL_R8, width, height);
		//glTextureSubImage2D(texture, 0, 0, 0, width, height, GL_RED, GL_UNSIGNED_BYTE, image2);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image2);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}

	//SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	   	
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	//Была неправильная спецификация шаги и другое гавно учись определять
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
			
	 // remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

}


int main()
{
	//Инициализация GLFW
	glfwInit();
	//Настройка GLFW
	//Задается минимальная требуемая версия OpenGL. 
	//Мажорная 
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	//Минорная
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//Установка профайла для которого создается контекст
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//Выключение возможности изменения размера окна
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetKeyCallback(window, key_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetCursorPos(window, lastX, lastY);
	glfwSetScrollCallback(window, scroll_callback);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);

	glViewport(0, 0, width, height);
		   
	init();


	Shader ourShader("shaders/shader.vert", "shaders/shader.frag");



	GLint nrAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;
	//////////////////////////////////////

	// Сверяемся с документацией
	//Явно создаем единичную 
	
	glm::vec3 cubePositions[] = {
  glm::vec3(0.0f,  0.0f,  0.0f),
  glm::vec3(2.0f,  5.0f, -15.0f),
  glm::vec3(-1.5f, -2.2f, -2.5f),
  glm::vec3(-3.8f, -2.0f, -12.3f),
  glm::vec3(2.4f, -0.4f, -3.5f),
  glm::vec3(-1.7f,  3.0f, -7.5f),
  glm::vec3(1.3f, -2.0f, -2.5f),
  glm::vec3(1.5f,  2.0f, -2.5f),
  glm::vec3(1.5f,  0.2f, -1.5f),
  glm::vec3(-1.3f,  1.0f, -1.5f)
	};
	/////////////////////////////////
	///Camera

	///Оказывается важна позиция для LookAt  Ведь это поставщики данныых
	/*
	std::cout << "cameraPos == vec3(" << cameraPos.x << ", " << cameraPos.y << ", " << cameraPos.z << ") " << '\n';

	///Оказывается важна цель для LookAt Ведь это поставщики данныых
	glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);

	glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);

	std::cout <<"cameraDirection == vec3("<< cameraDirection.x << ", " << cameraDirection.y << ", " << cameraDirection.z << ") " << '\n';

	///Оказывается важна цель для LookAt  Ведь это поставщики данныых
	
	glm::vec3 cameraRight = glm::normalize(glm::cross(upWorld, cameraDirection));

	std::cout << "cameraRight == vec3(" << cameraRight.x << ", " << cameraRight.y << ", " << cameraRight.z << ") " << '\n';

	///Остальное вычисления
	glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);

	std::cout << "cameraUp == vec3(" << cameraUp.x << ", " << cameraUp.y << ", " << cameraUp.z << ") " << '\n';

	
	

	glm::mat4 viewM = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f),
									glm::vec3(0.0f, 0.0f, 0.0f),
									glm::vec3(0.0f, 1.0f, 0.0f));
	*/
	//камера фронт это шаг вперед по оси Z
	///Так же это гарантирует что камера всегда смотрит в направлении цели
//	glm::mat4 viewM2 = glm::lookAt(cameraPos,	cameraPos + cameraFront ,upWorld);

	/////////////////////////////////


	GLfloat radius = 15.0f;


	///glm::mat4 view(1.0f);
	///view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));


	 ////////////////////
	

	//Вызвай до использования присвоений в программу
	ourShader.use();
		
	GLint modelLoc = glGetUniformLocation(ourShader.ID, "model");
	GLint viewLoc = glGetUniformLocation(ourShader.ID, "view");
	GLint projectionLoc = glGetUniformLocation(ourShader.ID, "projection");

	

	////////////////////////////////////

	/// Мы не вызывает Uniform но текстура сама попадаешь в шейдер объект Sampler2d
		///Хотя нужную текстуры все-таки забандили
		/// Активируем текстурный блок перед привязкой текстуры
	glActiveTexture(GL_TEXTURE0); 	/// Хотя нулевой блок всегда активирован по умолчанию
	/// Если нужно более одного то начинаем все активировать
	glBindTexture(GL_TEXTURE_2D, texture1);
	glUniform1i(glGetUniformLocation(ourShader.ID, "ourTexture1"), 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glUniform1i(glGetUniformLocation(ourShader.ID, "ourTexture2"), 1);

	

	while (!glfwWindowShouldClose(window))
	{
		

		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glfwPollEvents();
		do_movement();
		///И вот ответ матрицу надо всегда сбрасывать до начальных значений на каждой итерации
		///иначе значения вращения будут прибавляться по нарастающей
		///Так как матрица объявлена здесь то и отправка ее в шейдер пойдет после объявления
		/// Ну и преписывать матрицу в область памяти ГПУ нужно после изменения :))
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		
		//GLfloat camX = sin(glfwGetTime()) * radius;
		//GLfloat camZ = cos(glfwGetTime()) * radius;


		auto viewM2 = glm::lookAt(cameraPos, cameraPos+ cameraFront, upWorld);
		//auto view = glm::lookAt(glm::vec3(camX, 0.0, camZ), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(viewM2));

		glm::mat4  projection = glm::perspective(glm::radians(fov), (GLfloat)width / height, 0.1f, 100.0f);
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

		glBindVertexArray(VAO);

		for (GLuint i = 0; i < 10; i++)
		{
			glm::mat4 model(1.0f);
			model = glm::translate(model, cubePositions[i]);
			GLfloat angle = 20.0f * i;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		glBindVertexArray(0);

		glfwSwapBuffers(window);
		/// Проверяем события и вызываем функции обратного вызова.
		
	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	glfwTerminate();
	return 0;
}
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
