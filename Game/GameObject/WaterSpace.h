#pragma once
#include "Engine/GameObject/BaseGameObject.h"

/// <summary>
/// 水中を表すbox
/// </summary>
class WaterSpace :
    public BaseGameObject {
public:

    WaterSpace();
    ~WaterSpace();

    void Init() override;
    void Update() override;
    void Draw() const override;

#ifdef _DEBUG
    void Debug_Gui();
#endif // _DEBUG

private:



};

