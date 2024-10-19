#pragma once
#include "Engine/GameObject/BaseGameObject.h"
#include "Engine/Collider/Collider.h"

enum class PlacementObjType {
	ROCK,
	FISH,
	BIRD,
	ITEM,
	DRIFTWOOD,
	WATERWEED,
	COIN,
};

enum class SubAttributeType {
	NONE,
	SMALL,
	MIDIUM,
	LARGE,
};

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
	const virtual Vector3 GetWorldTranslation() const override { return worldPos_; }

#ifdef _DEBUG
	void Debug_Gui();
#endif // _DEBUG

	virtual void ApplyLoadData(const Vector3& scale, const Quaternion& rotate,
							   const Vector3& pos, const float& radius, const SubAttributeType& subType);

	virtual void IndividualFromCommon(const SubAttributeType& subType = SubAttributeType::NONE) {};

	void SetSubType(const SubAttributeType& subType) { subType_ = subType; }

public:

	const float GetRadius() const { return Collider::GetRadius(); }

protected:

	SubAttributeType subType_;

};

