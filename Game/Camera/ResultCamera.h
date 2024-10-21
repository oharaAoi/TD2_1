#pragma once
#include "Game/Camera/BaseCamera.h"

class ResultCamera : public BaseCamera{
public:

	ResultCamera();
	~ResultCamera() override;

	void Finalize() override;
	void Init() override;
	void Update() override;

#ifdef _DEBUG
	void Debug_Gui() override;
#endif

public:


private:

	Vector3 offsetVec_;
	float offsetLength_;
};