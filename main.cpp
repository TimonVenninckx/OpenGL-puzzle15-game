#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shader.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <sstream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
//math library glm
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


enum Directions {
	DIRECTION_UP, DIRECTION_DOWN, DIRECTION_RIGHT, DIRECTION_LEFT,DIRECTION_COUNT
};


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
unsigned int loadTexture(char const* path);
bool try_move(Directions direction);

const unsigned int SCR_WIDTH{ 800 };
const unsigned int SCR_HEIGHT{ 600 };


float deltatime{ 0.0f };
float lastFrame{ 0.0f };


const std::vector<glm::vec3> correctPositions{
	{0.f,6.f,0.f},
	{2.f,6.f,0.f},
	{4.f,6.f,0.f},
	{6.f,6.f,0.f},
	{0.f,4.f,0.f},
	{2.f,4.f,0.f},
	{4.f,4.f,0.f},
	{6.f,4.f,0.f},
	{0.f,2.f,0.f},
	{2.f,2.f,0.f},
	{4.f,2.f,0.f},
	{6.f,2.f,0.f},
	{0.f,0.f,0.f},
	{2.f,0.f,0.f},
	{4.f,0.f,0.f},
};

std::vector<glm::vec3> cubePositions{ 
	{0.f,6.f,0.f},
	{2.f,6.f,0.f},
	{4.f,6.f,0.f},
	{6.f,6.f,0.f},
	{0.f,4.f,0.f},
	{2.f,4.f,0.f},
	{4.f,4.f,0.f},
	{6.f,4.f,0.f},
	{0.f,2.f,0.f},
	{2.f,2.f,0.f},
	{4.f,2.f,0.f},
	{6.f,2.f,0.f},
	{0.f,0.f,0.f},
	{2.f,0.f,0.f},
	{4.f,0.f,0.f},
};

glm::vec3 EmptyTilePos{ 6.f,0.f,0.f };


int main() {
	
	// shuffle
	{
		//local var
		int i = 0;
		while (i < 100) {
			if (try_move(static_cast<Directions>(rand() % DIRECTION_COUNT))) {
				++i;
			}
		}
	}

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "15Game", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);


	stbi_set_flip_vertically_on_load(1);
	Shader cubeShader("shaders/vertexshader.vs","shaders/fragmentshader.fs");;


	float vertices[] = {
		// positions          // texture coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,

		-0.5f,  0.5f,  0.5f, 1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, 1.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f, 0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, 1.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f
	};

	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(VAO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	unsigned int textures[15];
	for (int i{ 0 }; i < 15; ++i) {
		std::ostringstream oss;
		oss << "textures/number" << i + 1 << ".png";
		textures[i] = loadTexture(oss.str().c_str());
	}

	cubeShader.use();
	cubeShader.setInt("ourTexture", 0);

	glEnable(GL_DEPTH_TEST);

	// graphics loop
	while (!glfwWindowShouldClose(window)) {
		float currentFrame{ static_cast<float>(glfwGetTime()) };
		deltatime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		
		//input
		glfwSetKeyCallback(window, key_callback);

		glClearColor(0.2f, 0.3f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		cubeShader.use();

		glm::mat4 projection = glm::perspective(glm::radians(40.f), float(SCR_WIDTH) / (float)SCR_HEIGHT, 0.1f, 100.f);
		glm::mat4 view = glm::mat4(1.0f);
		view = glm::translate(view, glm::vec3(-3.0f,-3.0f,-15.0f));

		glm::mat4 model{ glm::mat4(1.f) };

		cubeShader.setMat4("projection", projection);
		cubeShader.setMat4("view", view);
		cubeShader.setMat4("model", model);
		

		glActiveTexture(GL_TEXTURE0);
		glBindVertexArray(VAO);
		for (int i{ 0 }; i < cubePositions.size(); i++) {
			glBindTexture(GL_TEXTURE_2D, textures[i]);

			model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			cubeShader.setMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		

		glfwSwapBuffers(window);
		glfwPollEvents();
	}



	return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {

	if (key == GLFW_KEY_ESCAPE) {
		glfwSetWindowShouldClose(window, true);
	}
	if (key == GLFW_KEY_D && action == GLFW_PRESS) {
		try_move(DIRECTION_RIGHT);
	}
	if (key == GLFW_KEY_A && action == GLFW_PRESS) {
		try_move(DIRECTION_LEFT);
	}
	if (key == GLFW_KEY_W && action == GLFW_PRESS) {
		try_move(DIRECTION_UP);
	}
	if (key == GLFW_KEY_S && action == GLFW_PRESS) {
		try_move(DIRECTION_DOWN);
	}
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}


unsigned int loadTexture(char const* path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	
	int width, height, nrComponents;
	unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}

void check_if_won() {
	if (EmptyTilePos == glm::vec3(6.f, 0.f, 0.f)) {
		if (cubePositions == correctPositions) {
			std::cout << "YOU WON" << std::endl;
		}
	}
}

bool try_move(Directions direction) {

	switch (direction) {
	case DIRECTION_UP:
		if (EmptyTilePos.y - 2.f >= 0) {
			glm::vec3 swapPosition{ EmptyTilePos.x,EmptyTilePos.y - 2.f,EmptyTilePos.z };
			glm::vec3& iterator{ *std::find_if(cubePositions.begin(),cubePositions.end(),[&](glm::vec3 pos) {return pos == swapPosition; }) };
			std::swap(EmptyTilePos, iterator);
			check_if_won();
			return true;
		}
		return false;
	case DIRECTION_DOWN:
		if (EmptyTilePos.y + 2.f <= 6) {
			glm::vec3 swapPosition{ EmptyTilePos.x,EmptyTilePos.y + 2.f,EmptyTilePos.z };
			glm::vec3& iterator{ *std::find_if(cubePositions.begin(),cubePositions.end(),[&](glm::vec3 pos) {return pos == swapPosition; }) };
			std::swap(EmptyTilePos, iterator);
			check_if_won();
			return true;
		}
		return false;
	case DIRECTION_RIGHT:
		if (EmptyTilePos.x - 2.f >= 0) {
			glm::vec3 swapPosition{ EmptyTilePos.x - 2.f,EmptyTilePos.y,EmptyTilePos.z };
			glm::vec3& iterator{ *std::find_if(cubePositions.begin(),cubePositions.end(),[&](glm::vec3 pos) {return pos == swapPosition; }) };
			std::swap(EmptyTilePos, iterator);
			check_if_won();
			return true;
		}
		return false;
	case DIRECTION_LEFT:
		if (EmptyTilePos.x + 2.f <= 6) {
			glm::vec3 swapPosition{ EmptyTilePos.x + 2.f,EmptyTilePos.y,EmptyTilePos.z };
			glm::vec3& iterator{ *std::find_if(cubePositions.begin(),cubePositions.end(),[&](glm::vec3 pos) {return pos == swapPosition; }) };
			std::swap(EmptyTilePos, iterator);
			check_if_won();
			return true;
		}
		return false;
	}
}
