#include <iostream>
#include <vector>
#include <sstream>
#include <chrono>
#include <RenderAbstraction.hpp>
#include <Camera.hpp>
#include <Model.hpp>
#include <InputHandler.hpp>
#include <ProgramState.hpp>
#include <imgui.h>
#include <imgui_impl_sdl.h>
#include <imgui_impl_opengl3.h>



int main(int argc, char** argv) {
   string shaderVx = string(argv[1]);
   string shaderFg = string(argv[2]);
   string modelPath = string(argv[3]);
   RenderContext rctx;
   Camera cam = Camera(glm::vec3(2,2,0), glm::vec3(-1, -1, 0));
   // Shader
   Shader shader;
   shader.bindShader(shaderVx);
   shader.bindShader(shaderFg);

   Model myModel(modelPath.c_str());

   glm::mat4 model(1.f);
   model            = glm::scale(model,glm::vec3(0.08f));


    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // Setup Platform/Renderer bindings
    ImGui_ImplSDL2_InitForOpenGL(rctx.getWindow(), rctx.getContext());
    const char* glsl_version = "#version 130";
    ImGui_ImplOpenGL3_Init(glsl_version);


   struct ProgramState state(SDL_GetTicks());
   glViewport(0,0,800,600);
   while(!state.quit) {
      //collect time


      state.time.before = state.time.after;
      state.time.after = SDL_GetTicks();

      ImGui_ImplOpenGL3_NewFrame();
      ImGui_ImplSDL2_NewFrame(rctx.getWindow());
      ImGui::NewFrame();
      ImGui::ShowDemoWindow();
      ImGui::Render();

      //update program state up on input
      InputHandler::handleInput(state);

      //update camera
      cam.update(state);

      //update context
      rctx.enableDepthTest();
      rctx.clearColor(0.0f, 0.0f, 0.1f, 1.0f);
      rctx.clearColorBuffer();
      rctx.clearDepthBuffer();

      //initialise shader
      shader.activate();
      shader["mvp"] = cam.getProjection() * cam.getView() * model;

      //Draw all
      myModel.Draw(shader);
      ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
      rctx.swapBuffers();
   }
}
