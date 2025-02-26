#pragma once
#include "Engine/Math/MyMatrix.h"
#include "Engine/Math/MyMath.h"
#include "Engine/Lib/Transform.h"
#include "Engine/Assets/WorldTransform.h"
#include "Enviroment.h"
#include "Engine/Lib/GameTimer.h"
#include "Engine/Render.h"

/// <summary>
/// BaseとなるCamera
/// </summary>
class BaseCamera {
public:

	BaseCamera() = default;
	virtual ~BaseCamera();

	virtual void Finalize();
	virtual void Init();
	virtual void Update();

#ifdef _DEBUG
	virtual void Debug_Gui() {};
#endif // _DEBUG

	Matrix4x4 GetCameraMatrix() const { return cameraMatrix_; }

	Matrix4x4 GetViewMatrix() const { return viewMatrix_; }
	Matrix4x4 GetProjectionMatrix() const { return projectionMatrix_; }

	Matrix4x4 GetViewMatrix2D() const { return viewMatrix2D_; }
	Matrix4x4 GetProjectionMatrix2D() const { return projectionMatrix2D_; }

	Vector3 GetTranslate() const { return transform_.translate; }
	Vector3 GetRotate()const{ return transform_.rotate; }
	void SetRotate(const Vector3& rotate){ transform_.rotate = rotate; }
	Vector3 GetWorldTranslate() const {
		Matrix4x4 matViewInverse = Inverse(viewMatrix_);
		return { matViewInverse.m[3][0], matViewInverse.m[3][1] ,matViewInverse.m[3][2] };
	}

	void SetTarget(WorldTransform* target) { target_ = target; }

	const float GetNearClip() const { return nearClip_; }
	const float GetFarClip() const { return farClip_; }

	const float GetNearClip2D() const { return nearClip2D_; }
	const float GetFarClip2D() const { return farClip2D_; }

protected:

	WorldTransform* target_;

	kTransform transform_;

	Matrix4x4 translateMat_;
	Matrix4x4 scaleMat_;
	Matrix4x4 rotateMat_;

	Matrix4x4 cameraMatrix_;
	Matrix4x4 projectionMatrix_;
	Matrix4x4 viewMatrix_;
	// 2d
	Matrix4x4 projectionMatrix2D_;
	Matrix4x4 viewMatrix2D_;

	float nearClip_;
	float farClip_;

	float nearClip2D_;
	float farClip2D_;

};

