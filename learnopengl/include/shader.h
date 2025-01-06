#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

struct Shader
{
public:
    /// <summary>
    /// Program ID.
    /// </summary>
    unsigned int ID;

    /// <summary>
    /// Reads a vertex and fragment shader and builds it.
    /// </summary>
    Shader(const char *vertexPath, const char *fragmentPath)
    {
        // 1. retrive the vertex/fragment source code from the paths
        std::string vertexCode, fragmentCode;
        std::ifstream vShaderFile, fShaderFile;

        // ensure ifstream objects can throw exceptions
        vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

        try
        {
            vShaderFile.open(vertexPath);
            fShaderFile.open(fragmentPath);

            std::stringstream vShaderStream, fShaderStream;
            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();

            vShaderFile.close();
            fShaderFile.close();

            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();
        }
        catch (std::ifstream::failure &e)
        {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
        }
        const char *vShaderCode = vertexCode.c_str();
        const char *fShaderCode = fragmentCode.c_str();

        // 2. compile the shaders
        unsigned int vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, NULL);
        glCompileShader(vertex);
        CheckCompileErrors(vertex, "VERTEX");

        unsigned int fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        glCompileShader(fragment);
        CheckCompileErrors(fragment, "FRAGMENT");

        ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);
        glLinkProgram(ID);
        CheckCompileErrors(ID, "PROGRAM");

        // 3. delete the shaders as they're properly linked into the program and are not needed
        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }

    /// <summary>
    /// Uses, or activates, the shader.
    /// </summary>
    void Use()
    {
        glUseProgram(ID);
    }

    /// <summary>
    /// Sets a boolean uniform value in the shader.
    /// </summary>
    void SetBool(const std::string &name, bool value) const
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
    }

    /// <summary>
    /// Sets an integer uniform value in the shader.
    /// </summary>
    void SetInt(const std::string &name, int value) const
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    }

    /// <summary>
    /// Sets a float uniform value in the shader.
    /// </summary>
    void SetFloat(const std::string &name, float value) const
    {
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
    }

    /// <summary>
    /// Sets a Vector2 uniform value in the shader with two specified floats.
    /// </summary>
    void SetVector2(const std::string& name, float x, float y) const
    {
        glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
    }

    /// <summary>
    /// Sets a Vector2 uniform value in the shader.
    /// </summary>
    void SetVector2(const std::string &name, glm::vec2 &value) const 
    {
        glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
    }

    /// <summary>
    /// Sets a Vector3 uniform value in the shader with three specified floats.
    /// </summary>
    void SetVector3(const std::string &name, float x, float y, float z) const
    {
        glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
    }

    /// <summary>
    /// Sets a Vector3 uniform value in the shader.
    /// </summary>
    void SetVector3(const std::string &name, glm::vec3 &value) const
    {
        glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
    }

    // <summary>
    /// Sets a Vector4 uniform value in the shader with four specified floats.
    /// </summary>
    void SetVector4(const std::string &name, float x, float y, float z, float w) const
    {
        glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
    }

    /// <summary>
    /// Sets a Vector4 uniform value in the shader.
    /// </summary>
    void SetVector4(const std::string &name, glm::vec4 &value) const
    {
        glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
    }

    /// <summary>
    /// Sets a Matrix2x2 uniform value in the shader.
    /// </summary>
    void SetMatrix2x2(const std::string &name, glm::mat2 &value) const
    {
        glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &value[0][0]);
    }

    /// <summary>
    /// Sets a Matrix3x3 uniform value in the shader.
    /// </summary>
    void SetMatrix3x3(const std::string &name, glm::mat3 &value) const
    {
        glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &value[0][0]);
    }

    /// <summary>
    /// Sets a Matrix4x4 uniform value in the shader.
    /// </summary>
    void SetMatrix4x4(const std::string &name, glm::mat4 &value) const
    {
        glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &value[0][0]);
    }

private:
    void CheckCompileErrors(unsigned int shader, std::string type)
    {
        int success;
        char infoLog[1024];
        if (type != "PROGRAM")
        {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n"
                          << infoLog
                          << "\n -- --------------------------------------------------- -- "
                          << std::endl;
            }
        }
        else
        {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success)
            {
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n"
                          << infoLog
                          << "\n -- --------------------------------------------------- -- "
                          << std::endl;
            }
        }
    }
};

#endif