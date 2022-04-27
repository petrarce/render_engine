#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <ProgramState.hpp>
#include <types.hpp>

struct Camera
{
    Camera(glm::vec3 position, glm::vec3 lookAt): 
        Position(position),
        LookAt(lookAt),
        LookUp(glm::vec3(0,0,1)){}

    const glm::mat4 getOrthProjection() const
    {
        return glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 500.0f);
    }
    const glm::vec3 getDirection() const
    {
        return LookAt;
    }

    const glm::mat4 getProjection() const {
        return glm::perspective( glm::radians(45.0f), 1.33f, 0.1f, 500.0f );
    }

    const glm::mat4 getView() const {
        return glm::lookAt(Position, Position + LookAt, LookUp);
    }

    void update(struct ProgramState& state){
        if(!state.active){
            return;
        }
        rotateCamera(state.mouseMove.x, 
                    state.mouseMove.y, 
                    state.camera.maxAngle, 
                    state.camera.sensitivity);
        moveCamera(state.camera.move.front, 
                   state.camera.move.back,
                   state.camera.move.left,
                   state.camera.move.right,
                   state.camera.move.up,
                   state.camera.move.down,
                   state.camera.velocity,
                   state.time.after - state.time.before);
        if(state.light.moveToCamera){
            state.light.position = this->Position;
            state.light.moveToCamera = false;
        }
    }
    glm::vec3 getPosition()
    {
        return Position;
    }
private:
    void clamp(int& val, int clamp){
        val = min(clamp, abs(val)) * ((val<0) ? -1:1);
    }
    void moveCamera(bool foreward, 
               bool backward,
               bool left,
               bool right,
               bool up,
               bool down,
               float velocity,
               float dt)
    {
        glm::vec3 leftDir = glm::normalize(glm::cross(LookUp, LookAt));
        glm::vec3 upDir   = glm::normalize(glm::cross(LookAt, leftDir));
        Position += dt * velocity * left * leftDir;
        Position -= dt * velocity * right * leftDir;
        Position += dt * velocity * foreward * normalize(LookAt);
        Position -= dt * velocity * backward * normalize(LookAt);
        Position += dt * velocity * up * upDir;
        Position -= dt * velocity * down * upDir;
    }
    void rotateCamera(int dx, int dy, int maxAngle, float sensitivity){
        //no need to rotate view and alternative vectors
        if(dx + dy == 0){
            return;
        }
        clamp(dx, 30);
        clamp(dy, 30);
        auto Tangent = glm::normalize(glm::cross(LookUp, LookAt));
        auto Bitangent = glm::normalize(glm::cross(Tangent, LookAt));
        auto Normal = glm::normalize(LookAt);

        constexpr float degreToRad = 3.14 / 360.0f;
        float dxAngle = dx * degreToRad * sensitivity;
        float dyAngle = dy * degreToRad * sensitivity;

        auto n1 = glm::mat3(glm::rotate(glm::mat4(1.0f), dxAngle, Bitangent)) * Normal;
        auto n2 = glm::mat3(glm::rotate(glm::mat4(1.0f), dyAngle, Tangent)) * Normal;

        auto newNormal = normalize(glm::normalize(n1 + n2));
        auto axe = glm::cross(Normal, newNormal);
        float angle = acos(dot(newNormal, Normal));
        auto transl = glm::mat3(glm::rotate(glm::mat4(1.0f), angle, axe));

        LookAt = glm::normalize(transl * LookAt);
        LookUp = glm::normalize(transl * LookUp);

    }
    glm::vec3 LookUp;
    glm::vec3 LookAt;
    glm::vec3 Position;

};
