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

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
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
#if 0
        float positions[] = {
            -100.0f, -100.0f, 0.0f, 0.0f,
             100.0f, -100.0f, 1.0f, 0.0f,
             100.0f,  100.0f, 1.0f, 1.0f,
            -100.0f,  100.0f, 0.0f, 1.0f           
        };

        unsigned int indices[] = {
            0,1,2,
            2,3,0
        };

#endif
        GLCALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
        GLCALL(glEnable(GL_BLEND));
#if 0
        VertexArray va;
        VertexBuffer vb(positions, 4 * 4 * sizeof(float));
        
        VertexBufferLayout layout;
        layout.Push<float>(2);
        layout.Push<float>(2);
        va.addBuffer(vb, layout);

        IndexBuffer ib(indices, 6);
        glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
        glm::mat4 view = glm::translate(glm::mat4(1.0f),glm::vec3(0,0,0));


        /*Dit was het probleem*/
        //GLCALL(glEnableVertexAttribArray(0));
        //GLCALL(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, nullptr));

        Shader shader("res/shaders/basic.shader");
        shader.bind();
        //shader.setUniform4f("u_color", 0.1f, 0.6f, 0.7f, 1.0f);

        Texture texture("res/textures/img.png");
        texture.bind();
        shader.setUniform1i("u_Texture", 0);

        va.Unbind();
        shader.unbind();
        vb.Unbind();
        ib.Unbind();
#endif
        Renderer renderer;

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        ImGui::StyleColorsDark();
        //ImGui::StyleColorsClassic();

        //glm::vec3 translationA(0, 0, 0);
        //glm::vec3 translationB(400, 0, 0);

        /* Setup Platform/Renderer bindings */
        ImGui_ImplGlfw_InitForOpenGL(window, true);

        test::TestClearColor test;

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            renderer.clear();

            test.onUpdate(0.0f);
            test.onRender();

            glfwPollEvents();
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();
            test.onImGuiRender();
            
#if 0
            {
                glm::mat4 model = glm::translate(glm::mat4(1.0f), translationA);
                glm::mat4 mvp = proj * view * model;
                shader.bind();
                shader.setUniformMat4f("u_MVP", mvp);
                renderer.draw(va, ib, shader);
            }
            
            {
                glm::mat4 model = glm::translate(glm::mat4(1.0f), translationB);
                glm::mat4 mvp = proj * view * model;
                shader.bind();
                shader.setUniformMat4f("u_MVP", mvp);
                renderer.draw(va, ib, shader);
            }
            
            {
                ImGui::Begin("Debug Window");                          // Create a window called "Debug" and append into it.

                ImGui::SliderFloat3("Translation A", &translationA.x, 0.0f, 960.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
                ImGui::SliderFloat3("Translation B", &translationB.x, 0.0f, 960.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
                ImGui::End();
            }
#endif
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            /* Swap front and back buffers */
            GLCALL(glfwSwapBuffers(window));

            /* Poll for and process events */
            GLCALL(glfwPollEvents());
        }
    }
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
    return 0;
}