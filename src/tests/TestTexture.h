#pragma once

#include "Test.h"
#include <memory>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Texture.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "VertexArray.h"
#include "VertexBuffer.h"

namespace test {
	class TestTexture : public Test{
	public:
		TestTexture();
		~TestTexture();

		void onUpdate(float deltatime) override;
		void onRender() override;
		void onImGuiRender() override;

	private:
		glm::vec3 m_translationA;
		glm::vec3 m_translationB;
		glm::mat4 m_Proj;
		glm::mat4 m_View;
		std::unique_ptr<Shader> m_shader;
		std::unique_ptr<VertexArray> m_va;
		std::unique_ptr<IndexBuffer> m_ib;
		std::unique_ptr<Texture> m_texture;
		std::unique_ptr<VertexBuffer> m_vb;
	};
}