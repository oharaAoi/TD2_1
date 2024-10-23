#include "ObstaclesManager.h"
#include "Game/Scene/GameScene.h"

ObstaclesManager::ObstaclesManager(){}
ObstaclesManager::~ObstaclesManager(){
	Finalize();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void ObstaclesManager::Finalize(){
	obstaclesList_.clear();
}

void ObstaclesManager::Init(Player* pPlayer){
	std::filesystem::path dire(kDirectoryPath_);
	if(!std::filesystem::exists(kDirectoryPath_)) {
		std::filesystem::create_directories(kDirectoryPath_);
	}

	pPlayer_ = pPlayer;
	LoadAllFile();

	//RandomAddObject();

	/*playerPos_ = { 0.0f, 0.0f,0.0f };
	prePlayerPos_ = { 0.0f, 0.0f,0.0f };

	playerDrawLenght_ = 250.0f;
	RandomImport();

	playerDrawLenght_ = 450.0f;
	RandomImport();

	playerDrawLenght_ = 450.0f;
	importLevel_ = 1;*/
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void ObstaclesManager::Update(){
	if (pGameScene_->GetGameState() == GAME_STATE::GAME) {
		if (playerPos_.x - prePlayerPos_.x > 200.0f) {
			NotMatchRandomImport();
			prePlayerPos_ = playerPos_;
		}
	}

	animationDrawList_.clear();
	normalDrawList_.clear();
	// リストの更新を行う
	for(std::list<std::unique_ptr<BasePlacementObject>>::iterator it = obstaclesList_.begin(); it != obstaclesList_.end();) {
		if(!(*it)->GetIsActive()) {
			it = obstaclesList_.erase(it);
			continue;
		}

		// -------------------------------------------------
		// ↓ playerのbodyの数によって色を変更
		// -------------------------------------------------
		/*Collider* obj = dynamic_cast<Collider*>((*it).get());*/
		Fish* pFish = dynamic_cast<Fish*>((*it).get());
		if ((*it)->GetObjectType() == (int)ObjectType::FISH) {
			if (pPlayer_->GetBodyCount()>= pFish->GetEatCount()) {//pPlayer_->GetChargePower() / fishSizeDivision >= (float)pFish->GetFishSize()
				pFish->SetIsAte(true);
			} else {
				pFish->SetIsAte(false);
			}
		}

		// -------------------------------------------------
		// ↓ 本更新
		// -------------------------------------------------
		(*it)->Update();

		// -------------------------------------------------
		// ↓ playerとの長さでactiveをoffにする
		// -------------------------------------------------
		float length = (((*it)->GetWorldTranslation().x - playerPos_.x));
		if(length < -60.0f) {
 			(*it)->SetIsActive(false);
		}

		// -------------------------------------------------
		// ↓ 描画のリストにアニメーションがあるかないかで描画リストを変える
		// -------------------------------------------------
		if ((*it)->IsSetAnimetor()) {
			animationDrawList_.push_back((*it).get());
		} else {
			normalDrawList_.push_back((*it).get());
		}

		++it;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　描画処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void ObstaclesManager::Draw() const {
	Engine::SetPipeline(PipelineType::NormalPipeline);
	for (std::list<BasePlacementObject*>::const_iterator it = normalDrawList_.begin(); it != normalDrawList_.end();) {
		float length = std::abs((playerPos_ - (*it)->GetWorldTranslation()).Length());
		if (length < playerDrawLenght_) {
			(*it)->Draw();
		}
		++it;
	}

	Engine::SetPipeline(PipelineType::SkinningPipeline);
	for (std::list<BasePlacementObject*>::const_iterator it = animationDrawList_.begin(); it != animationDrawList_.end();) {
		float length = std::abs((playerPos_ - (*it)->GetWorldTranslation()).Length());
		if(length < playerDrawLenght_) {
			(*it)->Draw();
		}
		++it;
	}
}

void ObstaclesManager::AllFileClear() {
	levelFileName_.clear();
}

void ObstaclesManager::RandomImport(){
	// レベルごとでファイル名が保存がされているため、現在のレベルの配列から文字列を取得する
	if (levelFileName_[importLevel_].size() - 1 <= 0) {
		return;
	}

	int fileNum = RandomInt(0, static_cast<int>(levelFileName_[importLevel_].size()) - 1);
	std::string randomFileName = levelFileName_[importLevel_][fileNum];

	Inport(randomFileName, importLevel_);
	Log("Load : GameData[" + randomFileName + "]\n");
}

void ObstaclesManager::NotMatchRandomImport(){
	// レベルごとでファイル名が保存がされているため、現在のレベルの配列から文字列を取得する
	if(levelFileName_[importLevel_].size() - 1 <= 0) {
		return;
	}

	int fileNum = RandomInt(0, static_cast<int>(levelFileName_[importLevel_].size()) - 1);
	if(fileNum == prePattern){
		for(int i = 0; i < 5; i++)
		{
			if(fileNum != prePattern){ break; }
			fileNum = RandomInt(0, static_cast<int>(levelFileName_[importLevel_].size()) - 1);
		}
	}
	prePattern = fileNum;
	std::string randomFileName = levelFileName_[importLevel_][fileNum];

	Inport(randomFileName, importLevel_);
	Log("Load : GameData[" + randomFileName + "]\n");
}

void ObstaclesManager::TutorialImport(const std::string& fileName, const Vector3& pos, int inportlevel) {
	for (size_t oi = 0; oi < groupMap_[inportlevel][fileName].loadData_.size(); ++oi) {
		auto& objData = groupMap_[inportlevel][fileName].loadData_;
		auto& obj = obstaclesList_.emplace_back(std::make_unique<BasePlacementObject>());
		Quaternion rotate = { objData[oi].rotate_.x,objData[oi].rotate_.y,objData[oi].rotate_.z,objData[oi].rotate_.w };
		Vector3 createPos = objData[oi].pos_;
		createPos.x += pos.x;

		switch (objData[oi].type_) {
		case PlacementObjType::ROCK:
			obj.reset(new Rock);
			obj->Init();
			obj->ApplyLoadData(objData[oi].scale_, rotate, createPos, objData[oi].subType_);
			break;
		case PlacementObjType::FISH:
			obj.reset(new Fish);
			obj->Init();
			obj->ApplyLoadData(objData[oi].scale_, rotate, createPos, objData[oi].subType_);
			break;
		case PlacementObjType::BIRD:
			obj.reset(new Bird);
			obj->Init();
			obj->ApplyLoadData(objData[oi].scale_, rotate, createPos, objData[oi].subType_);
			obj->SetObbSize(obj->GetRadius());
			break;
		case PlacementObjType::DRIFTWOOD:
			obj.reset(new Driftwood);
			obj->Init();
			obj->ApplyLoadData(objData[oi].scale_, rotate, createPos, objData[oi].subType_);
			break;
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　ランダム配置の追加
//////////////////////////////////////////////////////////////////////////////////////////////////

void ObstaclesManager::RandomImportCreate(){
	for(auto it = randomImportArray_.begin(); it != randomImportArray_.end();) {

		float length = std::abs(it->pos_.x - playerPos_.x);

		// 仮初期化ゾーン
		FISH_SIZE fishSize = FISH_SIZE::SMALL;
		Fish* fish = nullptr;
		float randTheta = 0.0f;
		Vector3 direction{};

		if(length < playerDrawLenght_) {
			auto& obj = obstaclesList_.emplace_back(std::make_unique<BasePlacementObject>());
			Quaternion rotate = {
				it->rotate_.x,
				it->rotate_.y,
				it->rotate_.z,
				it->rotate_.w
			};

			Vector3 createPos = it->pos_;
			switch(it->type_) {
			case PlacementObjType::ROCK:

				obj.reset(new Rock);
				obj->Init();
				obj->ApplyLoadData(it->scale_, rotate, createPos, it->subType_);
				break;

			case PlacementObjType::FISH:

				obj.reset(new Fish);
				fish = dynamic_cast<Fish*>(obj.get());
				fishSize = FISH_SIZE(RandomInt(0, (int)FISH_SIZE::kFishSizeCount - 1));
				
				fish->Init();
				fish->ApplyLoadData(it->scale_, rotate, createPos, it->subType_);
				fish->IndividualFromCommon(it->subType_);
				fish->SetFishSize(fishSize);
				break;

			case PlacementObjType::BIRD:

				obj.reset(new Bird);
				obj->Init();
				obj->ApplyLoadData(it->scale_, rotate, createPos, it->subType_);
				obj->SetObbSize(obj->GetRadius());


				randTheta = RandomFloat(0.0f, 3.14f * 2.0f);
				direction = { std::cosf(randTheta),std::sinf(randTheta),0.0f };
				dynamic_cast<Bird*>(obj.get())->SetMoveDirection(direction);
				dynamic_cast<Bird*>(obj.get())->SetMoveRadius(RandomFloat(6.0f, 10.0f));
				dynamic_cast<Bird*>(obj.get())->SetIsMove(RandomInt(0, 1));

				break;

			case PlacementObjType::ITEM:

				obj.reset(new Item);
				obj->Init();
				obj->ApplyLoadData(it->scale_, rotate, createPos, it->subType_);
				obj->SetObbSize(obj->GetRadius());
				break;

			case PlacementObjType::DRIFTWOOD:

				obj.reset(new Driftwood);
				obj->Init();
				obj->ApplyLoadData(it->scale_, rotate, createPos, it->subType_);
				break;

			case PlacementObjType::WATERWEED:

				obj.reset(new Waterweed);
				obj->Init();
				obj->ApplyLoadData(it->scale_, rotate, createPos, it->subType_);
				break;

			case PlacementObjType::COIN:

				obj.reset(new Coin);
				obj->Init();
				obj->ApplyLoadData(it->scale_, rotate, createPos, it->subType_);
				coinNum_++;
				break;
			}

			obj->SetIsLighting(false);

			it = randomImportArray_.erase(it);
		} else {
			++it;
		}
	}

}


//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　引数の配列の中にあるファイルを取得
//////////////////////////////////////////////////////////////////////////////////////////////////
void ObstaclesManager::SetObstacles(const std::vector<std::string>& stageInformation){
	for(size_t oi = 0; oi < stageInformation.size(); ++oi) {
		Inport(stageInformation[oi], importLevel_);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　障害物を実際にリストに追加する
//////////////////////////////////////////////////////////////////////////////////////////////////
void ObstaclesManager::Inport(const std::string& fileName, uint32_t level){
	for(size_t oi = 0; oi < groupMap_[level][fileName].loadData_.size(); ++oi) {
		auto& objData = groupMap_[level][fileName].loadData_;
		auto& obj = obstaclesList_.emplace_back(std::make_unique<BasePlacementObject>());
		Quaternion rotate = { objData[oi].rotate_.x,objData[oi].rotate_.y,objData[oi].rotate_.z,objData[oi].rotate_.w };
		Vector3 createPos = objData[oi].pos_;

		createPos.x += playerPos_.x + playerDrawLenght_;

		switch(objData[oi].type_) {
		case PlacementObjType::ROCK:
			obj.reset(new Rock);
			obj->Init();
			obj->ApplyLoadData(objData[oi].scale_, rotate, createPos, objData[oi].subType_);
			break;
		case PlacementObjType::FISH:
			obj.reset(new Fish);
			obj->Init();
			obj->ApplyLoadData(objData[oi].scale_, rotate, createPos, objData[oi].subType_);
			break;
		case PlacementObjType::BIRD:
			obj.reset(new Bird);
			obj->Init();
			// 60.0f以上の高さにいたら少し上げる
			if(pPlayer_->GetTransform()->GetTranslation().y >= 30.0f){
				createPos.y += std::abs(pPlayer_->GetTransform()->GetTranslation().y - createPos.y) * birdPopYRaito_;
			}
			createPos.y = std::clamp(createPos.y, 11.0f,999.0f);
			obj->ApplyLoadData(objData[oi].scale_, rotate, createPos, objData[oi].subType_);
			obj->SetObbSize(obj->GetRadius());
			break;
		case PlacementObjType::DRIFTWOOD:
			obj.reset(new Driftwood);
			obj->Init();
			obj->ApplyLoadData(objData[oi].scale_, rotate, createPos, objData[oi].subType_);
			break;
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　すべてのファイルの読み込みを行う
//////////////////////////////////////////////////////////////////////////////////////////////////

void ObstaclesManager::LoadAllFile(){
	groupMap_.clear();
	fileNames_.clear();
	levelFileName_.clear();
	for(const auto& entry : std::filesystem::directory_iterator(kDirectoryPath_)) {
		std::string fileName = entry.path().stem().string();
		fileNames_.push_back(fileName);
		MergeMaps(LoadFile(fileName));
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　ファイルの読み込みを行う
//////////////////////////////////////////////////////////////////////////////////////////////////

std::map<std::string, ObstaclesManager::Group> ObstaclesManager::LoadFile(const std::string& fileName){
	std::map<std::string, ObstaclesManager::Group> map;
	// 読み込むjsonファイルのフルパスを合成する
	std::string filePath = kDirectoryPath_ + fileName + ".json";
	// 読み込み用ファイルストリーム
	std::ifstream ifs;
	// ファイルを読み込みように開く
	ifs.open(filePath);

	if(ifs.fail()) {
		std::string message = "not Exist " + fileName + ".json";
	}

	json root;
	// json文字列からjsonのデータ構造に展開
	ifs >> root;
	// ファイルを閉じる
	ifs.close();

	for(auto& [topKey, nestedData] : root.items()) {
		for(auto& [key, value] : nestedData.items()) {
			if(key == "level") {
				map[topKey].level = value;
			} else {
				PlacementObjType objType = value["objType"];
				SubAttributeType subType = value["subType"];
				Vector3 position = { value["position"][0], value["position"][1], value["position"][2] };
				float radius = value["radius"];
				Vector4 rotate = { value["rotate"][0], value["rotate"][1], value["rotate"][2], value["rotate"][3] };
				Vector3 scale = { value["scale"][0], value["scale"][1], value["scale"][2] };

				map[topKey].loadData_.emplace_back(scale, rotate, position, radius, objType, subType);
			}
		}
	}

	auto& fileList = levelFileName_[map[fileName].level];
	if(std::find(fileList.begin(), fileList.end(), fileName) == fileList.end()) {
		fileList.push_back(fileName);
	}

	return map;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　読み込んだデータが入ったMapを結合させる
//////////////////////////////////////////////////////////////////////////////////////////////////

void ObstaclesManager::MergeMaps(const std::map<std::string, Group>& map){
	for(const auto& pair : map) {
		// map1にキーが存在しない場合は新しく挿入される
		groupMap_[pair.second.level][pair.first].loadData_.insert(
			groupMap_[pair.second.level][pair.first].loadData_.end(),
			pair.second.loadData_.begin(),
			pair.second.loadData_.end()
		);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　debugようにランダムに配置する関数
//////////////////////////////////////////////////////////////////////////////////////////////////

void ObstaclesManager::RandomAddObject(){

	for(int32_t i = 0; i < int(StageInformation::stageWidth_ / 5.0f); i++) {

		//////////////////////////////////////////////////////
		//                   水中
		//////////////////////////////////////////////////////

		// 魚をランダムに配置
		int rand = RandomInt(1, 100);

		// 10mごとに1/3の確率で配置
		if(rand <= 33) {

			// 1/2で魚、アイテムを切り替える
			//if (rand % 2 == 0) {

			float depth = RandomFloat(StageInformation::groundDepth_ + 1.0f, -1.0f);
			LoadData data{
				Vector3(1.0f,1.0f,1.0f),			// scale
				Vector4(0.0f,0.0f,0.0f, 1.0f),		// rotate
				Vector3(10.0f * i, depth, 0.0f),	// 位置
				1.0f,								// 半径
				PlacementObjType::FISH,				// type
				SubAttributeType::NONE
			};

			randomImportArray_.push_back(data);

			//} else {
			//	float depth = RandomFloat(StageInformation::groundDepth_ + 1.0f, -1.0f);
			//	LoadData data{
			//		Vector3(1.0f,1.0f,1.0f),			// scale
			//		Vector4(0.0f,0.0f,0.0f, 1.0f),		// rotate
			//		Vector3(10.0f * i, depth, 0.0f),	// 位置
			//		3.0f,								// 半径
			//		PlacementObjType::ITEM				// type
			//	};
			//
			//	randomImportArray_.push_back(data);
			//}
		}

		////////////////////////////////////////////
		//              空中
		////////////////////////////////////////////

		if(rand < 20) {

			// 10mに一回、1/5の確率で出現
			float height = RandomFloat(0.0f, 60.0f);
			LoadData data{
				Vector3(3.0f,3.0f,3.0f),			// scale
				Vector4(0.0f,0.0f,0.0f, 1.0f),		// rotate
				Vector3(10.0f * i, height, 0.0f),	// 位置
				6.0f,								// 半径
				PlacementObjType::BIRD,				// type
				SubAttributeType::NONE
			};

			randomImportArray_.push_back(data);
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　Debug表示
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
void ObstaclesManager::Debug_Gui(){
	ImGui::Begin("ObstaclesManager");
	ImGui::Text("objectNum: %d", static_cast<int>(obstaclesList_.size()));
	ImGui::DragFloat("drawLenght : %f", &playerDrawLenght_);

	ImGui::DragFloat("birdPopYRaito", &birdPopYRaito_, 0.1f);

	if(ImGui::Button("Reload")) {
		LoadAllFile();
	}

	if(ImGui::Button("Clear")) {
		obstaclesList_.clear();
	}
	if(ImGui::Button("Add")) {
		Inport(inportFileName_, debug_importLevel_);
	}
	ImGui::SameLine();
	if(ImGui::Button("Inport")) {
		obstaclesList_.clear();
		groupMap_.clear();
		LoadAllFile();
		Inport(inportFileName_, debug_importLevel_);
	}

	if(!fileNames_.empty()) {
		if (debug_importLevel_ < fileNames_.size()) {
			inportFileName_ = fileNames_[debug_importLevel_];
		}
	}

	//if (ImGui::TreeNode("All")) {
	//	if (ImGui::BeginCombo("##InportFileName", &inportFileName_[0], ImGuiComboFlags_HeightLargest)) {
	//		for (int i = 0; i < fileNames_.size(); i++) {
	//			const bool isSelected = (inportIndex_ == i);
	//			if (ImGui::Selectable(fileNames_[i].c_str(), isSelected)) {
	//				inportIndex_ = i;
	//			}
	//			if (isSelected) {
	//				ImGui::SetItemDefaultFocus();
	//			}
	//		}
	//		ImGui::EndCombo();
	//	}

	//	ImGui::TreePop();
	//} 

	if(ImGui::TreeNode("Level")) {
		if(ImGui::Button("-")) { debug_importLevel_--; }
		ImGui::SameLine();
		if(ImGui::Button("+")) { debug_importLevel_++; }
		ImGui::SameLine();
		ImGui::Text(" level:%d", debug_importLevel_);
		debug_importLevel_ = std::clamp(static_cast<int>(debug_importLevel_), 0, 10);
		if(ImGui::BeginCombo("##InportFileName", &inportFileName_[0], ImGuiComboFlags_HeightLargest)) {
			for(int i = 0; i < levelFileName_[debug_importLevel_].size(); i++) {
				const bool isSelected = (inportIndex_ == i);
				if(ImGui::Selectable(levelFileName_[debug_importLevel_][i].c_str(), isSelected)) {
					inportIndex_ = i;
				}
				if(isSelected) {
					ImGui::SetItemDefaultFocus();
				}
			}
			ImGui::EndCombo();
		}
		ImGui::TreePop();
	}
	ImGui::End();
}
#endif // _DEBUG