// JOHN ALEXANDER LE ROUX

// User-Defined Headers
// --------------------
#include "config/config.hpp"
#include "shader/shader.hpp"
#include "camera/camera.hpp"

// 3rd Party Library Headers
// -------------------------
// GLAD
#include "../contrib/glad/include/glad/glad.hpp"
// GLFW
#include <GLFW/glfw3.h>
GLFWwindow* window;
// stb
#define STB_IMAGE_IMPLEMENTATION
#include "../contrib/stb/stb_image.hpp"
// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Standard Library Headers
// ------------------------
#include <iostream>
#include <vector>
#include <fstream>



// Shaders
// -------
// Vertex Shader
const char* vertexShaderSource = "#version 330 core\n"
"layout(location = 0) in vec3 aPos;\n"
"layout(location = 1) in vec2 aTexCoord;\n"
"out vec2 TexCoord;\n"
"uniform mat4 model;\n"
"uniform mat4 view;\n"
"uniform mat4 projection;\n"
"void main()\n"
"{\n"
"	gl_Position = projection * view * model * vec4(aPos, 1.0);\n"
"	TexCoord = vec2(aTexCoord.x, aTexCoord.y);\n"
"}\n"
"\0";

// Fragment Shader
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"in vec2 TexCoord;\n"
"// Texture sampler\n"
"uniform sampler2D texture1;\n"
"void main()\n"
"{\n"
"	FragColor = texture(texture1, TexCoord);\n"
"}\n"
"\0";

std::vector<std::vector<int>> map;



// Settings
// --------
// Screen
const unsigned int SCR_WIDTH = 1024; // Default 800 or 1024
const unsigned int SCR_HEIGHT = 768; // Default 600 or 768

// Timing
float deltaTime = 0.0f;	// deltaTime is the time between current frame and last frame
float lastFrame = 0.0f;

// Keyboard
bool KeysProcessed[1024];

// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 45.0f);
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// Map
std::string mapFilePath = "data/maps/map.txt";



// Callback functions
// ------------------
// Whenever the window size changes (by OS or user resize) this callback function executes
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	// Make sure the viewport matches the new window dimensions
	glViewport(0, 0, width, height);
}

// Input callback. Process all input
void processInput(GLFWwindow* window) {
	const float cameraSpeed = 2.5f * deltaTime; // Adjust accordingly

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) // Check if the ESC key was pressed
		glfwSetWindowShouldClose(window, true);            // If so, close the window

	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) // Check if the Q key was pressed
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);    // If so, change draw mode to GL_FILL

	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) // Check if the E key was pressed
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);    // If so, change draw mode to GL_LINE / wireframe

	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) // Check if the C key was pressed
		glfwSetClipboardString(NULL, "Hello this is a clipboard test");


	if ((glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) && (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS) && (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS))
		glfwSetWindowShouldClose(window, true); // If so, close the window          

	if ((glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS) && (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS))
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	if ((glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS) && (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS))
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		camera.ProcessKeyboard(DOWN, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		camera.ProcessKeyboard(UP, deltaTime);
}

// Mouse callback. Whenever the mouse moves, this callback is called
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // Reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

// Scroll callback. Whenever the mouse scroll wheel scrolls, this callback is called
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(static_cast<float>(yoffset));
}



