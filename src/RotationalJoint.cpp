#include <RotationalJoint.h>
#include <stdexcept>

static Box GetBox(Mesh mesh) {
    auto bounds = Box{
        Vector3{ mesh.vertices[0], mesh.vertices[1], mesh.vertices[2] },
        Vector3{ mesh.vertices[0], mesh.vertices[1], mesh.vertices[2] },
        Vector3{ mesh.vertices[0], mesh.vertices[1], mesh.vertices[2] },
        Vector3{ mesh.vertices[0], mesh.vertices[1], mesh.vertices[2] },
        Vector3{ mesh.vertices[0], mesh.vertices[1], mesh.vertices[2] },
        Vector3{ mesh.vertices[0], mesh.vertices[1], mesh.vertices[2] },
        Vector3{ mesh.vertices[0], mesh.vertices[1], mesh.vertices[2] },
        Vector3{ mesh.vertices[0], mesh.vertices[1], mesh.vertices[2] },
    };


    for (int i = 1; i < mesh.vertexCount; i++)
    {
        auto tempVert = Vector3{mesh.vertices[i*3], mesh.vertices[i*3 + 1], mesh.vertices[i*3 + 2]};
        for (int i = 0; i < 8; i++) {
            float x = ((i & 0b100) == 0) ? std::min(tempVert.x, bounds.points[i].x) :  std::max(tempVert.x, bounds.points[i].x);
            float y = ((i & 0b010) == 0) ? std::min(tempVert.y, bounds.points[i].y) :  std::max(tempVert.y, bounds.points[i].y);
            float z = ((i & 0b001) == 0) ? std::min(tempVert.z, bounds.points[i].z) :  std::max(tempVert.z, bounds.points[i].z);

            bounds.points[i] = Vector3{ x, y, z };
        }
    }

    return bounds;
}

static Box GetBox(Model model) {
    auto bounds = GetBox(model.meshes[0]);

    for (int i = 1; i < model.meshCount; i++)
        bounds = BoxCombine(bounds, GetBox(model.meshes[i]));

    return bounds;
}

static void ShrinkBox(Box& box, Matrix T = M4Id()) {
    auto middle = Vector3{0,0,0};

    for (int i = 0; i < 8; i++) {
        box.points[i] = T * box.points[i];
        middle += box.points[i];
    }
    middle *=  (1/8.0f);


    for (int i = 0; i < 8; i++) {
        auto diff = (middle - box.points[i]);
        diff *= 0.2;
        box.points[i] += diff;
    }
}

static void DrawBox(Box box, Color color, Matrix T = M4Id()) {
    for (int i = 0; i < 8; i++) {
        auto p = T * box.points[i];
        DrawSphere(p, 0.01, color);
    }

    DrawLine3D(T * box.points[0b000], T * box.points[0b001], color);
    DrawLine3D(T * box.points[0b000], T * box.points[0b010], color);
    DrawLine3D(T * box.points[0b000], T * box.points[0b100], color);

    DrawLine3D(T * box.points[0b101], T * box.points[0b001], color);
    DrawLine3D(T * box.points[0b101], T * box.points[0b100], color);

    DrawLine3D(T * box.points[0b100], T * box.points[0b110], color);
    DrawLine3D(T * box.points[0b001], T * box.points[0b011], color);

    DrawLine3D(T * box.points[0b010], T * box.points[0b011], color);
    DrawLine3D(T * box.points[0b010], T * box.points[0b110], color);

    DrawLine3D(T * box.points[0b111], T * box.points[0b011], color);
    DrawLine3D(T * box.points[0b111], T * box.points[0b101], color);
    DrawLine3D(T * box.points[0b111], T * box.points[0b110], color);
}

Robot::RotationalJoint::RotationalJoint(Model model, Matrix transform) {
    m_transform = transform * model.transform;
    m_model = true;
    m_model_ref = model;
    m_rotation = RotateZ(m_theta);

    m_meshes = std::vector<Mesh>(model.meshCount);
    m_meshMaterial = std::vector<int>(model.meshCount);
    for (int i = 0; i < model.meshCount; i++) {
        m_meshMaterial[i] = model.meshMaterial[i];
        m_meshes[i] = model.meshes[i];
    }
    m_materials = std::vector<Material>(model.materialCount);
    for (int i = 0; i < model.materialCount; i++) { m_materials[i] = model.materials[i]; }
}

Robot::RotationalJoint::RotationalJoint(Robot::RenderFunction renderFunction, Matrix transform) {
    m_transform = transform;
    m_renderFunc = renderFunction;
    m_model = false;
    m_rotation = RotateZ(m_theta);
}

void Robot::RotationalJoint::render(Matrix previous_transform) {
    auto T = transform() * previous_transform;
    if (m_model) {
        int i = 0;
        for (auto& mesh : m_meshes) {
            DrawMesh(mesh, m_materials[m_meshMaterial[i++]], T);
        }
    }
    else {
        if (m_renderFunc != nullptr) m_renderFunc(T);
    }
}

Box Robot::RotationalJoint::get_box() {
    auto b = GetBox(m_model_ref);
    ShrinkBox(b);
    return b;
}

void Robot::RotationalJoint::draw_box(Matrix previous_transform, Color color) {
    auto T = transform() * previous_transform;
    auto b = GetBox(m_model_ref);
    ShrinkBox(b);
    DrawBox(b, color, T);
}


Matrix Robot::RotationalJoint::transform() { return m_rotation * m_transform; }

void Robot::RotationalJoint::rotate(float angle) { m_theta += angle; m_rotation = RotateZ(m_theta); }

void Robot::RotationalJoint::set_angle(float angle) { m_theta = angle; m_rotation = RotateZ(m_theta); }

void Robot::RotationalJoint::destroy() { if (m_model) UnloadModel(m_model_ref); m_model = false; }
