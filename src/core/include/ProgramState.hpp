#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <array>

struct ProgramState {
	bool quit;
	struct {
		int x;
		int y;
	} mouseMove;
	struct{
		int 		maxAngle;
		float 		sensitivity;
		struct {
			char front : 1;
			char back : 1;
			char left : 1;
			char right : 1;
			char up : 1;
			char down : 1;
		} move;
		float velocity;
		bool active;
	} camera;
	struct {
		unsigned int before;
		unsigned int after;
	} time;
	std::array<unsigned char, SDL_NUM_SCANCODES> keyStates;

	ProgramState(unsigned int currentMs):
		quit(false),
		mouseMove({0, 0}),
		camera({30, 1.0f, {0,0,0,0,0,0}, 0.001}),
		time({currentMs, currentMs}){
			for(unsigned char& state : keyStates){
				state = false;
			}
		}
};