#pragma once

#include "Test.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Renderer.h"

namespace test {
	class TestTriangle : public Test {
	public:
		TestTriangle();
		~TestTriangle();

		void onUpdate(float deltatime) override;
		void onRender() override;
		void onImGuiRender() override;
	private:
		float m_Dimensions_XY[2];
		float m_Positions[6];
		VertexArray va;
		VertexBuffer vb;
		VertexBufferLayout layout;
		unsigned int indices[3];
		Renderer r;
		IndexBuffer ib;
		Shader shader;
	};
}