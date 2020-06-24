#include "TestBatchMultiTexture.h"
#include "imgui/imgui.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "Renderer.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <array>
#include <Texture.h>
#include <stb_image\stb_image.h>

test::TestBatchMultiTexture::TestBatchMultiTexture():
    m_Proj(glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f)),
    m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0))),
    m_translationA((0, 0, 0)) {
    
    m_shader = std::make_unique<Shader>("res/shaders/multTexture.shader");
    GLCALL(glUseProgram(m_shader->getRendererID()));
    auto textureLoc = m_shader->getUniformLocation("u_Textures");
    int samplers[2] = { 0, 1 };
    GLCALL(glUniform1iv(textureLoc, 2, samplers));

    float positions[] = {
             0.0f,   0.0f,      0.25f, 0.1f, 0.85f, 1.0f,   0.0f, 0.0f,  0.0f,   //0
             200.0f, 0.0f,      0.25f, 0.1f, 0.85f, 1.0f,   1.0f, 0.0f,  0.0f,   //1
             200.0f, 200.0f,    0.25f, 0.1f, 0.85f, 1.0f,   1.0f, 1.0f,  0.0f,   //2
             0.0f,   200.0f,    0.25f, 0.1f, 0.85f, 1.0f,   0.0f, 1.0f,  0.0f,   //3
                               
             300.0f,  0.0f,      0.9f, 0.2f,  0.1f, 1.0f,   0.0f, 0.0f,  1.0f,   //4
             500.0f,  0.0f,      0.9f, 0.2f,  0.1f, 1.0f,   1.0f, 0.0f,  1.0f,   //5
             500.0f,  200.0f,    0.9f, 0.2f,  0.1f, 1.0f,   1.0f, 1.0f,  1.0f,   //6
             300.0f,  200.0f,    0.9f, 0.2f,  0.1f, 1.0f,   0.0f, 1.0f,  1.0f   //7
    };

    unsigned int indices[] = {
        0,1,2,
        2,3,0,

        4,5,6,
        6,7,4
    };

    
    m_va = std::make_unique<VertexArray>();

    GLCALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    GLCALL(glEnable(GL_BLEND));

    m_vb = std::make_unique<VertexBuffer>(positions, std::size(positions) * sizeof(float));

    VertexBufferLayout layout;
    layout.Push<float>(2);  //position
    layout.Push<float>(4);  //color
    layout.Push<float>(2);  //texture coordinates
    layout.Push<float>(1);  //texture ID
    m_va->addBuffer(*m_vb, layout);

    m_ib = std::make_unique<IndexBuffer>(indices, std::size(indices));
    m_shader->bind();
    m_texture1ID = loadTexture("res/textures/unnamed.jpg");
    m_texture2ID = loadTexture("res/textures/img.png");
}


test::TestBatchMultiTexture::~TestBatchMultiTexture(){

}

void test::TestBatchMultiTexture::onUpdate(float deltaTime){
    GLCALL(glBindTextureUnit(0,m_texture1ID));
    GLCALL(glBindTextureUnit(1,m_texture2ID));
}

void test::TestBatchMultiTexture::onRender(){
    GLCALL(glClearColor(0.1f, 0.1f, 0.1f, 1.0f));
    GLCALL(glClear(GL_COLOR_BUFFER_BIT));

    Renderer renderer;
    {
        glm::mat4 Model = glm::translate(glm::mat4(1.0f), m_translationA);
        glm::mat4 MVP = m_Proj * m_View * Model;
        m_shader->bind();
        m_shader->setUniformMat4f("u_MVP", MVP);
        renderer.draw(*m_va, *m_ib, *m_shader);
    }
}

void test::TestBatchMultiTexture::onImGuiRender(){
    ImGui::Begin("Batchrendering Test");
    ImGui::SliderFloat("Move x", &m_translationA.x, 0.0f, 960.0f);
    ImGui::SliderFloat("Move Y", &m_translationA.y, 0.0f, 540.0f);
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::End();
}

GLuint test::TestBatchMultiTexture::loadTexture(const std::string& path)
{
    int w, h, bits;
    stbi_set_flip_vertically_on_load(1);
    auto* localBuffer = stbi_load(path.c_str(), &w, &h, &bits, 4);

    GLuint textureID;
    GLCALL(glGenTextures(1, &textureID));
    GLCALL(glBindTexture(GL_TEXTURE_2D, textureID));

    GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

    GLCALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, localBuffer));
    GLCALL(glBindTexture(GL_TEXTURE_2D, 0));

    if (localBuffer) {
        stbi_image_free(localBuffer);
    }
    return textureID;
}
