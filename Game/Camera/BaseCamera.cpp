#include "BaseCamera.h"

BaseCamera::~BaseCamera() {
	Finalize();
}

void BaseCamera::Finalize() {
	target_ = nullptr;
}

void BaseCamera::Init() {
	transform_ = {
		{1.0f, 1.0f, 1.0f},
		{0 , 0, 0.0f},
		{0.0f, 0.0f, -10.0f}
	};

	nearClip_ = 1.0f;
	farClip_ = 10000.0f;
	fov_ = 0.450f;
	nearClip2D_ = 0.0f;
	farClip2D_ = 100.0f;

	// 行列の生成
	scaleMat_ = MakeScaleMatrix(transform_.scale);
	rotateMat_ = MakeRotateXYZMatrix(transform_.rotate);
	translateMat_ = MakeTranslateMatrix(transform_.translate);

	// worldの生成
	cameraMatrix_ = Multiply(Multiply(scaleMat_, rotateMat_), translateMat_);
	viewMatrix_ = Inverse(cameraMatrix_);
	projectionMatrix_ = MakePerspectiveFovMatrix(fov_, float(kWindowWidth_) / float(kWindowHeight_), Render::GetNearClip(), Render::GetFarClip());

	// sprite描画のためのMatrixの初期化
	projectionMatrix2D_ = MakeOrthograhicMatrix(0.0f, 0.0f, float(kWindowWidth_), float(kWindowHeight_), Render::GetNearClip2D(), Render::GetFarClip2D());
	viewMatrix2D_ = MakeIdentity4x4();
}

void BaseCamera::Update() {
	// 行列の生成
	scaleMat_ = MakeScaleMatrix(transform_.scale);
	rotateMat_ = MakeRotateXYZMatrix(transform_.rotate);
	translateMat_ = MakeTranslateMatrix(transform_.translate);

	// worldの生成
	cameraMatrix_ =scaleMat_ * rotateMat_ * translateMat_;
	viewMatrix_ = Inverse(cameraMatrix_);
}
