#pragma once
#include <memory>
#include "Engine/Engine.h"
#include "Engine/2d/Sprite.h"
#include "Game/GameObject/Player.h"
#include "Engine/Json/IJsonConverter.h"

class PlayerControlUI {
public:	// メンバ構造体

	struct Parameter : public IJsonConverter {
		Vector2 spaceOffset;

		json ToJson(const std::string& id) const override {
			return JsonBuilder(id)
				.Add("spaceOffset", spaceOffset)
				.Build();
		}

		void FromJson(const json& jsonData) override {
			fromJson(jsonData, "spaceOffset", spaceOffset);
		}
	};

public:	// メンバ関数

	PlayerControlUI();
	~PlayerControlUI();

	void Init();
	void Update(const Vector2& playerScreenPos);
	void Draw(bool isPlayerFlying) const;

#ifdef _DEBUG
	void Debug_Gui();
#endif // _DEBUG

	void SetPlayer(Player* pPlayer) { pPlayer_ = pPlayer; }

private: // メンバ変数

	// ------------------- 保存用 ------------------- //
	Parameter parameter_;
	const std::string groupName_ = "UI";
	const std::string ItemName_ = "playerControl";

	// ------------------- メンバ変数 ------------------- //

	std::unique_ptr<Sprite> spaceButton_;
	
	Player* pPlayer_;
};

