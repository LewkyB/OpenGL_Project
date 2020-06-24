#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <windows.h>

// used to allow for window resizing
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void processInput(GLFWwindow* window);

const char* vertex_shader_source = "#version 330 core\n"
	"layout (location = 0) in vec3 aPos;\n"
	"void main()\n"
	"{\n"
	"	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
	"}\0";

const char* fragment_shader_orange_source = "#version 330 core\n"
	"out vec4 FragColor;\n"
	"void main()\n"
	"{\n"
	"	FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
	"}\0";

const char* fragment_shader_yellow_source = "#version 330 core\n"
	"out vec4 FragColor;\n"
	"void main()\n"
	"{\n"
	"	FragColor = vec4(255.0f, 255.0f, 0.0f, 1.0f);\n"
	"}\0";
int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(800, 600, "Luke's Window", NULL, NULL);

	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}


	// build and compile shader program
	// --------------------------------

	// error checking variables
	int success;
	char infoLog[512];

	// vertex shader orange
	unsigned int vertex_shader_orange;
	vertex_shader_orange = glCreateShader(GL_VERTEX_SHADER);

	glShaderSource(vertex_shader_orange, 1, &vertex_shader_source, NULL);
	glCompileShader(vertex_shader_orange);

	// error check vertex shader orange

	glGetShaderiv(vertex_shader_orange, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex_shader_orange, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::ORANGE::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// vertex shader yellow
	unsigned int vertex_shader_yellow;

	vertex_shader_yellow = glCreateShader(GL_VERTEX_SHADER);

	glShaderSource(vertex_shader_yellow, 1, &vertex_shader_source, NULL);
	glCompileShader(vertex_shader_yellow);

	// error check vertex shader yellow
	glGetShaderiv(vertex_shader_yellow, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex_shader_yellow, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::YELLOW::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// fragment shader orange
	unsigned int fragment_shader_orange;
	fragment_shader_orange = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader_orange, 1, &fragment_shader_orange_source, NULL);
	glCompileShader(fragment_shader_orange);

	// error check fragment shader
	glGetShaderiv(fragment_shader_orange, GL_COMPILE_STATUS, &success);
	if (!success)
		{
			glGetShaderInfoLog(fragment_shader_orange, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::FRAGMENT::ORANGE::COMPILATION_FAILED\n" << infoLog << std::endl;
		}

	// fragment shader yellow
	unsigned int fragment_shader_yellow;
	fragment_shader_yellow = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader_yellow, 1, &fragment_shader_yellow_source, NULL);
	glCompileShader(fragment_shader_yellow);

	// error check fragment shader
	glGetShaderiv(fragment_shader_yellow, GL_COMPILE_STATUS, &success);
	if (!success)
		{
			glGetShaderInfoLog(fragment_shader_yellow, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::FRAGMENT::YELLOW::COMPILATION_FAILED\n" << infoLog << std::endl;
		}

	// link shaders

	// link orange
	unsigned int shader_program_orange;

	shader_program_orange = glCreateProgram();

	glAttachShader(shader_program_orange, vertex_shader_orange);
	glAttachShader(shader_program_orange, fragment_shader_orange);
	glLinkProgram(shader_program_orange);

	// error check link orange shaders
	glGetShaderiv(shader_program_orange, GL_COMPILE_STATUS, &success);
	if (!success)
		{
			glGetShaderInfoLog(fragment_shader_orange, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::PROGRAM::ORANGE::COMPILATION_FAILED\n" << infoLog << std::endl;
		}
	
	// link yellow
	unsigned int shader_program_yellow;
	
	shader_program_yellow = glCreateProgram();

	glAttachShader(shader_program_yellow, vertex_shader_yellow);
	glAttachShader(shader_program_yellow, fragment_shader_yellow);
	glLinkProgram(shader_program_yellow);

	// error check link orange shaders
	glGetShaderiv(shader_program_yellow, GL_COMPILE_STATUS, &success);
	if (!success)
		{
			glGetShaderInfoLog(fragment_shader_yellow, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::PROGRAM::YELLOW::COMPILATION_FAILED\n" << infoLog << std::endl;
		}

	// delete shaders after linking
	glDeleteShader(vertex_shader_orange);
	glDeleteShader(fragment_shader_orange);

	glDeleteShader(vertex_shader_yellow);
	glDeleteShader(fragment_shader_yellow);


	// set up vertex data, buffers, and configure vertex attributes
	// ------------------------------------------------------------

	float triangle1[] = {
		// triangle 1
		-0.0f, 0.5f, 0.0f,
		-0.0f, 0.1f, 0.0f,
		-0.9f, 0.1f, 0.0f,
	};
	float triangle2[] = {
		// triangle 2
		0.0f, 0.5f, 0.0f,
		0.0f, 0.1f, 0.0f,
		0.9f, 0.1f, 0.0f
	};

	// variables
	unsigned int VAOs[2], VBOs[2];
	
	// generate buffers
	glGenVertexArrays(2, VAOs);
	glGenBuffers(2, VBOs);

	// TRIANGLE 1
	// bind VAO
	glBindVertexArray(VAOs[0]);
	// copy vertices array to buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle1), triangle1, GL_STATIC_DRAW);
	// set vertex attribute pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// TRIANGE 2
	// bind VAO
	glBindVertexArray(VAOs[1]);
	// copy vertices array to buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle2), triangle2, GL_STATIC_DRAW);
	// set vertex attribute pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// uncomment to change from fill to wireframe
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window)) // run window until close it pressed?
	{
		// input
		processInput(window);

		// rendering commands here

		// background window color
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// TRIANGLE 1
		glUseProgram(shader_program_orange);
		glBindVertexArray(VAOs[0]);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// TRIANGLE 2
		glUseProgram(shader_program_yellow);
		glBindVertexArray(VAOs[1]);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);
		glBindVertexArray(0);

		// check and call events and swap the buffers
		glfwSwapBuffers(window); // used to draw to screen
		glfwPollEvents(); // the event this one is looking for is pressing the close button

		Sleep(1);
	}

	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}
