#pragma once
#include "Engine/GameObject/BaseGameObject.h"
#include "Engine/Utilities/AdjustmentItem.h"
#include "Engine/Input/Input.h"
#include "Engine/Lib/GameTimer.h"


class Player :
    public BaseGameObject {
public:

    Player();
    ~Player();

    void Init() override;
    void Update() override;
    void Draw() const override;

#ifdef _DEBUG
    void Debug_Gui();
#endif // _DEBUG

    void Move();
    void LookAtDirection(const float& angle);

    void AdaptAdjustmentItem();

private:

    AdjustmentItem* adjustmentItem_;

    Vector3 velocity_;
    float moveSpeed_;

    float lookAtT_;
};

