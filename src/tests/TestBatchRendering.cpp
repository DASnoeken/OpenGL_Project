#include "TestBatchRendering.h"
#include "imgui/imgui.h"
#include "Texture.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "Renderer.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

test::TestBatchRendering::TestBatchRendering():
    m_Proj(glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f)),
    m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0))),
    m_translationA((0, 0, 0)) {
    float positions[] = {
            -100.0f, -100.0f, 0.0f, 0.0f,   //0
             100.0f, -100.0f, 1.0f, 0.0f,   //1
             100.0f,  100.0f, 1.0f, 1.0f,   //2
            -100.0f,  100.0f, 0.0f, 1.0f,   //3

             100.0f,  100.0f, 0.0f, 0.0f,   //4
             300.0f,  100.0f, 1.0f, 0.0f,   //5
             300.0f,  300.0f, 1.0f, 1.0f,   //6
             100.0f,  300.0f, 0.0f, 1.0f,   //7
    };

    unsigned int indices[] = {
        0,1,2,
        2,3,0,

        4,5,6,
        6,7,4
    };

    m_shader = std::make_unique<Shader>("res/shaders/basic.shader");
    m_va = std::make_unique<VertexArray>();

    GLCALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    GLCALL(glEnable(GL_BLEND));

    m_vb = std::make_unique<VertexBuffer>(positions, 4 * 8 * sizeof(float));

    VertexBufferLayout layout;
    layout.Push<float>(2);
    layout.Push<float>(2);
    m_va->addBuffer(*m_vb, layout);

    m_ib = std::make_unique<IndexBuffer>(indices, 12);
    m_shader->bind();

    m_texture = std::make_unique<Texture>("res/textures/unnamed.jpg");
    m_texture->bind();
    m_shader->setUniform1i("u_Texture", 0);
}

test::TestBatchRendering::~TestBatchRendering(){

}

void test::TestBatchRendering::onUpdate(float deltaTime){

}

void test::TestBatchRendering::onRender(){
    GLCALL(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
    GLCALL(glClear(GL_COLOR_BUFFER_BIT));

    Renderer renderer;
    m_texture->bind();
    {
        glm::mat4 Model = glm::translate(glm::mat4(1.0f), m_translationA);
        glm::mat4 MVP = m_Proj * m_View * Model;
        m_shader->bind();
        m_shader->setUniformMat4f("u_MVP", MVP);
        renderer.draw(*m_va, *m_ib, *m_shader);
    }
}

void test::TestBatchRendering::onImGuiRender(){
    ImGui::Begin("Batchrendering Test");
    ImGui::SliderFloat("Move x", &m_translationA.x, 0.0f, 960.0f);
    ImGui::SliderFloat("Move Y", &m_translationA.y, 0.0f, 540.0f);
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::End();
}
