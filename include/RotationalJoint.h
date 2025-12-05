#pragma once
#include <LineAlg.h>
#include <functional>
#include <memory>

namespace Robot {
    using RenderFunction = std::function<void(Matrix)>;
    class RotationalJoint {
        float m_theta = 0;
        Matrix m_rotation;
        Matrix m_transform;

        RenderFunction m_renderFunc = nullptr;
        bool m_model = false;
        Model m_model_ref;
        std::vector<Mesh> m_meshes;
        std::vector<Material> m_materials;
        std::vector<int> m_meshMaterial;

    public:
        RotationalJoint(Model model, Matrix transform);
        RotationalJoint(RenderFunction render, Matrix transform);
        void render(Matrix previous_transform);
        Box get_box();
        void draw_box(Matrix previous_transform, Color color = GREEN);
        void rotate(float angle);
        void set_angle(float angle);
        Matrix transform();
        void destroy();
    };
}
