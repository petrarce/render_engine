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
   string shaderDir = string(argv[1]);
   string modelPath = string(argv[2]);
   RenderContext rctx;
   rctx.enableDepthTest();
   rctx.enableSRGB();
   Camera cam = Camera(glm::vec3(3,3,3), glm::vec3(-1, -1, -1));
   // Shader
   Shader shader;
   shader.bindShader(shaderDir + "simple.vert");
   shader.bindShader(shaderDir + "simple.frag");

   Model scene(modelPath.c_str());

   glm::mat4 model(1.f);
   model            = glm::scale(model,glm::vec3(0.08f));


   rctx.initGui();

   struct ProgramState state(SDL_GetTicks());
   glViewport(0,0,800,600);
   
   RigidBodySystem rbs;
//   RigidBody rb1(  1,
//                  Vector3f(0,0,0),
//                  Vector3f(0,0,0),
//                  Vector3f::Zero(),
//                  Vector3f(0,0,0),
//                  modelPath);
//   RigidBody rb2(  1,
//                  Vector3f(3.14f / 4.0f,0, 0),//3.14f / 4.0f),
//                  Vector3f(0, 3, 3),
//                  Vector3f::Zero(),
//                  Vector3f(0,-0.5,-0.5),
//                  modelPath);

//   rbs.addRigidBody(rb1);
//   rbs.addRigidBody(rb2);
   
   float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
       // positions   // texCoords
       -1.0f,  1.0f,  0.0f, 1.0f,
       -1.0f, -1.0f,  0.0f, 0.0f,
        1.0f, -1.0f,  1.0f, 0.0f,

       -1.0f,  1.0f,  0.0f, 1.0f,
        1.0f, -1.0f,  1.0f, 0.0f,
        1.0f,  1.0f,  1.0f, 1.0f
   };
   
   //configure square screen quad
   unsigned int quadVAO, quadVBO;
   glGenVertexArrays(1, &quadVAO);
   glGenBuffers(1, &quadVBO);
   glBindVertexArray(quadVAO);
   glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
   glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
   glEnableVertexAttribArray(0);
   glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), static_cast<void*>(0));
   glEnableVertexAttribArray(1);
   glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), (void*)(2*sizeof(float)));
   Shader quadShader;
   quadShader.bindShader(shaderDir + "light.vert");
   quadShader.bindShader(shaderDir + "light.frag");
   
   //configure frame buffer
   unsigned int framebuffer;
   glGenFramebuffers(1, &framebuffer);
   glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
   //create texture for the framebuffer
   unsigned int fbufTexture;
   glGenTextures(1, &fbufTexture);
   glBindTexture(GL_TEXTURE_2D, fbufTexture);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fbufTexture, 0);
   
   //create render buffer
   unsigned int rbo;
   glGenTextures(1, &rbo);
   glBindTexture(GL_TEXTURE_2D, rbo);
   //glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 600);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 800,600, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);   
   glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, rbo, 0);
   if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
   {
       cout << "failed to generate framebuffer";
       return 0;
   } else 
   {
       cout << "framebuffer sucessfully created";
   }
   glBindFramebuffer(GL_FRAMEBUFFER, 0);
   

   
   while(!state.quit) {
      //collect time

      state.time.before = state.time.after;
      state.time.after = SDL_GetTicks();

      rctx.updateGui(state);
      //update program state up on input
      InputHandler::handleInput(state);

      //update context
      glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);      
      rctx.clearColor(state.window.color.r, state.window.color.g, state.window.color.b, state.window.color.a);
      rctx.clearColorBuffer();
      rctx.clearDepthBuffer();
      rctx.enableDepthTest();

      if(state.active){
         cam.update(state);
      }
      if(state.sim.active) {
         rbs.update(1.0/60);         
      }
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
      scene.Draw(shader);
      rbs.draw(shader);
      
      glBindFramebuffer(GL_FRAMEBUFFER, 0);
      rctx.clearColor(1,1,1,1);
      //rctx.clearColorBuffer();
      rctx.disableDepthTest();
      
      quadShader.activate();
      quadShader["frame"] = 0;
      quadShader["depth_texture"] = 1;
      quadShader["numPixels"] = 0;
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, fbufTexture);
      glActiveTexture(GL_TEXTURE1);
      glBindTexture(GL_TEXTURE_2D, rbo);
      
      glBindVertexArray(quadVAO);
      glDrawArrays(GL_TRIANGLES, 0, 6);
      

      rctx.drawGui();
      rctx.swapBuffers();
   }
}
