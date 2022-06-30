#pragma once

#include <numbers>
#include <cmath>
#include "Offsets.h"

#define PI 3.141592653589793238462643

struct Matrix {
	float matrix[16];
};

struct FMatrix {
	float matrix[4][4];
};

struct Vector4 {
    float  x, y, z, w;
};

struct Vector3 {
	float x, y, z;

	float distance(Vector3 vec)
	{
		return sqrt(
			pow(vec.x - x, 2) +
			pow(vec.y - y, 2)
		);
	}
	
	static float Distance(Vector3 a, Vector3 b) {
		Vector3 vector = Vector3(a.x - b.x, a.y - b.y, a.z - b.z);
		return sqrt(((vector.x * vector.x) + (vector.y * vector.y)) + (vector.z * vector.z));
	}

	Vector3 operator-(Vector3 ape)
	{
		return { x - ape.x, y - ape.y, z - ape.z };
	}

	Vector3 operator+(Vector3 ape)
	{
		return { x + ape.x, y + ape.y, z + ape.z };
	}

	Vector3 operator*(float ape)
	{
		return { x * ape, y * ape, z * ape };
	}

	Vector3 operator/(float ape)
	{
		return { x / ape, y / ape, z / ape };
	}

	Vector3 operator/=(float ape)
	{
		x /= ape;
		y /= ape;
		z /= ape;

		return *this;
	}

	Vector3 operator+=(Vector3 ape)
	{
		return { x += ape.x, y += ape.y, z += ape.z };
	}

	Vector3 operator-=(Vector3 ape)
	{
		return { x -= ape.x, y -= ape.y, z -= ape.z };
	}

	void Normalize()
	{
		while (x > 89.0f)
			x -= 180.f;

		while (x < -89.0f)
			x += 180.f;

		while (y > 180.f)
			y -= 360.f;

		while (y < -180.f)
			y += 360.f;
	}

	float Length()
	{
		return sqrt((x * x) + (y * y) + (z * z));
	}

	float Length2D()
	{
		return sqrt((x * x) + (y * y));
	}

	float DistTo(Vector3 ape)
	{
		return (*this - ape).Length();
	}

	float Dist2D(Vector3 ape)
	{
		return (*this - ape).Length2D();
	}

	struct Vector3 ScreenPosition(Matrix matrix) {
		struct Vector3 out;
		float _x = matrix.matrix[0] * x + matrix.matrix[1] * y + matrix.matrix[2] * z + matrix.matrix[3];
		float _y = matrix.matrix[4] * x + matrix.matrix[5] * y + matrix.matrix[6] * z + matrix.matrix[7];
		out.z = matrix.matrix[12] * x + matrix.matrix[13] * y + matrix.matrix[14] * z + matrix.matrix[15];

		_x *= 1.f / out.z;
		_y *= 1.f / out.z;

		int width = GetSystemMetrics(SM_CXSCREEN);
		int height = GetSystemMetrics(SM_CYSCREEN);

		out.x = width * .5f;
		out.y = height * .5f;

		out.x += 0.5f * _x * width + 0.5f;
		out.y -= 0.5f * _y * height + 0.5f;

		return out;
	}

	Vector3 to_angle() {
		Vector3 angles;

		if (y == 0.0f && x == 0.0f)
		{
			angles.x = (z > 0.0f) ? 270.0f : 90.0f;
			angles.y = 0.0f;
		}
		else
		{
			angles.x = std::atan2(-z, Length2D()) * -180 / PI;
			angles.y = std::atan2(y, x) * 180 / PI;

			if (angles.y > 90)
				angles.y -= 180;
			else if (angles.y < 90)
				angles.y += 180;
			else if (angles.y == 90)
				angles.y = 0;
		}

		angles.z = 0.0f;

		return angles;
	}

	Vector3 Clamp() {
		if (x > 180)
			x -= 360;
		else if (x < -180)
			x += 360;

		if (y > 180)
			y -= 360;
		else if (y < -180)
			y += 360;

		if (x < -89)
			x = -89;

		if (x > 89)
			x = 89;

		while (y < -180.0f)
			y += 360.0f;

		while (y > 180.0f)
			y -= 360.0f;

		z = 0;

		return *this;
	}

};

struct Vector2 {
	float x, y;

	Vector2 operator-(Vector2 ape)
	{
		return { x - ape.x, y - ape.y };
	}

	Vector2 operator+(Vector2 ape)
	{
		return { x + ape.x, y + ape.y };
	}

	Vector2 operator*(float ape)
	{
		return { x * ape, y * ape };
	}

	Vector2 operator/(float ape)
	{
		return { x / ape, y / ape };
	}

	Vector2 operator/=(float ape)
	{
		x /= ape;
		y /= ape;

		return *this;
	}

	Vector2 operator+=(Vector2 ape)
	{
		return { x += ape.x, y += ape.y };
	}

	Vector2 operator-=(Vector2 ape)
	{
		return { x -= ape.x, y -= ape.y };
	}

	void Normalize()
	{
		if (x > 89.0f)
			x -= 180.f;

		if (x < -89.0f)
			x += 180.f;

		if (y > 180.f)
			y -= 360.f;

		if (y < -180.f)
			y += 360.f;
	}

	float Length2D()
	{
		return sqrt((x * x) + (y * y));
	}

	float Dist2D(Vector2 ape)
	{
		return (*this - ape).Length2D();
	}
};

struct Bone {
	BYTE shit[0xCC];
	float x;
	BYTE shit2[0xC];
	float y;
	BYTE shit3[0xC];
	float z;
};

Vector3 GetBonePos(uintptr_t entity, int id, Vector3 position) {
	Vector3 bone;
	uintptr_t boneArray = Read<uintptr_t>(entity + Offsets::BoneArray);
	UINT32 boneloc = (id * 0x30);
	Bone bo = {};
	bo = Read<Bone>(boneArray + boneloc);

	bone.x = bo.x + position.x;
	bone.y = bo.y + position.y;
	bone.z = bo.z + position.z;
	return bone;
}

struct Vector3 WorldToScreen(struct Vector3 pos, Matrix matrix, int SWidth, int SHeight) {
	struct Vector3 out;

	float _x = matrix.matrix[0] * pos.x + matrix.matrix[1] * pos.y + matrix.matrix[2] * pos.z + matrix.matrix[3];
	float _y = matrix.matrix[4] * pos.x + matrix.matrix[5] * pos.y + matrix.matrix[6] * pos.z + matrix.matrix[7];

	out.z = matrix.matrix[12] * pos.x + matrix.matrix[13] * pos.y + matrix.matrix[14] * pos.z + matrix.matrix[15];

	_x *= 1.f / out.z;
	_y *= 1.f / out.z;

	out.x = SWidth * .5f;
	out.y = SHeight * .5f;

	out.x += 0.5f * _x * SWidth + 0.5f;
	out.y -= 0.5f * _y * SHeight + 0.5f;

	return out;
};