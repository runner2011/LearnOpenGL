#define STB_IMAGE_IMPLEMENTATION

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <LearnOpenGL/Shader_s.h>
#include <LearnOpenGL/Model.h>
#include <LearnOpenGL/camera.h>
#include <LearnOpenGL/HelperLibrary.h>
#include "main.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);


// link shaders
unsigned int shaderProgram;



float mix_tex1 = 0.8f;

int windowWidth = 2160;
int windowHeight = 1380;

// camera
Camera camera;
float lastX = windowWidth / 2.0f;
float lastY = windowHeight / 2.0f;
bool firstMouse = true;

// timing
double deltaTime = 0.0f;	// time between current frame and last frame


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
	if(glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		camera.ProcessKeyboard(UP, deltaTime);
	if(glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		camera.ProcessKeyboard(DOWN, deltaTime);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if(firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}


void SetupTexture(const char* path, unsigned int& referenceID, GLint internalFormat, GLenum format)
{
	glGenTextures(1, &referenceID);
	glBindTexture(GL_TEXTURE_2D, referenceID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set the texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		//glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture " + *path << std::endl;
	}
	stbi_image_free(data);
}


void SetupShader(Shader& shader, glm::vec3 ModelOffset, glm::vec3 lightPos, float RotateAngle = 0.f, glm::vec3 scale = glm::vec3(1.f))
{
	//model
	glm::mat4 model = glm::mat4(1.0f);

	//view
	glm::mat4 view = camera.GetViewMatrix();
	view = glm::rotate(view, AngleToRadians(RotateAngle), glm::vec3(0.f, 1.f, 0.f));
	view = glm::translate(view, ModelOffset);
	view = glm::scale(view, scale);
	
	//projection
	glm::mat4 projection;
	projection = glm::perspective(glm::radians(camera.Zoom), (float)windowWidth / windowHeight, 0.1f, 1000000.f);

	shader.setMatrix4("model", glm::value_ptr(model));
	shader.setMatrix4("view", glm::value_ptr(view));
	shader.setMatrix4("projection", glm::value_ptr(projection));

	float lightR = 1.f;
	float lightG = 1.f;
	float lightB = 1.f;

	shader.setFloat3("objectColor", 1.f, 1.f, 1.f);
	shader.setFloat3("lightColor", lightR, lightG, lightB);
	shader.setVec3("lightPos", lightPos);
	shader.setVec3("viewPos", camera.Position);

}

void RotateModel(Shader& shader, float angle, glm::vec3 axis)
{
	glm::mat4 model;
	shader.getMatrix4("model", glm::value_ptr(model));
	model = glm::rotate(model, glm::radians(angle), axis);
	shader.setMatrix4("model", glm::value_ptr(model));
}

void TranslateModel(Shader& shader, glm::vec3 offset)
{
	glm::mat4 model;
	shader.getMatrix4("model", glm::value_ptr(model));
	model = glm::translate(model, offset);
	shader.setMatrix4("model", glm::value_ptr(model));
}

Model SetupApplicationData(const string& modelPath)
{
	// load models
	// -----------
	Model model(modelPath);
	return model;


	//wireframe mode
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	//glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	//glBindVertexArray(0);
}

///// Game DATA

// planets
std::vector<glm::vec3> planetsLocations;
std::vector<glm::vec3> planetsRadiuses;

// asteroids
std::vector<glm::vec3> asteroidOffsets;
std::vector<float> asteroidAngles;
std::vector<float> asteroidSpeeds;


//////////////


int main(int argc, char** argv)
{
	long fpscount = 0;
	double lastFrameTime = 0;
	const char* _shaderpath = "../Source";
	if (argc > 1 && argv[1] != "")
	{
		_shaderpath = argv[1];
	}
	
	// glfw: initialize and configure
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw window creation
	GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(0);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glad: load all OpenGL function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// #Runner2011 #fix tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
	//stbi_set_flip_vertically_on_load(true);

	// configure global OpenGL state
	// -------------------------------------------------------------------------
	glEnable(GL_DEPTH_TEST);
	// END configure global OpenGL state

	std::string shaderPathStr = _shaderpath;
	Shader lightingShader((shaderPathStr+ std::string("/lighting.vs")).c_str(), (shaderPathStr + std::string("/lighting.fs")).c_str());
	Shader lightingShader1((shaderPathStr+ std::string("/10.3.asteroids.vs")).c_str(), (shaderPathStr + std::string("/10.3.asteroids.fs")).c_str());
	
	// light position
	float a = 100;
	glm::vec3 lightPos(-1.2f* a, 2.5f* a, 0/*-6.0f*a*/);

	///// Init game data
	Model planet = SetupApplicationData("../Res/planet/planet.obj");
	Model asteriod = SetupApplicationData("../Res/rock/rock.obj");

	int planetAmount = 10;
	float SpaceScale = 10000.f;
	float PlanetMinRadius = 0.5f;
	float PlanetMaxRadius = 20.f;
	
	planetsLocations.push_back(glm::vec3(0.f));
	planetsRadiuses.push_back(glm::vec3(1.f));

	for (int i = 1; i < planetAmount; i++)
	{
		glm::vec3 offset = glm::vec3(0.f + RandomFloat(-SpaceScale, SpaceScale), 0.f + RandomFloat(-SpaceScale, SpaceScale),  0.f + RandomFloat(-SpaceScale, SpaceScale));
		planetsLocations.push_back(offset);
		float r = RandomFloat(PlanetMinRadius, PlanetMaxRadius);
		planetsRadiuses.push_back(glm::vec3(1.f, 1.f, 1.f) * r);
		//printf("planet %d: %f %f %f\n", i, offset.x, offset.y, offset.z);
		//printf("planetx %d: %f %f %f\n", i, planetsRadiuses[i].x, planetsRadiuses[i].y, planetsRadiuses[i].z);
	}

	/////// instance

	unsigned int amount = 1000;
    glm::mat4* modelMatrices;
    modelMatrices = new glm::mat4[amount];
	srand(static_cast<unsigned int>(glfwGetTime())); // initialize random seed
    float radius = 150.0;
    float offset = 25.0f;
    for (unsigned int i = 0; i < amount; i++)
    {
        glm::mat4 model = glm::mat4(1.0f);
        // 1. translation: displace along circle with 'radius' in range [-offset, offset]
        float angle = (float)i / (float)amount * 360.0f;
        float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        float x = sin(angle) * radius + displacement;
        displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        float y = displacement * 0.4f; // keep height of asteroid field smaller compared to width of x and z
        displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        float z = cos(angle) * radius + displacement;
        model = glm::translate(model, glm::vec3(x, y, z));

        // 2. scale: Scale between 0.05 and 0.25f
        float scale = static_cast<float>((rand() % 20) / 100.0 + 0.05);
        model = glm::scale(model, glm::vec3(scale));

        // 3. rotation: add random rotation around a (semi)randomly picked rotation axis vector
        float rotAngle = static_cast<float>((rand() % 360));
        model = glm::rotate(model, glm::radians(rotAngle), glm::vec3(0.4f, 0.6f, 0.8f));

        // 4. now add to list of matrices
        modelMatrices[i] = model;
    }

	// configure instanced array
    // -------------------------
    unsigned int buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), &modelMatrices[0], GL_STATIC_DRAW);

	// set transformation matrices as an instance vertex attribute (with divisor 1)
    // note: we're cheating a little by taking the, now publicly declared, VAO of the model's mesh(es) and adding new vertexAttribPointers
    // normally you'd want to do this in a more organized fashion, but for learning purposes this will do.
    // -----------------------------------------------------------------------------------------------------------------------------------
    for (unsigned int i = 0; i < asteriod.meshes.size(); i++)
    {
        unsigned int VAO = asteriod.meshes[i].VAO;
        glBindVertexArray(VAO);
        // set attribute pointers for matrix (4 times vec4)
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
        glEnableVertexAttribArray(5);
        glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
        glEnableVertexAttribArray(6);
        glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

        glVertexAttribDivisor(3, 1);
        glVertexAttribDivisor(4, 1);
        glVertexAttribDivisor(5, 1);
        glVertexAttribDivisor(6, 1);

        glBindVertexArray(0);
    }
	
	
	camera = Camera(glm::vec3(120.0f, 300.0f, 510.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.f, -35.f);

	// render loop
	while (!glfwWindowShouldClose(window))
	{
		double currentFrame = glfwGetTime();
		double lastTitleUpdateTime = 0.0;

		deltaTime = currentFrame - lastFrameTime;
		if (currentFrame - lastFrameTime > 0.0001f)
		{
			double fps = 1 / deltaTime;
			lastFrameTime = currentFrame;

			char title[256];
			if (currentFrame - lastTitleUpdateTime > 0.01f)
			{
				std::snprintf(title, sizeof(title), "FPS: %.2f", fps);
				glfwSetWindowTitle(window, title);
				lastTitleUpdateTime = currentFrame;

				//input
				processInput(window);
			}

			// rendering commands here
			glClearColor(0.f, 0.f, 0.f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			// bind texture
			//glBindTexture(GL_TEXTURE_2D, texture);

			// bind textures on corresponding texture units
			/*glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture1);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, texture2);*/

			lightingShader.use(); // don't forget to activate/use the shader before setting uniforms!
			for (int i = 0; i < planetAmount; i++)
			{
				SetupShader(lightingShader, planetsLocations[i], lightPos, 0.f, planetsRadiuses[i] * 1.f);
				planet.Draw(lightingShader);
			}

			// configure transformation matrices
			
			lightingShader1.use();
			SetupShader(lightingShader1, glm::vec3(), glm::vec3(), 0.f, glm::vec3(1.f));

			 // draw meteorites
			lightingShader1.setInt("texture_diffuse1", 0);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, asteriod.textures_loaded[0].id); // note: we also made the textures_loaded vector public (instead of private) from the model class.
			for (unsigned int i = 0; i < asteriod.meshes.size(); i++)
			{
				glBindVertexArray(asteriod.meshes[i].VAO);
				glDrawElementsInstanced(GL_TRIANGLES, static_cast<unsigned int>(asteriod.meshes[i].indices.size()), GL_UNSIGNED_INT, 0, amount);
				glBindVertexArray(0);
			}



			// check and call events and swap the buffers
			glfwSwapBuffers(window);
			glfwPollEvents();
		}
	}

	glfwTerminate();
	return 0;

}
