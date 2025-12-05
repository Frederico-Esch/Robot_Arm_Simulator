#include <Robot.h>
#include <iostream>
#include <memory>

Robot::Robot::Robot(Model base)
{
    m_base = base;
}

Robot::Robot::Robot(Model base, std::initializer_list<RotationalJoint> joints)
{
    m_base = base;
    m_joints = joints;
}

void Robot::Robot::add_joint(RotationalJoint joint)
{
    m_joints.push_back(joint);
}

Matrix Robot::Robot::render()
{
    auto T = M4Id();
    DrawModel(m_base, { 0, 0, 0 }, 1, WHITE);

    for (auto& joint : m_joints) {
        joint.render(T);
        T = joint.transform() * T;
    }

    return T;
}

void Robot::Robot::draw_box()
{
    std::array<Color, 8> colors = { WHITE, GREEN, PINK, BLUE, YELLOW, PURPLE, ORANGE, RED };

    auto T = M4Id();
    for (int i = 0; i < m_joints.size(); i++) {
        m_joints[i].draw_box(T, colors[i % colors.size()]);
        T = m_joints[i].transform() * T;
    }
}

Matrix Robot::Robot::transform(int joint)
{
    auto T = M4Id();
    if (joint < 0 || joint >= m_joints.size())
        return T;

    for (int i = 0; i <= joint; i++) {
        T = m_joints[i].transform() * T;
    }

    return T;
}

void Robot::Robot::set_animation(std::vector<AnimationKeyFrame> animation)
{
    m_has_animation = std::vector(animation.size(), false);
    for (int i = 0; i < animation.size(); i++) {
        m_has_animation[i] = !animation[i].is_empty();
    }
    m_currentAnimation = animation;
    m_t = 0;
}

void Robot::Robot::iter_animation()
{
    auto size = std::min(m_joints.size(), m_has_animation.size());
    for (int anim = 0; anim < size; anim++) {
        if (!m_has_animation[anim])
            continue;

        auto& current = m_currentAnimation[anim];
        if (m_t > current.dt()) {
            m_has_animation[anim] = false;
            return;
        }

        // QUITIC NOT IMPLEMENTED
        auto coefs = current.coefs();
        auto p = coefs[0] + coefs[1] * m_t + coefs[2] * m_t * m_t + coefs[3] * m_t * m_t * m_t;
        m_joints[anim].set_angle(p);
    }
    auto dt = GetFrameTime();
    m_t += dt;
}

int Robot::Robot::joint_count() { return m_joints.size(); }

void Robot::Robot::rotate(int joint, float angle)
{
    m_joints[joint].rotate(angle);
}

void Robot::Robot::set_angles(std::vector<float> angles)
{
    int count = std::min(angles.size(), m_joints.size());
    for (int i = 0; i < count; i++) {
        m_joints[i].set_angle(*(angles.begin() + i));
    }
}

void Robot::Robot::destroy()
{
    UnloadModel(m_base);
    while (m_joints.size()) {
        m_joints.back().destroy();
        m_joints.pop_back();
    }
}

std::vector<float> Robot::AnimationKeyFrame::coefs()
{
    if (m_calculated) {
        return (m_equation == AKFE_CUBIC) ? std::vector<float>({ m_a[0], m_a[1], m_a[2], m_a[3] }) : std::vector<float>({ m_a[0], m_a[1], m_a[2], m_a[3], m_a[4], m_a[5] });
    }
    m_calculated = true;

    if (m_equation == AKFE_CUBIC) {
        auto t = m_delta_time;
        auto t2 = t * t;
        auto t3 = t * t * t;
        auto mat = Matrix {
            1, 0, 0, 0,
            0, 1, 0, 0,
            1, t, t2, t3,
            0, 1, 2 * t, 3 * t2
        };
        auto p = Vector4 { m_positions[0], m_velocities[0], m_positions[1], m_velocities[1] };
        auto imat = (~mat);
        // std::cout <<
        //     imat.m0 << " " << imat.m4 << " " << imat.m8  << " " << imat.m12 << std::endl <<
        //     imat.m1 << " " << imat.m5 << " " << imat.m9  << " " << imat.m13 << std::endl <<
        //     imat.m2 << " " << imat.m6 << " " << imat.m10 << " " << imat.m14 << std::endl <<
        //     imat.m3 << " " << imat.m7 << " " << imat.m11 << " " << imat.m15 << std::endl;
        auto coefs = imat * p;
        m_a[0] = coefs.x;
        m_a[1] = coefs.y;
        m_a[2] = coefs.z;
        m_a[3] = coefs.w;
        // std::cout << m_a[0] << " " << m_a[1] << " " << m_a[2] << " " << m_a[3] << std::endl;
        return { m_a[0], m_a[1], m_a[2], m_a[3] };
    } else {
        throw std::runtime_error("Not implemented");
    }
}

float Robot::AnimationKeyFrame::dt() { return m_delta_time; }
bool Robot::AnimationKeyFrame::is_empty() { return m_delta_time < 0; }

// static bool CheckCollision(const Box& b1, const Matrix& m1, const Box& b2, const Matrix& m2) {
//     bool xless = true;
//     bool xmore = true;
//     bool yless = true;
//     bool ymore = true;
//     bool zless = true;
//     bool zmore = true;
//
//     for (int i = 0; i < 8; i++) {
//         for (int j = 0; j < 8; j++) {
//             auto p = (m2 * b2.points[j]) - (m1 * b1.points[i]);
//             xless = xless && p.x < 0;
//             xmore = xmore && p.x >= 0;
//             yless = yless && p.y < 0;
//             ymore = ymore && p.y >= 0;
//             zless = zless && p.z < 0;
//             zmore = zmore && p.z >= 0;
//         }
//     }
//
//     bool collided = !(xless || xmore || yless || ymore || zless || zmore);
//     static int times = 0;
//     if (collided) {
//         for (int i = 0; i < 8; i++) {
//             for (int j = 0; j < 8; j++) {
//                 auto p = (m2 * b2.points[j]) - (m1 * b1.points[i]);
//                 std::cout << "TIMES: " << times << ": " << p.x << ", " << p.y << ", " << p.z << std::endl;
//             }
//         }
//         std::cout << std::endl << std::endl;
//         times++;
//     }
//     return collided;
// }
