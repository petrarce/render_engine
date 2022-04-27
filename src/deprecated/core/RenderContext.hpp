#pragma once
#include <Shader.hpp>
#include <glm/glm.hpp>
#include <memory>
#include <SDL2/SDL.h>
#include <ProgramState.hpp>



enum class PrimitiveType { Point, TriangleStrip, Triangle };
enum class StateType { Depth, Stencil, Blend, Culling };
enum class DepthFunction { Always, Never, Less, Equal, LessEqual, Greater, NotEqual, GreaterEqual };

class RenderContext {
public:
   RenderContext();
   ~RenderContext(){}

   void 
   enableSRGB();  

   void
   enableDepthTest();

   void
   disableDepthTest();

   void
   writeToDepthBuffer(DepthFunction d);
   void
   readOnlyDepthBuffer();
   void
   setDepthFunction(DepthFunction d);

   void
   disableState(StateType s);

   void
   clearColor (float r, float g, float b, float a);

   void
   clearColorBuffer();
   void
   clearDepthBuffer();

   void
   drawIndex(PrimitiveType p, int size);

/*   void
   drawDepthMap(Shader & shader, Texture &texture);
*/
   void
   swapBuffers();

   SDL_GLContext getContext(){
      return SDL_GL_GetCurrentContext();
   }

   SDL_Window* getWindow(){
      return m_Window;
   }

   void initGui();

   void updateGui(struct ProgramState& state);

   void drawGui();

   void ShowStatisticsWindow(struct ProgramState& state);
   void ShowConfigsWidget(struct ProgramState& state);



private:
   SDL_Window * m_Window;

};
