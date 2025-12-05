#include <Space.h>

Renderer::Space3D::Space3D(const Renderer::Camera& camera) {
    BeginMode3D(camera.m_camera);
}

Renderer::Space3D::~Space3D() {
    EndMode3D();
}

Renderer::Space::Space(Color background) {
    BeginDrawing();
    ClearBackground(background);
}

Renderer::Space::~Space() {
    if (space3D != nullptr){ delete space3D; }
    space3D = nullptr;
    EndDrawing();
}

void Renderer::Space::render3D(Renderer::Camera& camera) {
    camera.update();
    space3D = new Renderer::Space3D(camera);
}

void Renderer::Space::cube3D(Vector3 position, int width, int height, int length, Color color) {
    DrawCube(position, width, height, length, color);
}

