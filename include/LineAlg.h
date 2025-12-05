#pragma once

#include <raylib.h>
#include <cmath>

#pragma region Vector

inline Vector3 V3Subtract(const Vector3& v1, const Vector3& v2) { return { v1.x - v2.x, v1.y - v2.y, v1.z - v2.z, }; }

inline Vector3 V3Add(const Vector3& v1, const Vector3& v2) { return { v1.x + v2.x, v1.y + v2.y, v1.z + v2.z, }; }

inline Vector3 V3Scale(const Vector3& v, const float& s) { return { v.x*s, v.y*s, v.z*s }; }

inline Vector3 V3Cross(const Vector3& v1, const Vector3& v2) { return { v1.y*v2.z - v1.z*v2.y, v1.z*v2.x - v1.x*v2.z, v1.x*v2.y - v1.y*v2.x }; }

inline Vector3 V3Normalize(const Vector3& v) {
    Vector3 result = v;

    float length = sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
    if (length != 0.0f)
    {
        float ilength = 1.0f/length;

        result.x *= ilength;
        result.y *= ilength;
        result.z *= ilength;
    }

    return result;
}

inline Vector3 V3Rotate(const Vector3& v, const Vector3& a, float angle) {
    angle /= 2.0f;
    auto w = V3Scale(a, std::sin(angle));
    auto wv = V3Cross(w, v);
    auto wwv = V3Cross(w, wv);
    wv = V3Scale(wv, 2*std::cos(angle));
    wwv = V3Scale(wwv, 2);

    return V3Add(wv, V3Add(wwv, v));
}

inline Vector3 V3Transform(const Vector3& v, const Matrix& m) {
    float x = v.x;
    float y = v.y;
    float z = v.z;

    return {
        m.m0*x + m.m4*y + m.m8*z + m.m12,
        m.m1*x + m.m5*y + m.m9*z + m.m13,
        m.m2*x + m.m6*y + m.m10*z + m.m14
    };
}

inline Vector4 V4MultMat(const Vector4& v, const Matrix& m) {
    float x = v.x;
    float y = v.y;
    float z = v.z;
    float w = v.w;

    return {
        m.m0*x + m.m4*y + m.m8*z + m.m12 * w,
        m.m1*x + m.m5*y + m.m9*z + m.m13 * w,
        m.m2*x + m.m6*y + m.m10*z + m.m14 * w,
        m.m3*x + m.m7*y + m.m11*z + m.m15 * w
    };
}


inline Vector3 operator - (const Vector3& lhs, const Vector3& rhs)
{
    return V3Subtract(lhs, rhs);
}

inline const Vector3& operator -= (Vector3& lhs, const Vector3& rhs)
{
    lhs = V3Subtract(lhs, rhs);
    return lhs;
}

inline Vector3 operator + (const Vector3& lhs, const Vector3& rhs)
{
    return V3Add(lhs, rhs);
}

inline const Vector3& operator += (Vector3& lhs, const Vector3& rhs)
{
    lhs = V3Add(lhs, rhs);
    return lhs;
}

inline Vector3 operator * (const Vector3& lhs, const Vector3& rhs)
{
    return V3Cross(lhs, rhs);
}

inline const Vector3& operator *= (Vector3& lhs, const Vector3& rhs)
{
    lhs = V3Cross(lhs, rhs);
    return lhs;
}

inline const Vector3& operator *= (Vector3& lhs, const Matrix& rhs)
{
    lhs = V3Transform(lhs, rhs);
    return lhs;
}

inline Vector3 operator * (const Vector3& lhs, const float& rhs)
{
    return V3Scale(lhs, rhs);
}

inline const Vector3& operator *= (Vector3& lhs, const float& rhs)
{
    lhs = V3Scale(lhs, rhs);
    return lhs;
}

inline Vector3 operator~(const Vector3& lhs) {
    return V3Normalize(lhs);
}

#pragma endregion

#pragma region Matrix

inline Matrix RotateX(const float& angle) {
    return {
        1, 0              , 0               , 0,
        0, std::cos(angle), -std::sin(angle), 0,
        0, std::sin(angle), std::cos(angle) , 0,
        0, 0              , 0               , 1
    };
}

inline Matrix RotateY(const float& angle) {
    return {
        std::cos(angle) , 0 , std::sin(angle) , 0,
        0               , 1 , 0               , 0,
        -std::sin(angle), 0 , std::cos(angle) , 0,
        0               , 0 , 0               , 1
    };
}

inline Matrix RotateZ(const float& angle) {
    return {
        std::cos(angle), -std::sin(angle), 0, 0,
        std::sin(angle), std::cos(angle), 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    };
}

inline Matrix Translate(const Vector3 v) { return { 1, 0, 0, v.x, 0, 1, 0, v.y, 0, 0, 1, v.z, 0, 0, 0, 1}; }

