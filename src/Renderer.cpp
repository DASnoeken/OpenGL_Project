#include "Renderer.h"
#include <iostream>

void GLClearError() {
    while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line) {
    while (GLenum error = glGetError()) {

        std::cout << "[OPENGL ERROR] (" << error << ")\nIn function call: " << function << ",\nin file: " << file << ",\non line: " << line << std::endl;
        return false;
    }
    return true;
}

void Renderer::draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const {
    shader.bind();
    va.Bind();
    ib.Bind();
    GLCALL(glDrawElements(GL_TRIANGLES, ib.getCount(), GL_UNSIGNED_INT, nullptr));
}

void Renderer::clear() const {
    GLCALL(glClear(GL_COLOR_BUFFER_BIT));
}