#pragma once

#include <vector>
#include <initializer_list>
#include <stdexcept>
#include <RotationalJoint.h>

namespace Robot {
    class AnimationKeyFrame {
        enum AnimationKeyFrameEquation { AKFE_NONE, AKFE_CUBIC, AKFE_QUINTIC };
        std::array<float, 2> m_positions;
        std::array<float, 2> m_velocities;
        std::array<float, 2> m_accelerations;
        float m_delta_time;
        bool m_calculated = false;
        std::array<float, 6> m_a;
        AnimationKeyFrameEquation m_equation = AKFE_NONE;

        public:
        AnimationKeyFrame(
                std::array<float, 2> positions,
                std::array<float, 2> velocities,
                float time): m_positions(positions), m_velocities(velocities), m_equation(AKFE_CUBIC), m_delta_time(time)
        { }

        AnimationKeyFrame(
                std::array<float, 2> positions,
                std::array<float, 2> velocities,
                std::array<float, 2> accelerations,
                float time): m_positions(positions), m_velocities(velocities), m_accelerations(accelerations), m_equation(AKFE_QUINTIC), m_delta_time(time)
        { throw std::runtime_error("Not implemented"); }

        AnimationKeyFrame() : m_delta_time(-1)
        {}

        std::vector<float> coefs();
        float dt();
        bool is_empty();
    };

    class Robot {
        Model m_base;
        std::vector<RotationalJoint> m_joints;
        std::vector<bool> m_has_animation;
        std::vector<AnimationKeyFrame> m_currentAnimation;
        float m_t;

    public:
        Robot(Model base);
        Robot(Model base, std::initializer_list<RotationalJoint> joints);
        void add_joint(RotationalJoint joint);
        void rotate(int joint, float angle);
        void set_angles(std::vector<float> angles);
        int joint_count();
        Matrix render();
        void draw_box();
        void set_animation(std::vector<AnimationKeyFrame> animation);
        void iter_animation();
        Matrix transform(int joint = -1);
        void destroy();
    };

}