inline Matrix M4Mult(const Matrix& m1, const Matrix& m2) {
    Matrix result = {};

    result.m0 = m1.m0*m2.m0 + m1.m1*m2.m4 + m1.m2*m2.m8 + m1.m3*m2.m12;
    result.m1 = m1.m0*m2.m1 + m1.m1*m2.m5 + m1.m2*m2.m9 + m1.m3*m2.m13;
    result.m2 = m1.m0*m2.m2 + m1.m1*m2.m6 + m1.m2*m2.m10 + m1.m3*m2.m14;
    result.m3 = m1.m0*m2.m3 + m1.m1*m2.m7 + m1.m2*m2.m11 + m1.m3*m2.m15;
    result.m4 = m1.m4*m2.m0 + m1.m5*m2.m4 + m1.m6*m2.m8 + m1.m7*m2.m12;
    result.m5 = m1.m4*m2.m1 + m1.m5*m2.m5 + m1.m6*m2.m9 + m1.m7*m2.m13;
    result.m6 = m1.m4*m2.m2 + m1.m5*m2.m6 + m1.m6*m2.m10 + m1.m7*m2.m14;
    result.m7 = m1.m4*m2.m3 + m1.m5*m2.m7 + m1.m6*m2.m11 + m1.m7*m2.m15;
    result.m8 = m1.m8*m2.m0 + m1.m9*m2.m4 + m1.m10*m2.m8 + m1.m11*m2.m12;
    result.m9 = m1.m8*m2.m1 + m1.m9*m2.m5 + m1.m10*m2.m9 + m1.m11*m2.m13;
    result.m10 = m1.m8*m2.m2 + m1.m9*m2.m6 + m1.m10*m2.m10 + m1.m11*m2.m14;
    result.m11 = m1.m8*m2.m3 + m1.m9*m2.m7 + m1.m10*m2.m11 + m1.m11*m2.m15;
    result.m12 = m1.m12*m2.m0 + m1.m13*m2.m4 + m1.m14*m2.m8 + m1.m15*m2.m12;
    result.m13 = m1.m12*m2.m1 + m1.m13*m2.m5 + m1.m14*m2.m9 + m1.m15*m2.m13;
    result.m14 = m1.m12*m2.m2 + m1.m13*m2.m6 + m1.m14*m2.m10 + m1.m15*m2.m14;
    result.m15 = m1.m12*m2.m3 + m1.m13*m2.m7 + m1.m14*m2.m11 + m1.m15*m2.m15;

    return result;
}

inline Matrix M4Invert(Matrix m) {
    Matrix result = { 0 };
    float a00 = m.m0, a01 = m.m1, a02 = m.m2, a03 = m.m3;
    float a10 = m.m4, a11 = m.m5, a12 = m.m6, a13 = m.m7;
    float a20 = m.m8, a21 = m.m9, a22 = m.m10, a23 = m.m11;
    float a30 = m.m12, a31 = m.m13, a32 = m.m14, a33 = m.m15;

    float b00 = a00*a11 - a01*a10;
    float b01 = a00*a12 - a02*a10;
    float b02 = a00*a13 - a03*a10;
    float b03 = a01*a12 - a02*a11;
    float b04 = a01*a13 - a03*a11;
    float b05 = a02*a13 - a03*a12;
    float b06 = a20*a31 - a21*a30;
    float b07 = a20*a32 - a22*a30;
    float b08 = a20*a33 - a23*a30;
    float b09 = a21*a32 - a22*a31;
    float b10 = a21*a33 - a23*a31;
    float b11 = a22*a33 - a23*a32;

    // Calculate the invert determinant (inlined to avoid double-caching)
    float invDet = 1.0f/(b00*b11 - b01*b10 + b02*b09 + b03*b08 - b04*b07 + b05*b06);

    result.m0 = (a11*b11 - a12*b10 + a13*b09)*invDet;
    result.m1 = (-a01*b11 + a02*b10 - a03*b09)*invDet;
    result.m2 = (a31*b05 - a32*b04 + a33*b03)*invDet;
    result.m3 = (-a21*b05 + a22*b04 - a23*b03)*invDet;
    result.m4 = (-a10*b11 + a12*b08 - a13*b07)*invDet;
    result.m5 = (a00*b11 - a02*b08 + a03*b07)*invDet;
    result.m6 = (-a30*b05 + a32*b02 - a33*b01)*invDet;
    result.m7 = (a20*b05 - a22*b02 + a23*b01)*invDet;
    result.m8 = (a10*b10 - a11*b08 + a13*b06)*invDet;
    result.m9 = (-a00*b10 + a01*b08 - a03*b06)*invDet;
    result.m10 = (a30*b04 - a31*b02 + a33*b00)*invDet;
    result.m11 = (-a20*b04 + a21*b02 - a23*b00)*invDet;
    result.m12 = (-a10*b09 + a11*b07 - a12*b06)*invDet;
    result.m13 = (a00*b09 - a01*b07 + a02*b06)*invDet;
    result.m14 = (-a30*b03 + a31*b01 - a32*b00)*invDet;
    result.m15 = (a20*b03 - a21*b01 + a22*b00)*invDet;

    return result;
}


inline Vector4 operator * (const Matrix& lhs, const Vector4& rhs)
{
    return V4MultMat(rhs, lhs);
}

inline Vector3 operator * (const Matrix& lhs, const Vector3& rhs)
{
    return V3Transform(rhs, lhs);
}

inline Matrix operator*(const Matrix& lhs, const Matrix& rhs) {
    return M4Mult(lhs, rhs);
}

inline Matrix operator~(const Matrix& lhs) { return M4Invert(lhs); }

inline Matrix M4Id() { return { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 }; }

#pragma endregion

#pragma region Box

struct Box { Vector3 points[8]; };

inline Box BoxCombine(const Box& b1, const Box& b2) {

    Box result;
    for (int i = 0; i < 8; i++) {
        float x = ((i & 0b100) == 0) ? std::min(b1.points[i].x, b2.points[i].x) :  std::max(b1.points[i].x, b2.points[i].x);
        float y = ((i & 0b010) == 0) ? std::min(b1.points[i].y, b2.points[i].y) :  std::max(b1.points[i].y, b2.points[i].y);
        float z = ((i & 0b001) == 0) ? std::min(b1.points[i].z, b2.points[i].z) :  std::max(b1.points[i].z, b2.points[i].z);

        result.points[i] = Vector3{ x, y, z };
    }
    return result;
}


#pragma endregion
