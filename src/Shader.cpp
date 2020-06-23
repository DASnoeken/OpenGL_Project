#include "Shader.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"

Shader::Shader(const std::string& filepath):m_Filepath(filepath),m_RendererID(0){
    ShaderProgramSource source = parseShader(filepath);
    m_RendererID = createShader(source.vertexSource, source.fragmentSource);
}

Shader::~Shader(){
    GLCALL(glDeleteProgram(m_RendererID));
}

void Shader::bind() const{
    GLCALL(glUseProgram(m_RendererID));
}

void Shader::unbind() const{
    GLCALL(glUseProgram(0));
}

void Shader::setUniform1i(const std::string& name, int value){
    GLCALL(glUniform1i(getUniformLocation(name), value));
}

void Shader::setUniform1f(const std::string& name, float value){
    GLCALL(glUniform1f(getUniformLocation(name), value));
}

void Shader::setUniform4f(const std::string& name, float v0, float v1, float v2, float v3){
    GLCALL(glUniform4f(getUniformLocation(name), v0, v1, v2, v3));
}

void Shader::setUniformMat4f(const std::string& name, const glm::mat4 matrix){
    GLCALL(glUniformMatrix4fv(getUniformLocation(name),1,GL_FALSE,&matrix[0][0]));
}

int Shader::getUniformLocation(const std::string& name){
    if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end()) {
        return m_UniformLocationCache[name];
    }
    GLCALL(int location = glGetUniformLocation(m_RendererID, name.c_str()));
    if (location == -1) {
        std::cout << "WARNING uniform '" << name << "' doesn't exist!" << std::endl;
    }
    
    m_UniformLocationCache[name] = location;
    
	return location;
}

unsigned int Shader::createShader(const std::string& vertexShader, const std::string& fragmentShader) {
    unsigned int program = glCreateProgram();
    unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

    GLCALL(glAttachShader(program, vs));
    GLCALL(glAttachShader(program, fs));
    GLCALL(glLinkProgram(program));
    GLCALL(glValidateProgram(program))

    GLCALL(glDeleteShader(vs));
    GLCALL(glDeleteShader(fs));

    return program;
}

ShaderProgramSource Shader::parseShader(const std::string& filepath) {
    enum class ShaderType {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };
    std::ifstream stream(filepath);
    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;
    while (getline(stream, line)) {
        if (line.find("#shader") != std::string::npos) {
            if (line.find("vertex") != std::string::npos) {
                type = ShaderType::VERTEX;
            }
            else if (line.find("fragment") != std::string::npos) {
                type = ShaderType::FRAGMENT;
            }
            else if (line.find("ignore") != std::string::npos) {
                type = ShaderType::NONE;
            }
        }
        else {
            if (type != ShaderType::NONE) {
                ss[(int)type] << line << '\n';
            }
        }
    }

    return { ss[0].str(), ss[1].str() };
}

unsigned int Shader::compileShader(unsigned int type, const std::string& source) {
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    GLCALL(glShaderSource(id, 1, &src, nullptr));
    GLCALL(glCompileShader(id));

    int result;
    GLCALL(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
    if (result == GL_FALSE) {
        int length;
        GLCALL(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
        char* message = (char*)alloca(length * sizeof(char));       //alloca allows dynamic stack allocation
        GLCALL(glGetShaderInfoLog(id, length, &length, message));
        std::cout << "FAILED TO COMPILE " << (type == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT") << " SHADER!" << std::endl;
        std::cout << message << std::endl;
        GLCALL(glDeleteShader(id));
        return 0;
    }

    return id;
}
