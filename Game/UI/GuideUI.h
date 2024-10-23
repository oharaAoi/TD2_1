#pragma once
#include <memory>
#include <map>
#include "Engine/Engine.h"
#include "Engine/2d/Sprite.h"
#include "Engine/Utilities/AdjustmentItem.h"

class GuideUI {
public:

	GuideUI();
	~GuideUI();

	void Init();
	void Update();
	void Draw() const;

	void AddDrawList(const std::string& addName);

	void ClearDrawList();

	void SetTitle();
	void SetResult();

#ifdef _DEBUG
	void Debug_Gui();
#endif // _DEBUG

	void SetUIPos(const std::string& addName, const Vector2& pos);

private:

	AdjustmentItem* adjustmentItem_;
	std::string groupName_;

	std::unordered_map<std::string, std::unique_ptr<Sprite>> uiMap_;
	std::list<Sprite*> drawSpriteList_;

};

