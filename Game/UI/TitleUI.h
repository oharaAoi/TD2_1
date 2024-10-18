#pragma once
#include <vector>
#include "Engine.h"

class TitleUI {
public:

	TitleUI();
	~TitleUI();

	void Init();
	void Update();
	void Draw() const;

#ifdef _DEBUG
	void Debug_Gui();
#endif // _DEBUG

private:
	std::unique_ptr<Sprite> UI_title_;
	std::unique_ptr<Sprite> UI_startKeyUI_;
};

