#include <RenderContext.hpp>
#include <Camera.hpp>
#include <Buffer.hpp>
#include <Shader.hpp>
#include <InputHandler.hpp>
#include <Renderer.hpp>
#include <ModelManager.hpp>
#include <DrowableObjManager.hpp>

using namespace glm;

int main(int argc, char** argv)
{
    RenderContext rctx;
    rctx.initGui();
    Shader shader;
    shader.bindShader(string(argv[1]) + "simple.vert");
    shader.bindShader(string(argv[1]) + "simple.frag");
    Camera cam = Camera(glm::vec3(3,3,3), glm::vec3(-1, -1, -1));
    
    vector<float> quad = {
        -1, -1, -1,
        -1, -1,  1,
        -1,  1, -1,
        -1,  1,  1,
         1, -1, -1,
         1, -1,  1,
         1,  1, -1,
         1,  1,  1
    };
    vector<unsigned int> quadIndecis = {
        0,1,2,2,1,3,
        0,4,6,0,6,2,
        0,4,5,0,5,1,
        6,7,3,6,3,2,
        6,4,5,6,5,7,
    };
    ElementArrayBuffer quadBuf;
    quadBuf.create(quad.data(), 3*sizeof(float), quad.size(), quadIndecis.data(), quadIndecis.size());
    quadBuf.defineAttribute(0,3*sizeof(float), 3, false, GL_FLOAT, 0);
    quadBuf.defineAttribute(1,3*sizeof(float), 3, false, GL_FLOAT, 0);
    
    vector<float> tetrahedra = {
        1,0,0, 1,1,1,
        0,1,0, 1,1,1,
        0,0,1, 1,1,1,

        0,0,0, -1,0,0,
        0,0,1, -1,0,0,
        0,1,0, -1,0,0,
        
        0,0,0, 0,0,-1,
        0,1,0, 0,0,-1,
        1,0,0, 0,0,-1,
        
        0,0,0, 0,-1,0,
        1,0,0, 0,-1,0,
        0,0,1, 0,-1,0,

    };
    ArrayBuffer tetrahedraBuffer;
    tetrahedraBuffer.create(tetrahedra.data(), sizeof(float)*6, tetrahedra.size()/6);
    tetrahedraBuffer.defineAttribute(0,6*sizeof(float), 3, false, GL_FLOAT, 0);
    tetrahedraBuffer.defineAttribute(1,6*sizeof(float), 3, false, GL_FLOAT, (void*)(3*sizeof(float)));
    
    
    
    if(argc == 3)
    {
        ModelManager::createItemFromFile(string(argv[2]));
        DrowableID obj = DrowableObjectManager::createItem();
        DrowableObjectManager::setModel(obj, string(argv[2]));
        DrowableObjectManager::setModelMatrix(obj, mat4(1));
    }
    

    struct ProgramState state(SDL_GetTicks());
    glViewport(0, 0, 800, 600);
    while(!state.quit)
    {
        state.time.before = state.time.after;
        state.time.after = SDL_GetTicks();
        
        rctx.updateGui(state);
        
        InputHandler::handleInput(state);
        
        if(state.active)
            cam.update(state);
        
        rctx.enableDepthTest();
        rctx.enableSRGB();
        rctx.clearColorBuffer();
        glClearDepth(1.0f);
        rctx.clearDepthBuffer();
        shader.activate();
        
        shader["defaultDiffuse"] = glm::vec3(0.3,0.5,0.7);
        shader["lightDirection"] = glm::vec3(-1,-1,-1);
        Renderer::renderPass(cam, shader, GL_TRIANGLES);
        
        shader["useDiffuse"] = false;
        shader["model"] = glm::mat4(0.5);
        shader["defaultDiffuse"] = glm::vec3(1,0,0);
        Renderer::draw(&quadBuf, GL_TRIANGLES);
        shader["defaultDiffuse"] = glm::vec3(0,1,0);
        shader["model"] = glm::mat4(glm::vec4(1,0,0,0),
                                    glm::vec4(0,1,0,0),
                                    glm::vec4(0,0,1,0),
                                    glm::vec4(3,0,0,1));
        Renderer::draw(&tetrahedraBuffer, GL_TRIANGLES);
        
        rctx.swapBuffers();
    }
    
    return 0;
}
