#pragma once
#include <VertexArray.hpp>
#include <Shader.hpp>
#include <glm/glm.hpp>
#include <memory>
#include <SDL2/SDL.h>


enum class PrimitiveType { Point, TriangleStrip, Triangle };
enum class StateType { Depth, Stencil, Blend, Culling };
enum class DepthFunction { Always, Never, Less, Equal, LessEqual, Greater, NotEqual, GreaterEqual };

class RenderContext {
public:
   RenderContext();
   ~RenderContext(){}


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

   VertexArray
   createVertexArray();

   void
   draw(VertexArray & VertexArray, PrimitiveType p);

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

private:
   SDL_Window * m_Window;

   VertexArray m_Quad;
};
