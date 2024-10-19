#pragma once
#include "Engine/Engine.h"
#include "Engine/Render.h"
#include "Engine/GameObject/Model.h"
#include "Engine/Assets/Mesh.h"
#include "Engine/Assets/Material.h"
#include "Engine/Assets/PBRMaterial.h"
#include "Engine/Assets/WorldTransform.h"
#include "Engine/Utilities/DirectXUtils.h"
#include "Engine/Lib/GameTimer.h"
#include "Engine/Utilities/Loader.h"
#include "Engine/Manager/ModelManager.h"

/// <summary>
/// 水中を表すbox
/// </summary>
class WaterSpace {
public:

    struct WaveParameter {
        float amplitude;
        float frequency;
        float time;
    };

public:

    WaterSpace();
    ~WaterSpace();

    void Finalize();
    void Init(const std::string& directorPath, const std::string& fileName);
    void Update();
    void Draw() const;

#ifdef _DEBUG
    void Debug_Gui();
#endif // _DEBUG

public:

    const std::vector<Vector3>& GetWorldTopFaceList() { return worldTopFaceList_; }
    void SetTranslate(const Vector3& translate){ transform_->SetTranslaion(translate); }

    const Vector3 GetWorldPos() const;

private:

    /*ComPtr<ID3D12Resource> waveParameterBuffer_;
    D3D12_VERTEX_BUFFER_VIEW waveParameteBufferView_ = {};
    WaveParameter* waveParameter_ = nullptr;*/

    Model* model_ = nullptr;
    std::vector<std::unique_ptr<Mesh>> meshArray_;
    std::vector<std::unique_ptr<PBRMaterial>> materials_;
    //std::unordered_map<std::string, std::unique_ptr<Material>> materialArray_;
    std::unique_ptr<WorldTransform> transform_;

    // 水中の元のBoxとなる上面の位置配列
    std::vector<Vector3> topFaceList_;
    std::vector<Vector3> worldTopFaceList_;
};

