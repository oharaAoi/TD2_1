#pragma once
#include "Engine/GameObject/BaseGameObject.h"
#include "Engine/Collider/Collider.h"

/// <summary>
/// 障害物などのオブジェクトの元となるクラス
/// </summary>
class BasePlacementObject 
 : public BaseGameObject, public Collider {

public:

	BasePlacementObject() = default;
	virtual ~BasePlacementObject() override;

	virtual void Finalize() override;
	virtual void Init() override; 
	virtual void Update() override;
	virtual void Draw() const override; 

	virtual void OnCollision([[maybe_unused]] Collider* other) override {};
	const virtual Vector3 GetWorldTranslation(const Vector3& offset = { 0.0f, 0.0f, 0.0f }) const override { return offset; }

#ifdef _DEBUG
	void Debug_Gui();
#endif // _DEBUG

	void ApplyLoadData(const Vector3& scale, const Quaternion& rotate, const Vector3& pos, const float& radius);

public:

	const float GetRadius() const { return Collider::GetRadius(); }

protected:



};
