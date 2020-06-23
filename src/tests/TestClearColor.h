#pragma once

#include "Test.h"

namespace test {
	class TestClearColor : public Test{
	public:
		TestClearColor();
		~TestClearColor();

		void onUpdate(float deltatime) override;
		void onRender() override;
		void onImGuiRender() override;

	private:
		float m_ClearColor[4];
	};
}