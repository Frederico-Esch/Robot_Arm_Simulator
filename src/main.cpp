#include <iostream>
#include <raylib.h>
#include <cmath>
#include <Space.h>
#include <Camera.h>
#include <Robot.h>
#include <array>

#define WIDTH 1900
#define HEIGHT 1060

int main() {

    InitWindow(WIDTH, HEIGHT, "Robot Arm");
    SetTargetFPS(60);
    HideCursor();
    DisableCursor();

    auto camera = Renderer::Camera({ 0, 5, 10 }, { 0, 0.5, 0 }, { 0, 1, 0 }, 10);

    //auto model = LoadModel("../models/braco_robotico.glb");
    //auto model = LoadModel("../models/ra/Joint2+.glb");
    //model.transform = Translate({-0.001, 0.203, 0.165 }) * RotateY(M_PI_2f) * RotateX(M_PI_2f) * Translate({0, 0, 0.11}) * RotateX(-M_PI_2f);

    auto robot = Robot::Robot(
        LoadModel("../models/ra/Base.glb"),
        {
            Robot::RotationalJoint(LoadModel("../models/pieces/Joint1.glb"), RotateX(-M_PI_2f)   * Translate({0, 0.11, 0})),
            Robot::RotationalJoint(LoadModel("../models/pieces/Joint2.glb"), RotateY(M_PI_2f)    * RotateX(M_PI_2f) * Translate({0.165, -0.001, 0.203})),
            Robot::RotationalJoint(LoadModel("../models/pieces/Joint3.glb"), RotateY(M_PIf)      * Translate({0.000186235, 0.82913, -0.031})),
            Robot::RotationalJoint(LoadModel("../models/pieces/Joint4.glb"), RotateY(M_PIf)      * Translate({0.00133151, 0.627261, 0.0190001})),
            Robot::RotationalJoint(LoadModel("../models/pieces/Joint5.glb"), RotateY(-M_PI_2f) * Translate({-0.174068, -0.00111473, 0.115})),
            Robot::RotationalJoint(LoadModel("../models/pieces/Joint6.glb"), RotateY(-M_PI_2f)   * Translate({0.255901, -0.000136077, 0.133636}))
        }
    );

    int currentJoint = 0;

        //TODO: socorr pq eu faço isso comigo mesmo
        //CheckCollisionBoxes
        //GetModelBoundingBox
    bool box = false;
    bool robot_origin = false;
    while (!WindowShouldClose()) {
        Renderer::Space space;
        space.render3D(camera);

        //auto T = robot.render();
        auto T = robot.render();
        if (box) robot.draw_box();
        robot.iter_animation();
        //auto iT = ~T;
        //DrawModelWires(model, {0, 0, 0}, 1, WHITE);

        //DrawPlane({0,-0.01,0}, {1, 1}, RED);

        if (IsKeyPressed(KEY_P)) {
            robot.set_animation({
                Robot::AnimationKeyFrame{{0, M_PI_2f}, {0, 0}, 1},
                Robot::AnimationKeyFrame{{0, M_PI_2f}, {0, 0}, 1},
                Robot::AnimationKeyFrame{{0, -M_PI_2f}, {0, 0}, 2},
                Robot::AnimationKeyFrame{{0, -M_PIf}, {0, 0}, 3},
                Robot::AnimationKeyFrame{{0, -M_PIf}, {0, 0}, 4},
                Robot::AnimationKeyFrame{{0, -M_PI_2f*10}, {0, 0}, 5}
            });
        }
        if (IsKeyPressed(KEY_T)) {
            robot.set_angles({0});
        }
        if (IsKeyPressed(KEY_G)) {
            robot.set_angles({M_PIf});
        }

        if (robot_origin) T = robot.transform(currentJoint);
        else T = M4Id();
        auto og = T * Vector3{0,0,0};
        DrawLine3D(og, T * Vector3{5, 0, 0}, RED);
        DrawLine3D(og, T * Vector3{0, 5, 0}, GREEN);
        DrawLine3D(og, T * Vector3{0, 0, 5}, BLUE);

        if (camera.isOrbiting()) camera.m_camera.target = T * Vector3{ 0, 0, 0 };

        if (IsKeyPressed(KEY_X)) { robot.set_angles(std::vector(robot.joint_count(), 0.0f)); }
        if (IsKeyPressed(KEY_M)) robot_origin = !robot_origin;
        if (IsKeyPressed(KEY_Z)) { camera.togleOrbiting(); }
        if (IsKeyPressed(KEY_B)) box = !box;

        if (IsKeyPressed(KEY_UP)) { currentJoint = (currentJoint + 1) % robot.joint_count(); }
        if (IsKeyPressed(KEY_DOWN)) { currentJoint = currentJoint -1; if (currentJoint < 0) currentJoint = robot.joint_count() -1; }
        if (IsKeyPressed(KEY_LEFT)) { robot.rotate(currentJoint, -0.1); }
        if (IsKeyPressed(KEY_RIGHT)) { robot.rotate(currentJoint, 0.1); }
    }

    robot.destroy();
    CloseWindow();

    return 0;
}

////first joint
//Vector3 v1 = {0,0,0}, v2 = {0,0,a1};
//Matrix T = t01;
//DrawCylinderEx(T * v1, T * v2, 0.07, 0.07, 20, BLUE);
//T = RotateZ(theta[0]) * T;

////second joint
//v1 = {0,0,-0.1}, v2 = {0,0,0.1};
//T = t12 * T;
//DrawCylinderEx(T * v1, T * v2, 0.05, 0.05, 20, GREEN);
//T = RotateZ(theta[1]) * T;

//v1 = {0,0,0}, v2 = {0.3,0,0};
//DrawCylinderEx(T * v1, T * v2, 0.07, 0.07, 20, GREEN);

//--------

//std::array<Model, 2> joints = {
//    LoadModel("../models/ra/Joint1.glb"),
//    LoadModel("../models/ra/Joint2+.glb")
//};
//std::array<Matrix, 2> jointTransforms = {
//    Translate({0, 0, 0.11}) * RotateX(-M_PI_2f),
//    Translate({-0.001, 0.203, 0.165 }) * RotateY(M_PI_2f) * RotateX(M_PI_2f)
//    //Translate({0.00, 0.203, 0.165 }) * RotateY(M_PI_2f) * RotateX(M_PI_2f)
//};
//std::array<float, 2> theta = {};


//T = RotateZ(theta[0]) * jointTransforms[0] * T;
//DrawJoint(joints[0], T);
//T = jointTransforms[1] * T;
//DrawJoint(joints[1], T);

//
//if (IsKeyPressed(KEY_B)) {
//    if (dx < 0.005) dx = 0.01;
//    else dx = 0.001;
//    if (IsKeyDown(KEY_LEFT_SHIFT)) dx = 0.0001;
//}
//if (IsKeyPressed(KEY_I)) translation.z -= dx;
//if (IsKeyPressed(KEY_K)) translation.z += dx;
//if (IsKeyPressed(KEY_J)) translation.x -= dx;
//if (IsKeyPressed(KEY_L)) translation.x += dx;
//if (IsKeyPressed(KEY_U)) translation.y += dx;
//if (IsKeyPressed(KEY_O)) translation.y -= dx;
//
//if (IsKeyPressed(KEY_P)) std::cout << "X: " << translation.x << ", Y: " << translation.y << ", Z: " << translation.z << std::endl;
