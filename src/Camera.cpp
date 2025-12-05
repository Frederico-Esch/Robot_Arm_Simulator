#include <Camera.h>
#include <LineAlg.h>
#include <iostream>

static float CAMERA_MOVE_SPEED = 5.5f;
static float CAMERA_ROT_SPEED = 0.03f;
Renderer::Camera::Camera(Vector3 position, Vector3 target, Vector3 up, float fovy) : m_camera { position, target, up, fovy, CAMERA_PERSPECTIVE } {
}

inline Vector3 Renderer::Camera::getForward() const { return ~(m_camera.target - m_camera.position); }

inline Vector3 Renderer::Camera::getUp() const { return ~(m_camera.up); }

inline Vector3 Renderer::Camera::getRight() const { return ~(getForward() * getUp()); }

void Renderer::Camera::togleOrbiting() { m_orbiting = !m_orbiting; }

bool Renderer::Camera::isOrbiting() { return m_orbiting; }

void Renderer::Camera::update() {
    auto moveSpeed = CAMERA_MOVE_SPEED * GetFrameTime();
    auto rotSpeed = CAMERA_ROT_SPEED * GetFrameTime();
    auto mouseDelta = GetMouseDelta();
    if (std::abs(mouseDelta.x) > 150 || std::abs(mouseDelta.y) > 150) {
        mouseDelta.x = 0; mouseDelta.y = 0;
    }

    {
        auto rightSpeed = 0;
        if (IsKeyDown(KEY_A)) rightSpeed = -1;
        else if (IsKeyDown(KEY_D)) rightSpeed = 1;

        auto right = getRight() * (rightSpeed*moveSpeed * 0.2);
        m_camera.position = m_camera.position + right;
        if (!m_orbiting) m_camera.target = m_camera.target + right;
    }

    {
        auto forwardSpeed = 0;
        if (IsKeyDown(KEY_S)) forwardSpeed = -1;
        else if (IsKeyDown(KEY_W)) forwardSpeed = 1;
        auto forward = getForward() * (forwardSpeed*moveSpeed);
        m_camera.position += forward;
        if (!m_orbiting) m_camera.target += forward;
    }

    {
        auto upSpeed = 0;
        if (IsKeyDown(KEY_LEFT_CONTROL)) upSpeed = -1;
        else if (IsKeyDown(KEY_SPACE)) upSpeed = 1;
        auto up = getUp() * (upSpeed*moveSpeed*0.2);
        m_camera.position = m_camera.position + up;
        if (!m_orbiting) m_camera.target = m_camera.target + up;
    }

    if (!m_orbiting)
    {
        auto targetPos = m_camera.target - m_camera.position;
        m_camera.target = m_camera.position + V3Rotate(targetPos, getUp(), -mouseDelta.x*rotSpeed);
    }

    if (!m_orbiting)
    {
        auto targetPos = m_camera.target - m_camera.position;
        m_camera.target = m_camera.position + V3Rotate(targetPos, getRight(), -mouseDelta.y*rotSpeed);
    }

    bool shift = IsKeyDown(KEY_LEFT_SHIFT);
    float plus_minus = 0;
    if (IsKeyPressed(KEY_EQUAL)) plus_minus = 1;
    else if (IsKeyPressed(KEY_MINUS)) plus_minus = -1;

    CAMERA_ROT_SPEED += 0.01*plus_minus*shift;
    if (CAMERA_ROT_SPEED < 0.01) CAMERA_ROT_SPEED = 0.01;
    CAMERA_MOVE_SPEED += 0.5*plus_minus*!shift;
    if (CAMERA_MOVE_SPEED < 0.5) CAMERA_MOVE_SPEED = 0.5;
}
