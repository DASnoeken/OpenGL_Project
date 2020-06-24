#pragma once
#include "Test.h"
#include "Shader.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace test {
	class TestBatchMultiColor : public Test {
	public:
		TestBatchMultiColor();
		~TestBatchMultiColor();

		void onUpdate(float deltaTime);
		void onRender();
		void onImGuiRender();

	private:
		glm::vec3 m_translationA;
		glm::mat4 m_Proj;
		glm::mat4 m_View;
		std::unique_ptr<Shader> m_shader;
		std::unique_ptr<VertexArray> m_va;
		std::unique_ptr<IndexBuffer> m_ib;
		std::unique_ptr<VertexBuffer> m_vb;
	};
}