#pragma once
#include <string>
#include <unordered_map>
#include "glm/glm.hpp"

struct ShaderProgramSource {
	std::string vertexSource;
	std::string fragmentSource;
};

class Shader {
public:
	Shader(const std::string& filepath);
	~Shader();
	void bind() const;
	void unbind() const;

	//Set uniforms
	void setUniform1i(const std::string& name, int value);
	void setUniform1f(const std::string& name, float value);
	void setUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
	void setUniformMat4f(const std::string& name, const glm::mat4 matrix);
	int getUniformLocation(const std::string& name);
	inline unsigned int getRendererID() const { return m_RendererID; }
private:
	
	unsigned int createShader(const std::string& vertexShader, const std::string& fragmentShader);
	ShaderProgramSource parseShader(const std::string& filepath);
	unsigned int compileShader(unsigned int type, const std::string& source);
	std::string m_Filepath;
	unsigned int m_RendererID;
	//caching system
	std::unordered_map<std::string, int> m_UniformLocationCache;

};