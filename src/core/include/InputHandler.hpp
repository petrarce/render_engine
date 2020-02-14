#pragma once
#include <array>
#include <SDL2/SDL.h>
#include <ProgramState.hpp>
class InputHandler {
public:
    //count - maximum number of polled events
    static void handleInput(struct ProgramState& state){
        SDL_Event e;
        
        state.mouseMove.x = 0;
        state.mouseMove.y = 0;

        for(int i = 0; i < 10 && SDL_PollEvent(&e); i++){
            switch(e.type){
                case SDL_QUIT:
                state.quit = true;
                break;

                case SDL_MOUSEMOTION:
                {                
                    int dx = 0;
                    int dy = 0;
                    SDL_GetRelativeMouseState(&dx, &dy);
                    state.mouseMove.x += dx;
                    state.mouseMove.y += dy;
                }                
                break;

                case SDL_KEYDOWN:
                {
                    state.keyStates[e.key.keysym.scancode] = true;
                    switch(e.key.keysym.scancode){
                        case SDL_SCANCODE_W:
                        state.camera.move.front = true;
                        break;

                        case SDL_SCANCODE_A:
                        state.camera.move.left = true;
                        break;

                        case SDL_SCANCODE_D:
                        state.camera.move.right = true;
                        break;

                        case SDL_SCANCODE_S:
                        state.camera.move.back = true;
                        break;

                        case SDL_SCANCODE_Q:
                        state.camera.move.up = true;
                        break;

                        case SDL_SCANCODE_E:
                        state.camera.move.down = true;
                        break;
                    }
                    break;
                }
                case SDL_KEYUP:{
                    state.keyStates[e.key.keysym.scancode] = false;
                    switch(e.key.keysym.scancode){
                        case SDL_SCANCODE_W:
                        state.camera.move.front = false;
                        break;

                        case SDL_SCANCODE_A:
                        state.camera.move.left = false;
                        break;

                        case SDL_SCANCODE_D:
                        state.camera.move.right = false;
                        break;
                        
                        case SDL_SCANCODE_S:
                        state.camera.move.back = false;
                        break;

                        case SDL_SCANCODE_Q:
                        state.camera.move.up = false;
                        break;
                        
                        case SDL_SCANCODE_E:
                        state.camera.move.down = false;
                        break;
                    }
                    break;
                }
                default:
                //cout << "unsupported input" << endl;
                break; 
            }
        }
    }
};
