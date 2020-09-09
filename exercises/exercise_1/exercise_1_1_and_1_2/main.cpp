#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// Definition of the simple shader code that will be used as source
const char *vertexShaderSource = "#version 330 core\n"
                                 "layout (location = 0) in vec3 aPos;\n"
                                 "void main()\n"
                                 "{\n"
                                 "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0f);\n"
                                 "}\0";

const char *fragmentShaderSource = "#version 330 core\n"
                                   "out vec4 FragColor;\n"
                                   "void main()\n"
                                   "{\n"
                                   "FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
                                   "}\0";

const char *fragmentShaderAltSource = "#version 330 core\n"
                                   "out vec4 FragColor;\n"
                                   "void main()\n"
                                   "{\n"
                                   "FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
                                   "}\0";

int main(){
    // initialize GLFW with the attributes you want to use for context
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// Create the actual GLFW window
	GLFWwindow* window = glfwCreateWindow(800, 600, "Exercise 1.1 and 1.2", NULL, NULL);
	if(window == NULL) {
	  std::cout << "GLFW Window is null" << std::endl;
	  glfwTerminate();
	  return -1;
	}
	// Make the window you just created the current context
	glfwMakeContextCurrent(window);

	// load GLAD and check it's loaded, otherwise OpenGL won't work
	if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
	    std::cout << "Failed to initialize GLAD" << std::endl;
	    return -1;
    }

	// set the viewport in OpenGL and a resize callback if window size changes
	//glViewport(0, 0, 800, 600);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// Create the shader to be used on the vertices
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	// Check if the Vertex Shader was compiled successfully
	int vertSuccess;
	char vertInfoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vertSuccess);
	if(!vertSuccess) {
	    glGetShaderInfoLog(vertexShader, 512, NULL, vertInfoLog);
	    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << vertInfoLog << std::endl;
	}

	// Create the fragment shader
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

    // Check if the Fragment Shader was compiled successfully
    int fragSuccess;
    char fragInfoLog[512];
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fragSuccess);
    if(!fragSuccess) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, fragInfoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << fragInfoLog << std::endl;
    }

    // Create the alternative fragment shader
    unsigned int fragmentShaderAlt;
    fragmentShaderAlt = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShaderAlt, 1, &fragmentShaderAltSource, NULL);
    glCompileShader(fragmentShaderAlt);

    // Check if the Fragment Shader was compiled successfully
    int fragSuccessAlt;
    char fragInfoLogAlt[512];
    glGetShaderiv(fragmentShaderAlt, GL_COMPILE_STATUS, &fragSuccessAlt);
    if(!fragSuccessAlt) {
        glGetShaderInfoLog(fragmentShaderAlt, 512, NULL, fragInfoLogAlt);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << fragInfoLogAlt << std::endl;
    }

    // Create the Shader Program to link vertex and fragment shaders together
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Check if the linking in the shader program was successful
    int progSuccess;
    char progInfoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &progSuccess);
    if(!progSuccess) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, progInfoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << progInfoLog << std::endl;
    }

    // Create the Shader Program to link vertex and fragment shaders together
    unsigned int shaderProgramAlt;
    shaderProgramAlt = glCreateProgram();
    glAttachShader(shaderProgramAlt, vertexShader);
    glAttachShader(shaderProgramAlt, fragmentShaderAlt);
    glLinkProgram(shaderProgramAlt);

    // Check if the linking in the shader program was successful
    int progSuccessAlt;
    char progInfoLogAlt[512];
    glGetProgramiv(shaderProgramAlt, GL_LINK_STATUS, &progSuccessAlt);
    if(!progSuccessAlt) {
        glGetProgramInfoLog(shaderProgramAlt, 512, NULL, progInfoLogAlt);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << progInfoLogAlt << std::endl;
    }

    // Delete the shader object now that they're linked already
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glDeleteShader(fragmentShaderAlt);

    // vertices definition
    float firstTriangle[] = {
            0.5f, 0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            -0.5f, -0.5f, 0.0f
    };
    float secondTriangle[] = {
            -0.5f, -0.5f, 0.0f,
            -0.7f, -0.5f, 0.0f,
            -0.7f, 0.2f, 0.0f
    };

    // indices definition (for EBO)
    //unsigned int indices[] = {
    //        0, 1, 2,    // First triangle
    //        2, 3, 4     // Second triangle
    //};

    // ..:: VBO & VAO Initialization
    // Create the VBO and VAO
    unsigned int VBOs[2], VAOs[2];
    glGenBuffers(2, VBOs);
    glGenVertexArrays(2, VAOs);

    // Bind first VAO and VBO
    glBindVertexArray(VAOs[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    // Copy the defined vertices in the VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(firstTriangle), firstTriangle, GL_STATIC_DRAW);
    // Enable the vertex Shader on the currently bounded VBO
    glVertexAttribPointer(0,3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Bind second VAO and VBO
    glBindVertexArray(VAOs[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
    // Copy the defined vertices in the VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(secondTriangle), secondTriangle, GL_STATIC_DRAW);
    // Enable the vertex Shader on the currently bounded VBO
    glVertexAttribPointer(0,3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Element Buffer Objet
    //unsigned int EBO;
    //glGenBuffers(1, &EBO);
    // Copy he indices and bind EBO
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Uncomment this to draw in wireframe mode
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// render loop
	while(!glfwWindowShouldClose(window)) {
	    // handle input from user
	    processInput(window);

	    // rendering commands should be here
	    // example: clear the whole screen with a specified color
	    glClearColor(.2f, .3f, .4f, 1.0f);
	    glClear(GL_COLOR_BUFFER_BIT);

        // Use the Shader Program to draw the triangle and bind the VAO to be drawn
        glUseProgram(shaderProgram);
        glBindVertexArray(VAOs[0]);
        // Draw the current VAO
        glDrawArrays(GL_TRIANGLES, 0, 3);
        //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


        // Use the Shader Program to draw the triangle and bind the VAO to be drawn
        glUseProgram(shaderProgramAlt);
        glBindVertexArray(VAOs[1]);
        glDrawArrays(GL_TRIANGLES, 0, 3);

	    // check and call events and swap the buffers
	    glfwSwapBuffers(window);
	    glfwPollEvents();
	}



	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}