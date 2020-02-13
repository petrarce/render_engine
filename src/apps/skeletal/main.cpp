#include <iostream>
#include <vector>
#include <sstream>
#include <chrono>
#include <RenderAbstraction.hpp>
#include <Camera.hpp>
#include <Model.hpp>
#include <InputHandler.hpp>
#include <ProgramState.hpp>


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


   auto start = std::chrono::steady_clock::now();
   struct ProgramState state(SDL_GetTicks());
   glViewport(0,0,800,600);
   while(!state.quit) {
      //collect time
      state.time.before = state.time.after;
      state.time.after = SDL_GetTicks();

      //update program state up on input
      InputHandler::handleInput(state);

      //update camera
      cam.update(state);
      rctx.enableDepthTest();
      rctx.clearColor(0.0f, 0.0f, 0.1f, 1.0f);
      rctx.clearColorBuffer();
      rctx.clearDepthBuffer();


      shader.activate();
      shader["mvp"] = cam.getProjection() * cam.getView() * model;

      myModel.Draw(shader);
      rctx.swapBuffers();
   }
}
