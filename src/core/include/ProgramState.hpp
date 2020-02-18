#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <array>

enum SpecularTypes{
	PHONG = 0,
	BLINNPHONG,
	GGX
};

enum RenderingMode{
	ALDEBO = 0,
	NORMALMAP,
	ROUGHNESS,
	HEIGHT
};

typedef struct {
	float factor;
	int type;
} LightingModel;

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
	} camera;
	struct {
		unsigned int before;
		unsigned int after;
	} time;
	std::array<unsigned char, SDL_NUM_SCANCODES> keyStates;
	unsigned long long int frame;
	struct {
		struct {
            float r;
            float g;
            float b;
            float a;
        } color;
	}window;
	struct{
		bool moveToCamera;
		glm::vec3 position;
		glm::vec4 color;
		struct{
			LightingModel ambient;
			LightingModel diffuse;
			struct{
				float factor;
				int type;
				float roughness;
				float fresnel;
				float metalicness;
			} specular;
		} model;
	} light;
	int mode;

	bool active;

	ProgramState(unsigned int currentMs):
		quit(false),
		mouseMove({0, 0}),
		camera({30, 1.0f, {0,0,0,0,0,0}, 0.001}),
		time({currentMs, currentMs}),
		frame(0),
		window({{0,0,0,0}}),
		light({false, glm::vec3(4,4,4), glm::vec4(1,1,1,1), {{0.3, 0}, {1,0}, {0.1,0, 0.5, 0.5, 0.04}}})
		{
			for(unsigned char& state : keyStates){
				state = false;
			}
		}
};