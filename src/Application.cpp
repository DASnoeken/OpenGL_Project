#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Shader.h"
#include "Texture.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_glfw.h"

#include "tests/TestClearColor.h"
#include "tests/TestTexture.h"
#include "tests/TestBatchRendering.h"

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(960, 540, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);         //Here you make the valid OPENGL context

    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK)
        std::cout << "Not GLEW_OK" << std::endl;
    else {
        std::cout << "GLEW_OK" << std::endl;
    }

    std::cout << glGetString(GL_VERSION) << std::endl;
    {

        GLCALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
        GLCALL(glEnable(GL_BLEND));

        Renderer renderer;

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        ImGui::StyleColorsDark();
        //ImGui::StyleColorsClassic();

        

        /* Setup Platform/Renderer bindings */
        ImGui_ImplGlfw_InitForOpenGL(window, true);

        test::Test* currentTest = nullptr;
        test::TestMenu* testMenu = new test::TestMenu(currentTest);
        currentTest = testMenu;

        testMenu->registerTest<test::TestClearColor>("Clear Color");
        testMenu->registerTest<test::TestTexture>("Texture");
        testMenu->registerTest<test::TestBatchRendering>("Batch Rendering");
        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            GLCALL(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
            /* Render here */
            renderer.clear();

            glfwPollEvents();
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            if (currentTest) {
                currentTest->onUpdate(0.0f);
                currentTest->onRender();
                ImGui::Begin("Test");
                if (currentTest != testMenu && ImGui::Button("<-")) {
                    delete currentTest;
                    currentTest = testMenu;
                }
                currentTest->onImGuiRender();
                ImGui::End();
            }

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            /* Swap front and back buffers */
            GLCALL(glfwSwapBuffers(window));

            /* Poll for and process events */
            GLCALL(glfwPollEvents());
        }
        delete currentTest;
        if (currentTest != testMenu) {
            delete testMenu;

        }
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        glfwTerminate();

        return 0;
    }
}