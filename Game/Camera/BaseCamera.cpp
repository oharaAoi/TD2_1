#include "BaseCamera.h"

BaseCamera::~BaseCamera() {
}

void BaseCamera::Finalize() {
}

void BaseCamera::Init() {
}

void BaseCamera::Update() {
	// 行列の生成
	scaleMat_ = MakeScaleMatrix(transform_.scale);
	rotateMat_ = MakeRotateXYZMatrix(transform_.rotate);
	translateMat_ = MakeTranslateMatrix(transform_.translate);

	// worldの生成
	cameraMatrix_ = Multiply(Multiply(scaleMat_, rotateMat_), translateMat_);
	viewMatrix_ = Inverse(cameraMatrix_);
	projectionMatrix_ = MakePerspectiveFovMatrix(0.45f, float(kWindowWidth_) / float(kWindowHeight_), 0.1f, 100.0f);

	// sprite描画のためのMatrixの初期化
	projectionMatrix2D_ = MakeOrthograhicMatrix(0.0f, 0.0f, float(kWindowWidth_), float(kWindowHeight_), 0.0f, 100.0f);
	viewMatrix2D_ = MakeIdentity4x4();
}