int main(int argc, char* argv[]) {
	// Terminal interaction
	// --------------------
	if (argc >= 2)
	{
		if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0)
		{
			std::cout << "Commands: \n";
			std::cout << "-h            or --help             Help Menu\n";
			std::cout << "-m (filepath) or --map (filepath)   Load custom map\n";
			std::cout << "-v            or --version          Program Version\n";
			std::cout << "-c            or --copyright        Copyright Notice\n";
			return 0;
		}
		else if (strcmp(argv[1], "-m") == 0 || strcmp(argv[1], "--map") == 0)
		{
			mapFilePath = "data/maps/" + std::string(argv[2]);
			std::cout << "New map path: " << mapFilePath << '\n';
		}
		else if (strcmp(argv[1], "-v") == 0 || strcmp(argv[1], "--version") == 0)
		{
			std::cout << "   Name: " << PROJECT_NAME << '\n';
			std::cout << "Version: " << PROJECT_VER << '\n';
			return 0;
		}
		else if (strcmp(argv[1], "-c") == 0 || strcmp(argv[1], "--copyright") == 0)
		{
			std::string licenseText;
			std::ifstream licenseFile("LICENSE");
			if (licenseFile.fail())
			{
				std::cout << "Can NOT find the license! Please DO NOT remove the license from this project!!\n";
			}
			while (std::getline(licenseFile, licenseText))
			{
				std::cout << licenseText << '\n';
			}
			return 0;
		}
		else
		{
			std::cout << "Sorry, \"" << argv[1] << "\" is not a command!\n";
			std::cout << "Please use '--help' for a list of commands\n";
			return 1;
		}
	}
	
	
	
	// Initialize GLFW
	// ---------------
	if (!glfwInit()) {
		std::cerr << "Failed to initialize GLFW" << std::endl;
		return -1;
	}

	// Configure GLFW
	glfwWindowHint(GLFW_SAMPLES, 4);                               // 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);                 // We want OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL
