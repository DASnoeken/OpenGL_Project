#include "TestTriangle.h"
#include <imgui\imgui.h>
#include <iostream>

test::TestTriangle::TestTriangle() : 
	m_Dimensions_XY{ 1.0f, 1.0f }, 
    m_Positions{ -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.5f },
    vb(m_Positions, 2 * 3 * sizeof(float)),
    indices{ 0, 1, 2}, 
    shader("res/shaders/basic.shader"),
    ib(indices,3)   
{
    shader.bind();
    shader.setUniform4f("u_color", 1.0f, 1.0f, 1.0f, 1.0f);
    layout.Push<float>(2);
    va.addBuffer(vb, layout);
}

test::TestTriangle::~TestTriangle(){
}

void test::TestTriangle::onUpdate(float deltatime){
}

void test::TestTriangle::onRender(){
    va.Bind();
    ib.Bind();
    shader.bind();
    r.draw(va, ib, shader);
}

void test::TestTriangle::onImGuiRender(){
    ImGui::SliderFloat("Size X", &m_Dimensions_XY[0], 0.0f, 10.0f);
    ImGui::SliderFloat("Size Y", &m_Dimensions_XY[1], 0.0f, 10.0f);
    m_Positions[2] = m_Dimensions_XY[0];
    m_Positions[5] = m_Dimensions_XY[1];
}
