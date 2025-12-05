#pragma once

#include <raylib.h>

namespace Renderer {
    class Camera {
        private:
            bool m_orbiting = false;
        public:
            Camera3D m_camera;
            void update();
            inline Vector3 getForward() const;
            inline Vector3 getUp() const;
            inline Vector3 getRight() const;
            void togleOrbiting();
            bool isOrbiting();
            Camera(Vector3 position, Vector3 target, Vector3 up, float fovy);
    };
}