#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);           // To make MacOS happy; should not be needed
#endif

	// Create a window with glfw
	window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OpenGL Windows95Maze!", NULL, NULL);
	if (!window) {
		std::cerr << "Failed to create GLFW window. If you have an Intel GPU, they are not 3.3 compatible." << std::endl;
		glfwTerminate();
		return -1;
	}

	// Set the context to be the window and setup any callback functions
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// Tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);



	// Initialize GLAD; Load all OpenGL pointers
	// -----------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cerr << "Failed to initialize GLAD" << std::endl;
		glfwTerminate();
		return -1;
	}

	// Configure global OpenGL state
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_FRAMEBUFFER_SRGB); // Use sRGB
	glEnable(GL_MULTISAMPLE); // Enable MSAA multisampling
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // https://www.andersriggelsen.dk/glblendfunc.php
	glfwSwapInterval(1); // Turn on vsync for smoother rendering and so OpenGL doesn't draw faster than the monitor refresh rate. Otherwise the program will use 100% CPU and GPU



	// Build and compile our shader program
	// ------------------------------------
	//Shader ourShader(vertexShaderSource, fragmentShaderSource);
	Shader ourShader("data/shaders/cube.vert", "data/shaders/cube.frag");

	// Setup vertex data
	std::vector<float> vertices = {
		// positions         // texture coords
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

		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		 0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		
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
	std::vector<float> quadVertices = {
		// positions         // texture coords
		-0.5f, -0.5f, 0.0f,  0.0f, 0.0f,
		 0.5f, -0.5f, 0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, 0.0f,  1.0f, 1.0f,
		 0.5f,  0.5f, 0.0f,  1.0f, 1.0f,
		-0.5f,  0.5f, 0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, 0.0f,  0.0f, 0.0f,
	};



	// Setup VAO and VBO
	// -----------------
	// Vertex Array Object // Vertex Buffer Object
	unsigned int VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	// Bind the Vertex Array Object first, then set vertex buffer(s), and then configure vertex attributes(s)
	glBindVertexArray(VAO);

	// Copy our vertices array in a buffer for OpenGL to use
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * 3 * sizeof(float), &vertices.front(), GL_STATIC_DRAW); // https://stackoverflow.com/questions/7173494/vbos-with-stdvector
	// GL_STATIC_DRAW: Data will be modified once and used many times
	// GL_DYNAMIC_DRAW: Data will be modified repeatedly and used many times
	// GL_STREAM_DRAW: Data will be modified once and used at most a few times

	// Then set our vertex attributes pointers
	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0); // Please note that we put 5 * sizeof(float) here because we have 3 floats for position and 2 for texture
	glEnableVertexAttribArray(0);
	// Texture coord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Unbind VAO (It's always a good thing to unbind any buffer/array to prevent strange bugs)
	glBindVertexArray(0);

	// Vertex Array Object // Vertex Buffer Object
	unsigned int quadVAO, quadVBO;
	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);

	// Bind the Vertex Array Object first, then set vertex buffer(s), and then configure vertex attributes(s)
	glBindVertexArray(quadVAO);

	// Copy our vertices array in a buffer for OpenGL to use
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, quadVertices.size() * 3 * sizeof(float), &quadVertices.front(), GL_STATIC_DRAW); // https://stackoverflow.com/questions/7173494/vbos-with-stdvector
	// GL_STATIC_DRAW: Data will be modified once and used many times
	// GL_DYNAMIC_DRAW: Data will be modified repeatedly and used many times
	// GL_STREAM_DRAW: Data will be modified once and used at most a few times

	// Then set our vertex attributes pointers
	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0); // Please note that we put 5 * sizeof(float) here because we have 3 floats for position and 2 for texture
	glEnableVertexAttribArray(0);
	// Texture coord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Unbind VAO (It's always a good thing to unbind any buffer/array to prevent strange bugs)
	glBindVertexArray(0);



	// Load and create a texture
	// -------------------------
	unsigned int wallTexture, floorTexture, roofTexture, startTexture, endTexture;

	// wallTexture
	glGenTextures(1, &wallTexture);
	glBindTexture(GL_TEXTURE_2D, wallTexture); // All upcoming GL_TEXTURE_2D operations now have effect on this texture object
	// Set the texture wrapping parameters (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Axis s, t, r are the equivalent of x, y, z in OpenGL
	// GL_REPEAT: The default behavior for textures. Repeats the texture image
	// GL_MIRRORED_REPEAT: The texture image is flipped and then repeated
	// GL_CLAMP_TO_EDGE: The texture image is stretched to fill the edge of the texture
	// GL_CLAMP_TO_BORDER: The texture image is stretched to fill the edge of the texture and the remaining area is filled with a border color

	// Set the texture filtering parameters (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // GL_NEAREST_MIPMAP_NEAREST
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	// GL_NEAREST: The texture image is filtered by a nearest-neighbor algorithm. The texture appears blocky.
	// GL_LINEAR: The texture image is filtered by a bilinear algorithm. The texture appears smooth.
	// GL_NEAREST_MIPMAP_NEAREST: The texture image is filtered by a nearest-neighbor algorithm. The texture appears blocky. The mipmap is also filtered by a nearest-neighbor algorithm.
	// GL_LINEAR_MIPMAP_NEAREST: The texture image is filtered by a bilinear algorithm. The texture appears smooth. The mipmap is also filtered by a nearest-neighbor algorithm.
	// GL_NEAREST_MIPMAP_LINEAR: The texture image is filtered by a nearest-neighbor algorithm. The texture appears blocky. The mipmap is also filtered by a bilinear algorithm.
	// GL_LINEAR_MIPMAP_LINEAR: The texture image is filtered by a bilinear algorithm. The texture appears smooth. The mipmap is also filtered by a bilinear algorithm.

	// Load image, create texture and generate mipmaps
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true); // Tell stb_image.h to flip loaded texture's on the y-axis. The standard OpenGL texture coordinate system is upside down. (Because texture coordinates go from 0 to 1 and not 0 to -1)
	unsigned char* data = stbi_load("data/textures/wall.bmp", &width, &height, &nrChannels, 0);  // container.jpg does not have transparency, so make sure to tell OpenGL the data type is of GL_RGB (3 channels)
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	// floorTexture
	glGenTextures(1, &floorTexture);
	glBindTexture(GL_TEXTURE_2D, floorTexture); // All upcoming GL_TEXTURE_2D operations now have effect on this texture object
	// Set the texture wrapping parameters (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Axis s, t, r are the equivalent of x, y, z in OpenGL
	// GL_REPEAT: The default behavior for textures. Repeats the texture image
	// GL_MIRRORED_REPEAT: The texture image is flipped and then repeated
	// GL_CLAMP_TO_EDGE: The texture image is stretched to fill the edge of the texture
	// GL_CLAMP_TO_BORDER: The texture image is stretched to fill the edge of the texture and the remaining area is filled with a border color

	// Set the texture filtering parameters (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // GL_NEAREST_MIPMAP_NEAREST
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	// GL_NEAREST: The texture image is filtered by a nearest-neighbor algorithm. The texture appears blocky.
	// GL_LINEAR: The texture image is filtered by a bilinear algorithm. The texture appears smooth.
	// GL_NEAREST_MIPMAP_NEAREST: The texture image is filtered by a nearest-neighbor algorithm. The texture appears blocky. The mipmap is also filtered by a nearest-neighbor algorithm.
	// GL_LINEAR_MIPMAP_NEAREST: The texture image is filtered by a bilinear algorithm. The texture appears smooth. The mipmap is also filtered by a nearest-neighbor algorithm.
	// GL_NEAREST_MIPMAP_LINEAR: The texture image is filtered by a nearest-neighbor algorithm. The texture appears blocky. The mipmap is also filtered by a bilinear algorithm.
	// GL_LINEAR_MIPMAP_LINEAR: The texture image is filtered by a bilinear algorithm. The texture appears smooth. The mipmap is also filtered by a bilinear algorithm.

	// Load image, create texture and generate mipmaps
	stbi_set_flip_vertically_on_load(true); // Tell stb_image.h to flip loaded texture's on the y-axis. The standard OpenGL texture coordinate system is upside down. (Because texture coordinates go from 0 to 1 and not 0 to -1)
	data = stbi_load("data/textures/floor.bmp", &width, &height, &nrChannels, 0);  // container.jpg does not have transparency, so make sure to tell OpenGL the data type is of GL_RGB (3 channels)
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	// roofTexture
	glGenTextures(1, &roofTexture);
	glBindTexture(GL_TEXTURE_2D, roofTexture); // All upcoming GL_TEXTURE_2D operations now have effect on this texture object
	// Set the texture wrapping parameters (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Axis s, t, r are the equivalent of x, y, z in OpenGL
	// GL_REPEAT: The default behavior for textures. Repeats the texture image
	// GL_MIRRORED_REPEAT: The texture image is flipped and then repeated
	// GL_CLAMP_TO_EDGE: The texture image is stretched to fill the edge of the texture
	// GL_CLAMP_TO_BORDER: The texture image is stretched to fill the edge of the texture and the remaining area is filled with a border color

	// Set the texture filtering parameters (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // GL_NEAREST_MIPMAP_NEAREST
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	// GL_NEAREST: The texture image is filtered by a nearest-neighbor algorithm. The texture appears blocky.
	// GL_LINEAR: The texture image is filtered by a bilinear algorithm. The texture appears smooth.
	// GL_NEAREST_MIPMAP_NEAREST: The texture image is filtered by a nearest-neighbor algorithm. The texture appears blocky. The mipmap is also filtered by a nearest-neighbor algorithm.
	// GL_LINEAR_MIPMAP_NEAREST: The texture image is filtered by a bilinear algorithm. The texture appears smooth. The mipmap is also filtered by a nearest-neighbor algorithm.
	// GL_NEAREST_MIPMAP_LINEAR: The texture image is filtered by a nearest-neighbor algorithm. The texture appears blocky. The mipmap is also filtered by a bilinear algorithm.
	// GL_LINEAR_MIPMAP_LINEAR: The texture image is filtered by a bilinear algorithm. The texture appears smooth. The mipmap is also filtered by a bilinear algorithm.

	// Load image, create texture and generate mipmaps
	stbi_set_flip_vertically_on_load(true); // Tell stb_image.h to flip loaded texture's on the y-axis. The standard OpenGL texture coordinate system is upside down. (Because texture coordinates go from 0 to 1 and not 0 to -1)
	data = stbi_load("data/textures/ceiling2.bmp", &width, &height, &nrChannels, 0);  // container.jpg does not have transparency, so make sure to tell OpenGL the data type is of GL_RGB (3 channels)
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	// startTexture
	glGenTextures(1, &startTexture);
	glBindTexture(GL_TEXTURE_2D, startTexture); // All upcoming GL_TEXTURE_2D operations now have effect on this texture object
	// Set the texture wrapping parameters (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Axis s, t, r are the equivalent of x, y, z in OpenGL
	// GL_REPEAT: The default behavior for textures. Repeats the texture image
	// GL_MIRRORED_REPEAT: The texture image is flipped and then repeated
	// GL_CLAMP_TO_EDGE: The texture image is stretched to fill the edge of the texture
	// GL_CLAMP_TO_BORDER: The texture image is stretched to fill the edge of the texture and the remaining area is filled with a border color

	// Set the texture filtering parameters (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // GL_NEAREST_MIPMAP_NEAREST
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	// GL_NEAREST: The texture image is filtered by a nearest-neighbor algorithm. The texture appears blocky.
	// GL_LINEAR: The texture image is filtered by a bilinear algorithm. The texture appears smooth.
	// GL_NEAREST_MIPMAP_NEAREST: The texture image is filtered by a nearest-neighbor algorithm. The texture appears blocky. The mipmap is also filtered by a nearest-neighbor algorithm.
	// GL_LINEAR_MIPMAP_NEAREST: The texture image is filtered by a bilinear algorithm. The texture appears smooth. The mipmap is also filtered by a nearest-neighbor algorithm.
	// GL_NEAREST_MIPMAP_LINEAR: The texture image is filtered by a nearest-neighbor algorithm. The texture appears blocky. The mipmap is also filtered by a bilinear algorithm.
	// GL_LINEAR_MIPMAP_LINEAR: The texture image is filtered by a bilinear algorithm. The texture appears smooth. The mipmap is also filtered by a bilinear algorithm.

	// Load image, create texture and generate mipmaps
	stbi_set_flip_vertically_on_load(true); // Tell stb_image.h to flip loaded texture's on the y-axis. The standard OpenGL texture coordinate system is upside down. (Because texture coordinates go from 0 to 1 and not 0 to -1)
	data = stbi_load("data/textures/start2.png", &width, &height, &nrChannels, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	// endTexture
	glGenTextures(1, &endTexture);
	glBindTexture(GL_TEXTURE_2D, endTexture); // All upcoming GL_TEXTURE_2D operations now have effect on this texture object
	// Set the texture wrapping parameters (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Axis s, t, r are the equivalent of x, y, z in OpenGL
	// GL_REPEAT: The default behavior for textures. Repeats the texture image
	// GL_MIRRORED_REPEAT: The texture image is flipped and then repeated
	// GL_CLAMP_TO_EDGE: The texture image is stretched to fill the edge of the texture
	// GL_CLAMP_TO_BORDER: The texture image is stretched to fill the edge of the texture and the remaining area is filled with a border color

	// Set the texture filtering parameters (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // GL_NEAREST_MIPMAP_NEAREST
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	// GL_NEAREST: The texture image is filtered by a nearest-neighbor algorithm. The texture appears blocky.
	// GL_LINEAR: The texture image is filtered by a bilinear algorithm. The texture appears smooth.
	// GL_NEAREST_MIPMAP_NEAREST: The texture image is filtered by a nearest-neighbor algorithm. The texture appears blocky. The mipmap is also filtered by a nearest-neighbor algorithm.
	// GL_LINEAR_MIPMAP_NEAREST: The texture image is filtered by a bilinear algorithm. The texture appears smooth. The mipmap is also filtered by a nearest-neighbor algorithm.
	// GL_NEAREST_MIPMAP_LINEAR: The texture image is filtered by a nearest-neighbor algorithm. The texture appears blocky. The mipmap is also filtered by a bilinear algorithm.
	// GL_LINEAR_MIPMAP_LINEAR: The texture image is filtered by a bilinear algorithm. The texture appears smooth. The mipmap is also filtered by a bilinear algorithm.

	// Load image, create texture and generate mipmaps
	stbi_set_flip_vertically_on_load(true); // Tell stb_image.h to flip loaded texture's on the y-axis. The standard OpenGL texture coordinate system is upside down. (Because texture coordinates go from 0 to 1 and not 0 to -1)
	data = stbi_load("data/textures/fin.png", &width, &height, &nrChannels, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);



	// Tell OpenGL for each sampler to which texture unit it belongs to (only has to be done once)
	// -------------------------------------------------------------------------------------------
	// Activate the shader
	ourShader.use();
	// Either set it via the texture class
	ourShader.setInt("wallTexture", 0);
	ourShader.setInt("floorTexture", 1);
	ourShader.setInt("roofTexture", 2);
	ourShader.setInt("startTexture", 3);
	ourShader.setInt("endTexture", 4);
	// Or set it manually like so:
	//glUniform1i(glGetUniformLocation(ourShader.ID, "wallTexture"), 0);
	//glUniform1i(glGetUniformLocation(ourShader.ID, "floorTexture"), 1);
	//glUniform1i(glGetUniformLocation(ourShader.ID, "roofTexture"), 2);



	// Load map
	// https://www.guru99.com/cpp-file-read-write-open.html
	std::string mapData;
	std::ifstream mapFile;

	mapFile.open(mapFilePath, std::ios::in);
	std::cout << "Map file: \"" << mapFilePath;

	if (!mapFile) {
		std::cout << "\" DOES NOT EXIST!\n";
		std::cout << "ERROR::MAP::FILE_NOT_SUCCESFULLY_READ" << std::endl;

		std::cout << "Assuming no map passed...\n" << "Using empty map...\n" << "\n -- --------------------------------------------------- -- " << std::endl;
		map = {
			{1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
		};
	}
	else
		std::cout << "\"\n";

	std::cout << "Map data: \n";
	while (std::getline(mapFile, mapData)) {
		std::vector<int> row;

		for (char& c : mapData) {
			//if (c != ',' && c != ' ') {
			if (c == '0' || c == '1' || c == '2' || c == '3') { // This will remove anything that is not 0, 1, 2 or 3, aka keeping the map data
				row.push_back((int)c - 48);
			}
		}
		
		map.push_back(row);
	}

	for (std::vector<int>& row : map) {
		for (int& c : row) {
			std::cout << c << ' ';
		}

		std::cout << '\n';
	}
	std::cout << "Map size: " << map.size() << "x" << map[0].size() << std::endl;
	


	// Create the delay
	int frameDelay = 25;
	std::vector<glm::vec3> lastCamPos;
	for(int i = 0; i < frameDelay; i++)
		lastCamPos.push_back(camera.Position);



	// Render loop
	// -----------
	while (!glfwWindowShouldClose(window)) {
		// Per-frame time logic
		// --------------------
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;



		// Input
		// -----
		processInput(window);



		// Render
		// ------
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // a=x/255 where x is the RGB value and a is your desired result from 0 to 1
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear color and depth buffer

		// Bind textures on corresponding texture units
		glActiveTexture(GL_TEXTURE0); // Activate the texture unit before binding

		// Projection matrix
		glm::mat4 projection = glm::mat4(1.0f); // Make sure to initialize matrix to identity matrix first
		projection = glm::perspective(glm::radians(camera.Zoom), static_cast<float>(SCR_WIDTH) / static_cast<float>(SCR_HEIGHT), 0.1f, 50.0f);
		ourShader.setMat4("projection", projection); // Note: Currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.

		// View transformation
		glm::mat4 view = glm::mat4(1.0f); // Make sure to initialize matrix to identity matrix first
		view = camera.GetViewMatrix();
		ourShader.setMat4("view", view);

		// Activate shader
		ourShader.use();

		// Bind the triangle VAO
		glBindVertexArray(VAO);
		
		// Render walls
		glBindTexture(GL_TEXTURE_2D, wallTexture);
		for (int j = 0; j < map.size(); j++) {
			for (int i = 0; i < map[0].size(); i++) {
				if (map[j][i] == 1) {
					glm::mat4 model = glm::mat4(1.0f);
					model = glm::translate(model, glm::vec3(i, 0, j));
					ourShader.setMat4("model", model);
					glDrawArrays(GL_TRIANGLES, 0, 36);
				}
			}
		}

		// Render floor
		glBindTexture(GL_TEXTURE_2D, floorTexture);
		glm::vec3 floorPosition = glm::vec3(0.0f, -1.0f, 0.0f);
		for (int i = 0; i < map.size(); i++) {
			floorPosition.z = i;
			for (int i = 0; i < map[0].size(); i++) {
				// Calculate the model matrix for each object and pass it to shader before drawing
				glm::mat4 model = glm::mat4(1.0f);
				floorPosition.x = i;
				model = glm::translate(model, floorPosition);
				ourShader.setMat4("model", model);

				glDrawArrays(GL_TRIANGLES, 0, 36);
			}
		}

		// Render roof
		glBindTexture(GL_TEXTURE_2D, roofTexture);
		glm::vec3 roofPosition = glm::vec3(0.0f, 1.0f, 0.0f);
		for (int i = 0; i < map.size(); i++) {
			roofPosition.z = i;
			for (int i = 0; i < map[0].size(); i++) {
				// Calculate the model matrix for each object and pass it to shader before drawing
				glm::mat4 model = glm::mat4(1.0f);
				roofPosition.x = i;
				model = glm::translate(model, roofPosition);
				ourShader.setMat4("model", model);

				glDrawArrays(GL_TRIANGLES, 0, 36);
			}
		}

		// Bind the quadVAO
		glBindVertexArray(quadVAO);

		// Render start quad
		glBindTexture(GL_TEXTURE_2D, startTexture);
		for (int j = 0; j < map.size(); j++) {
			for (int i = 0; i < map[0].size(); i++) {
				if (map[j][i] == 2) {
					glm::mat4 model = glm::mat4(1.0f);
					model = glm::translate(model, glm::vec3(i, 0, j));
					model = glm::inverse(glm::lookAt(glm::vec3(i, 0, j), lastCamPos[0], glm::vec3(0.0f, 1.0f, 0.0f)));
					model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
					ourShader.setBool("transparent", true);
					ourShader.setMat4("model", model);
					glDrawArrays(GL_TRIANGLES, 0, 6);
					ourShader.setBool("transparent", false);
				}
			}
		}
		lastCamPos.erase(lastCamPos.begin());
		lastCamPos.push_back(camera.Position);

		// Render end quad
		glBindTexture(GL_TEXTURE_2D, endTexture);
		for (int j = 0; j < map.size(); j++) {
			for (int i = 0; i < map[0].size(); i++) {
				if (map[j][i] == 3) {
					glm::mat4 model = glm::mat4(1.0f);
					model = glm::translate(model, glm::vec3(i, 0, j));
					model = glm::rotate(model, glm::radians(static_cast<float>(glfwGetTime()) * 150.0f), glm::vec3(0.0f, 1.0f, 0.0f));
					ourShader.setBool("transparent", true);
					ourShader.setMat4("model", model);
					glDrawArrays(GL_TRIANGLES, 0, 6);
					ourShader.setBool("transparent", false);
				}
			}
		}

		// Unbind VAO (It's always a good thing to unbind any buffer/array to prevent strange bugs)
		glBindVertexArray(0);



		// Swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------
		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	// Clean up
	// --------
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteTextures(1, &wallTexture);
	glDeleteTextures(1, &floorTexture);
	glDeleteTextures(1, &roofTexture);
	glDeleteProgram(ourShader.ID);

	// Terminate GLFW, clearing any resources allocated by GLFW
	glfwTerminate();
	return 0;
}

#ifdef _WIN32 // _WIN32 = Windows32,  _WIN64 = Windows64,  __APPLE__ = MacOS,  __linux__ = Linux,  __ANDROID__ = Android. https://blog.kowalczyk.info/article/j/guide-to-predefined-macros-in-c-compilers-gcc-clang-msvc-etc..html
#ifndef _DEBUG // Project properties > Linker > System > Subsystem to "Windows" (only for all Release builds) will use WinMain as the start of program and hide the console
#define nominmax
#include <Windows.h>
int CALLBACK WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ PSTR szCmdLine, _In_ int iCmdShow) {
	main(__argc, __argv); // __argc, __argv, __wargv: https://docs.microsoft.com/en-us/cpp/c-runtime-library/argc-argv-wargv?view=msvc-170
	//MessageBox(NULL, TEXT("Body Text"), TEXT("Title"), 0);
	return 0;
}
#endif
#endif // Another option to hide window without WinMain: https://stackoverflow.com/questions/18260508/c-how-do-i-hide-a-console-window-on-startup