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

	Inport("WaterWeed.json");
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void ObstaclesManager::Update() {
	//for (const auto& pair : groupMap_) {
	//	const std::string& key = pair.first;	// key
	//	const Group& value = pair.second;		// volue

	//	// mapに格納されているデータの位置を
	//}

	// randomに配置されている物をリストに追加していく
	RandomImportCreate();

	// リストの更新を行う
	for (std::list<std::unique_ptr<BasePlacementObject>>::iterator it = obstaclesList_.begin(); it != obstaclesList_.end();) {
		if (!(*it)->GetIsActive()) {
			it = obstaclesList_.erase(it);
			continue;
		}

		// 更新をする
		(*it)->Update();
		float length = (((*it)->GetWorldTranslation().x - playerPos_.x));
		if(length < -50.0f) {
 			(*it)->SetIsActive(false);
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
// ↓　ランダム配置の追加
//////////////////////////////////////////////////////////////////////////////////////////////////

void ObstaclesManager::RandomImportCreate() {
	for (auto it = randomImportArray_.begin(); it != randomImportArray_.end();) {

		float length = std::abs(it->pos_.x - playerPos_.x);

		// 仮初期化ゾーン
		FISH_SIZE fishSize = FISH_SIZE::SMALL;
		Fish* fish = nullptr;
		float randTheta = 0.0f;
		Vector3 direction{};

		if (length < playerDrawLenght_) {
			auto& obj = obstaclesList_.emplace_back(std::make_unique<BasePlacementObject>());
			obj->Init();
			Quaternion rotate = {
				it->rotate_.x,
				it->rotate_.y,
				it->rotate_.z,
				it->rotate_.w
			};

			Vector3 createPos = it->pos_;
			switch (it->type_) {
			case PlacementObjType::ROCK:

				obj.reset(new Rock);
				obj->Init();
				obj->ApplyLoadData(it->scale_, rotate, createPos, it->radius_);
				break;

			case PlacementObjType::FISH:

				obj.reset(new Fish);
				fish = dynamic_cast<Fish*>(obj.get());
				fishSize = FISH_SIZE(RandomInt(0, (int)FISH_SIZE::kFishSizeCount - 1));

				fish->Init();
				fish->ApplyLoadData(it->scale_, rotate, createPos, it->radius_);
				fish->SetFishSize(fishSize);
				break;

			case PlacementObjType::BIRD:

				obj.reset(new Bird);
				obj->Init();
				obj->ApplyLoadData(it->scale_, rotate, createPos, it->radius_);
				obj->SetObbSize(obj->GetRadius());


				randTheta = RandomFloat(0.0f, 3.14f * 2.0f);
				direction = { std::cosf(randTheta),std::sinf(randTheta),0.0f };
				dynamic_cast<Bird*>(obj.get())->SetMoveDirection(direction);
				dynamic_cast<Bird*>(obj.get())->SetMoveRadius(RandomFloat(6.0f, 10.0f));
				dynamic_cast<Bird*>(obj.get())->SetIsMove(RandomInt(0,1));

				break;

			case PlacementObjType::ITEM:

				obj.reset(new Item);
				obj->Init();
				obj->ApplyLoadData(it->scale_, rotate, createPos, it->radius_);
				obj->SetObbSize(obj->GetRadius());
				break;

			case PlacementObjType::DRIFTWOOD:

				obj.reset(new Driftwood);
				obj->Init();
				obj->ApplyLoadData(it->scale_, rotate, createPos, it->radius_);
				break;

			case PlacementObjType::WATERWEED:

				obj.reset(new Waterweed);
				obj->Init();
				obj->ApplyLoadData(it->scale_, rotate, createPos, it->radius_);
				break;

			case PlacementObjType::COIN:

				obj.reset(new Coin);
				obj->Init();
				obj->ApplyLoadData(it->scale_, rotate, createPos, it->radius_);
				coinNum_++;
				break;
			}

			it = randomImportArray_.erase(it);
		} else {
			++it;
		}
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
			obj->SetObbSize(obj->GetRadius());
			break;

		case PlacementObjType::ITEM:

			obj.reset(new Item);
			obj->Init();
			obj->ApplyLoadData(objData[oi].scale_, rotate, createPos, objData[oi].radius_);
			obj->SetObbSize(obj->GetRadius());
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

		//////////////////////////////////////////////////////
		//                   水中
		//////////////////////////////////////////////////////

		// 魚をランダムに配置
		int rand = RandomInt(1, 100);

		// 10mごとに2/3の確率で配置
		if (rand <= 66) {

			// 1/2で魚、アイテムを切り替える
			if (rand % 2 == 0) {
				float depth = RandomFloat(StageInformation::groundDepth_ + 1.0f, -1.0f);
				LoadData data{
					Vector3(1.0f,1.0f,1.0f),			// scale
					Vector4(0.0f,0.0f,0.0f, 1.0f),		// rotate
					Vector3(10.0f * i, depth, 0.0f),	// 位置
					1.0f,								// 半径
					PlacementObjType::FISH				// type
				};

				randomImportArray_.push_back(data);

			} else {
				float depth = RandomFloat(StageInformation::groundDepth_ + 1.0f, -1.0f);
				LoadData data{
					Vector3(1.0f,1.0f,1.0f),			// scale
					Vector4(0.0f,0.0f,0.0f, 1.0f),		// rotate
					Vector3(10.0f * i, depth, 0.0f),	// 位置
					3.0f,								// 半径
					PlacementObjType::ITEM				// type
				};

				randomImportArray_.push_back(data);
			}
		}

		////////////////////////////////////////////
		//              空中
		////////////////////////////////////////////

		if (rand < 20) {

			// 10mに一回、1/5の確率で出現
			float height = RandomFloat(0.0f, 60.0f);
			LoadData data{
				Vector3(1.0f,1.0f,1.0f),			// scale
				Vector4(0.0f,0.0f,0.0f, 1.0f),		// rotate
				Vector3(10.0f * i, height, 0.0f),	// 位置
				3.0f,								// 半径
				PlacementObjType::BIRD				// type
			};

			randomImportArray_.push_back(data);
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