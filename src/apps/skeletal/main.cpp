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
#include <FrameBuffer.hpp>
#include <Buffer.hpp>
#include <Renderer.hpp>

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
   shader.bindShader(shaderDir + "shadow.vert");
   shader.bindShader(shaderDir + "shadow.frag");
   Shader shadowShader;
   shadowShader.bindShader(shaderDir + "lightDepth.vert");
   shadowShader.bindShader(shaderDir + "lightDepth.frag");
   Shader quadShader;
   quadShader.bindShader(shaderDir + "light.vert");
   quadShader.bindShader(shaderDir + "light.frag");

   Model scene(modelPath.c_str());

   glm::mat4 model(1.f);
   model            = glm::scale(model,glm::vec3(0.08f));


   rctx.initGui();

   struct ProgramState state(SDL_GetTicks());
   glViewport(0,0,800,600);
   
   RigidBodySystem rbs;
   
   float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
       // positions   // texCoords
       -1.0f,  1.0f,  0.0f, 1.0f,
       -1.0f, -1.0f,  0.0f, 0.0f,
        1.0f, -1.0f,  1.0f, 0.0f,

        1.0f,  1.0f,  1.0f, 1.0f
   };
   unsigned int indexBuffer[] = {0,1,2,0,2,3};
   
   //configure square screen quad
//   glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), (void*)(2*sizeof(float)));
   ElementArrayBuffer screenQuadBuffer;
   screenQuadBuffer.create(quadVertices, 4*sizeof(float), 4, indexBuffer, 6);
   screenQuadBuffer.defineAttribute(0, 4*sizeof(float), 2, false, GL_FLOAT, 0);
   screenQuadBuffer.defineAttribute(1, 4*sizeof(float), 2, false, GL_FLOAT, (void*)(2*sizeof(float)));
   
   FrameBuffer shadowBuffer;
   shadowBuffer.create();
   shadowBuffer.createDepthTexture(800, 600);
   shadowBuffer.bind();
   glDrawBuffer(GL_NONE);
   glReadBuffer(GL_NONE);
   FrameBuffer::bindDefault();
   
   FrameBuffer defferedBuffer;
   defferedBuffer.create();
   defferedBuffer.bind();
   defferedBuffer.createColorTexture(800, 600);
   defferedBuffer.createDepthTexture(800, 600);
   defferedBuffer.bind();
   assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
   FrameBuffer::bindDefault();
   glm::mat4 lightView = cam.getView();
   glm::mat4 lightProj = cam.getOrthProjection();
   glm::vec3 lightDir = cam.getDirection();
   glEnable(GL_CULL_FACE);
   
   while(!state.quit) {
      //collect time

      state.time.before = state.time.after;
      state.time.after = SDL_GetTicks();

      rctx.updateGui(state);
      //update program state up on input
      InputHandler::handleInput(state);

      //firs path generate depth buffer from light sorce
      shadowBuffer.bind();
      rctx.enableDepthTest();
      rctx.clearDepthBuffer();
      shadowShader.activate();
      shadowShader["projection"] = lightProj;
      shadowShader["model"] = model;
      shadowShader["view"] = lightView;
      glCullFace(GL_FRONT);
      //scene.Draw(shadowShader);
      glCullFace(GL_BACK);
      
      //second path draw scene
      defferedBuffer.bind();
      rctx.enableDepthTest();      
      rctx.clearColor(state.window.color.r, state.window.color.g, state.window.color.b, state.window.color.a);
      rctx.clearColorBuffer();
      rctx.clearDepthBuffer();
      rctx.enableDepthTest();

      if(state.active){
         cam.update(state);
      }
      if(state.sim.active) {
          lightView = cam.getView();
          lightProj = cam.getOrthProjection();
          lightDir = cam.getDirection();

         //rbs.update(1.0/60);         
      }
      //initialise shader
      shader.activate();
      shadowBuffer.bindDepthTexture(3);
      shader["shadow_depth"] = 3;
      shader["model"] = model;
      shader["projection"] = cam.getProjection();
      shader["view"] = cam.getView();
      shader["lightViewProjection"] = lightProj * lightView;
      shader["lightDirection"] = lightDir;
      shader["shadowKernelSize"] = state.shadow.kernelSize;
      shader["shadowKernelOffset"] = state.shadow.kernelOffset;
      //Draw all
      //scene.Draw(shader);
      rbs.draw(shader);
      
      FrameBuffer::bindDefault();
      rctx.disableDepthTest();
      
      quadShader.activate();
      quadShader["frame"] = 0;
      quadShader["depth_texture"] = 1;
      quadShader["numPixels"] = 0;
      defferedBuffer.bindColorTexture(0);
      defferedBuffer.bindDepthTexture(1);
      
      Renderer::draw(&screenQuadBuffer, GL_TRIANGLES);

      rctx.drawGui();
      rctx.swapBuffers();
   }
}
