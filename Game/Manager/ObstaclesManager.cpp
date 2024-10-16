#include "ObstaclesManager.h"

ObstaclesManager::ObstaclesManager() {}
ObstaclesManager::~ObstaclesManager() {
	Finalize();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void ObstaclesManager::Finalize() {
	obstaclesList_.clear();
}

void ObstaclesManager::Init() {
	std::filesystem::path dire(kDirectoryPath_);
	if (!std::filesystem::exists(kDirectoryPath_)) {
		std::filesystem::create_directories(kDirectoryPath_);
	}

	LoadAllFile();

	RandomAddObject();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void ObstaclesManager::Update() {
	for (std::list<std::unique_ptr<BasePlacementObject>>::iterator it = obstaclesList_.begin(); it != obstaclesList_.end();) {
		if (!(*it)->GetIsActive()) {
			it = obstaclesList_.erase(it);
			continue;
		}
		float length = std::abs((playerPos_ - (*it)->GetWorldTranslation()).Length());
		if (length < playerDrawLenght_) {
			(*it)->Update();
		}
		++it;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　描画処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void ObstaclesManager::Draw() const {
	for (std::list<std::unique_ptr<BasePlacementObject>>::const_iterator it = obstaclesList_.begin(); it != obstaclesList_.end();) {
		float length = std::abs((playerPos_ - (*it)->GetWorldTranslation()).Length());
		if (length < playerDrawLenght_) {
			(*it)->Draw();
		}
		++it;
	}
}


//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　引数の配列の中にあるファイルを取得
//////////////////////////////////////////////////////////////////////////////////////////////////
void ObstaclesManager::SetObstacles(const std::vector<std::string>& stageInformation) {
	for (size_t oi = 0; oi < stageInformation.size(); ++oi) {
		Inport(stageInformation[oi]);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　障害物を実際にリストに追加する
//////////////////////////////////////////////////////////////////////////////////////////////////
void ObstaclesManager::Inport(const std::string& fileName) {
	for (size_t oi = 0; oi < groupMap_[fileName].loadData_.size(); ++oi) {
		auto& objData = groupMap_[fileName].loadData_;
		auto& obj = obstaclesList_.emplace_back(std::make_unique<BasePlacementObject>());
		obj->Init();
		Quaternion rotate = { objData[oi].rotate_.x,objData[oi].rotate_.y,objData[oi].rotate_.z,objData[oi].rotate_.w };
		Vector3 createPos = objData[oi].pos_;
		switch (objData[oi].type_) {
		case PlacementObjType::ROCK:
			obj.reset(new Rock);
			obj->Init();
			obj->ApplyLoadData(objData[oi].scale_, rotate, createPos, objData[oi].radius_);
			break;
		case PlacementObjType::FISH:
			obj.reset(new Fish);
			obj->Init();
			obj->ApplyLoadData(objData[oi].scale_, rotate, createPos, objData[oi].radius_);
			break;
		case PlacementObjType::BIRD:
			obj.reset(new Bird);
			obj->Init();
			obj->ApplyLoadData(objData[oi].scale_, rotate, createPos, objData[oi].radius_);
			break;
		case PlacementObjType::ITEM:
			obj.reset(new Item);
			obj->Init();
			obj->ApplyLoadData(objData[oi].scale_, rotate, createPos, objData[oi].radius_);
			break;
		case PlacementObjType::DRIFTWOOD:
			obj.reset(new Driftwood);
			obj->Init();
			obj->ApplyLoadData(objData[oi].scale_, rotate, createPos, objData[oi].radius_);
			break;
		case PlacementObjType::WATERWEED:
			obj.reset(new Waterweed);
			obj->Init();
			obj->ApplyLoadData(objData[oi].scale_, rotate, createPos, objData[oi].radius_);
			break;
		case PlacementObjType::COIN:
			obj.reset(new Coin);
			obj->Init();
			obj->ApplyLoadData(objData[oi].scale_, rotate, createPos, objData[oi].radius_);
			coinNum_++;
			break;
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　すべてのファイルの読み込みを行う
//////////////////////////////////////////////////////////////////////////////////////////////////

void ObstaclesManager::LoadAllFile() {
	groupMap_.clear();
	fileNames_.clear();
	for (const auto& entry : std::filesystem::directory_iterator(kDirectoryPath_)) {
		std::string fileName = entry.path().stem().string();
		fileNames_.push_back(fileName);
		MergeMaps(LoadFile(fileName));
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　ファイルの読み込みを行う
//////////////////////////////////////////////////////////////////////////////////////////////////

std::map<std::string, ObstaclesManager::Group> ObstaclesManager::LoadFile(const std::string& fileName) {
	std::map<std::string, ObstaclesManager::Group> map;
	// 読み込むjsonファイルのフルパスを合成する
	std::string filePath = kDirectoryPath_ + fileName + ".json";
	// 読み込み用ファイルストリーム
	std::ifstream ifs;
	// ファイルを読み込みように開く
	ifs.open(filePath);

	if (ifs.fail()) {
		std::string message = "not Exist " + fileName + ".json";
	}

	json root;
	// json文字列からjsonのデータ構造に展開
	ifs >> root;
	// ファイルを閉じる
	ifs.close();

	for (auto& [topKey, nestedData] : root.items()) {
		for (auto& [key, value] : nestedData.items()) {
			PlacementObjType objType = value["objType"];
			Vector3 position = { value["position"][0], value["position"][1], value["position"][2] };
			float radius = value["radius"];
			Vector4 rotate = { value["rotate"][0], value["rotate"][1], value["rotate"][2], value["rotate"][3] };
			Vector3 scale = { value["scale"][0], value["scale"][1], value["scale"][2] };

			map[topKey].loadData_.emplace_back(scale, rotate, position, radius, objType);
		}
	}

	return map;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　読み込んだデータが入ったMapを結合させる
//////////////////////////////////////////////////////////////////////////////////////////////////

void ObstaclesManager::MergeMaps(const std::map<std::string, Group>& map) {
	for (const auto& pair : map) {
		// map1にキーが存在しない場合は新しく挿入される
		groupMap_[pair.first].loadData_.insert(
			groupMap_[pair.first].loadData_.end(),
			pair.second.loadData_.begin(),
			pair.second.loadData_.end()
		);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　debugようにランダムに配置する関数
//////////////////////////////////////////////////////////////////////////////////////////////////

void ObstaclesManager::RandomAddObject() {
	for (int32_t i = 0; i < int(StageInformation::stageWidth_ / 5.0f); i++) {

		// 魚をランダムに配置
		int rand = RandomInt(1, 100);

		// 10mごとに2/3の確率で配置
		if (rand <= 66) {


			// 1/2で魚、アイテムを切り替える
			if (rand % 2 == 0) {
				auto& fish = obstaclesList_.emplace_back(std::make_unique<Fish>());
				float depth = RandomFloat(StageInformation::groundDepth_ + fish->GetRadius(), -fish->GetRadius());
				fish->GetTransform()->SetTranslaion(Vector3(10.0f * i, depth, 0.0f));
				fish->Update();

			} else {
				auto& item = obstaclesList_.emplace_back(std::make_unique<Item>());
				float depth = RandomFloat(StageInformation::groundDepth_ + item->GetRadius(), -item->GetRadius());
				item->GetTransform()->SetTranslaion(Vector3(10.0f * i, depth, 0.0f));
				item->Update();

			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　Debug表示
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
void ObstaclesManager::Debug_Gui() {
	ImGui::Begin("ObstaclesManager");
	ImGui::Text("objectNum: %d", static_cast<int>(obstaclesList_.size()));
	if (ImGui::Button("clear")) {
		obstaclesList_.clear();
	}
	if (ImGui::Button("Add")) {
		Inport(inportFileName_);
	}
	ImGui::SameLine();
	if (ImGui::Button("Inport")) {
		obstaclesList_.clear();
		groupMap_.clear();
		LoadAllFile();
		Inport(inportFileName_);
	}
	ImGui::SameLine();
	if (!fileNames_.empty()) {
		inportFileName_ = fileNames_[inportIndex_];
	}
	if (ImGui::BeginCombo("##InportFileName", &inportFileName_[0], ImGuiComboFlags_HeightLargest)) {
		for (int i = 0; i < fileNames_.size(); i++) {
			const bool isSelected = (inportIndex_ == i);
			if (ImGui::Selectable(fileNames_[i].c_str(), isSelected)) {
				inportIndex_ = i;
			}
			if (isSelected) {
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}
	ImGui::End();
}
#endif // _DEBUG