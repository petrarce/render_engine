#include <iostream>
#include <vector>
#include <sstream>
#include <chrono>
#include <RenderAbstraction.hpp>
#include <Camera.hpp>
#include <Model.hpp>
#include <InputHandler.hpp>
#include <ProgramState.hpp>
#include <RigidBodySystem.h>
#include <RigidBody.h>
#include <Eigen/Dense>

using namespace Eigen;

static float myrand(){
   return float(rand()) / RAND_MAX;
}

int main(int argc, char** argv) {
   string shaderVx = string(argv[1]);
   string shaderFg = string(argv[2]);
   string modelPath = string(argv[3]);
   RenderContext rctx;
   rctx.enableDepthTest();
   rctx.enableSRGB();
   Camera cam = Camera(glm::vec3(2,2,0), glm::vec3(-1, -1, 0));
   // Shader
   Shader shader;
   shader.bindShader(shaderVx);
   shader.bindShader(shaderFg);

   Model myModel(modelPath.c_str());
   if(argc == 5)
      myModel.loadNormalMapTexture(argv[4]);

   glm::mat4 model(1.f);
   model            = glm::scale(model,glm::vec3(0.08f));


   rctx.initGui();

   struct ProgramState state(SDL_GetTicks());
   glViewport(0,0,800,600);
   
   RigidBodySystem rbs;
   for(int i = 0; i < 10; i++){
      RigidBody rb(  i,
                     Vector3f::Zero() * i,
                     Vector3f::Random() * i,
                     Vector3f(0.05,0.05,0.05) * i,
                     Vector3f(1,1,1) * i,
                     modelPath);
      rbs.addRigidBody(rb);
   }

   while(!state.quit) {
      //collect time


      state.time.before = state.time.after;
      state.time.after = SDL_GetTicks();

      rctx.updateGui(state);
      //update program state up on input
      InputHandler::handleInput(state);

      //update camera
      if(state.active){
        cam.update(state);
      }

      //update context
      rctx.clearColor(state.window.color.r, state.window.color.g, state.window.color.b, state.window.color.a);
      rctx.clearColorBuffer();
      rctx.clearDepthBuffer();

      //initialise shader
      shader.activate();
      shader["view"] = cam.getView();
      shader["projection"] = cam.getProjection();
      shader["model"] = model;
      shader["defaultDiffuse"] = glm::vec3(0.1,0.2,0.6);
      shader["lightPosition"] = state.light.position;
      shader["lightColor"] = glm::vec3(state.light.color);
      shader["camPosition"] = cam.getPosition();
      shader["ambFactor"] = state.light.model.ambient.factor;
      shader["specFactor"] = state.light.model.specular.factor;
      shader["diffFactor"] = state.light.model.diffuse.factor;
      shader["lightingModel"] = state.light.model.specular.type;
      shader["rghness"] = state.light.model.specular.roughness;
      shader["frnlFactor"] = state.light.model.specular.fresnel;
      shader["metalicness"] = state.light.model.specular.metalicness;
      shader["mode"] = state.mode;


      //Draw all
      //myModel.Draw(shader);
      rbs.update(1.0/60);
      rbs.draw(shader);
      rctx.drawGui();
      rctx.swapBuffers();
   }
}
