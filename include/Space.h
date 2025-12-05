#pragma once

#include <raylib.h>
#include <Camera.h>

namespace Renderer {
    class Space3D {
        public:
            Space3D(const Renderer::Camera& camera);
            ~Space3D();
    };

    class Space {
        private:
            Space3D* space3D = nullptr;
        public:
            Space(Color background = BLACK);
            void render3D(Renderer::Camera& camera);
            void cube3D(Vector3 position, int width, int height, int length, Color color);
            ~Space();
    };
}
