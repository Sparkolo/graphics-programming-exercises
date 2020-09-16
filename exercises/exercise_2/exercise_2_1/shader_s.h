#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader {
public:
    unsigned int ID; // program ID

    // constructor to read and build the shaders
    Shader(const char* vertexPath, const char* fragmentPath) {
        // Retrieve the vertex/fragment source code from filePath
        std::string vertexCode, fragmentCode;
        std::ifstream vShaderFile, fShaderFile;
        // Ensure that ifstream objects can throw exceptions
        vShaderFile.exceptions( std::ifstream::failbit | std::ifstream::badbit);
        fShaderFile.exceptions( std::ifstream::failbit | std::ifstream::badbit);
        try {
            // Open the files
            vShaderFile.open(vertexPath);
            fShaderFile.open(fragmentPath);
            std::stringstream vShaderStream, fShaderStream;
            // Read the file's buffer content into streams
            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();
            // Close file handlers
            vShaderFile.close();
            fShaderFile.close();
            // Convert the stream into a string
            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();
        }
        catch (std::ifstream::failure e) {
            std::cout << "ERROR::SHADER::FILE_READ_UNSUCCESSFUL" << std::endl;
        }
        // Convert from string to const char* usable by OpenGL
        const char* vShaderCode = vertexCode.c_str();
        const char* fShaderCode = fragmentCode.c_str();

        // Compiler the shaders
        unsigned int vertex, fragment;

        // Compile vertex shader
        createShader(vertex, GL_VERTEX_SHADER, vShaderCode);
        // Compile fragment shader
        createShader(fragment, GL_FRAGMENT_SHADER, fShaderCode);

        // Create the Shader Program
        ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);
        glLinkProgram(ID);
        // Print the linking errors, if any
        checkCompileErrors(ID, "PROGRAM");

        // Delete the shaders, they are linked already in the program so we don't need them anymore
        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }

    // Use/activate this shader
    void use() {
        glUseProgram(ID);
    }
    // Setters for uniforms
    void setBool(const std::string &name, bool value) const {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), (int) value);
    }
    void setInt(const std::string &name, int value) const {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    }
    void setFloat(const std::string &name, float value) const {
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
    }
    void setVec2f(const std::string &name, float first, float second) const {
        glUniform2f(glGetUniformLocation(ID, name.c_str()), first, second);
    }
    void setVec3f(const std::string &name, float first, float second, float third) const {
        glUniform3f(glGetUniformLocation(ID, name.c_str()), first, second, third);
    }
    void setVec4f(const std::string &name, float first, float second, float third, float fourth) const {
        glUniform4f(glGetUniformLocation(ID, name.c_str()), first, second, third, fourth);
    }

private:
    // Utility function to create a vertex or fragment shader and check it compiles correctly
    void createShader(unsigned int &shader, GLenum shaderType, const char* shaderCode) {
        shader = glCreateShader(shaderType);
        glShaderSource(shader, 1, &shaderCode, NULL);
        glCompileShader(shader);
        switch(shaderType) {
            case GL_VERTEX_SHADER: checkCompileErrors(shader, "VERTEX");
                break;
            case GL_FRAGMENT_SHADER: checkCompileErrors(shader, "FRAGMENT");
                break;
            default:
                break;
        }
    }

    // Utility function to check for compiling and linking errors in the shader program
    void checkCompileErrors(unsigned int shader, std::string type) {
        int success;
        char infoLog[1024];
        if (type != "PROGRAM")
        {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::SHADER::" << type << "::COMPILATION_FAILED\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
        else
        {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success)
            {
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
    }
};

#endif