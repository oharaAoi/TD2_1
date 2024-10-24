#include "PlacementObjectEditer.h"

PlacementObjectEditer::PlacementObjectEditer() {}
PlacementObjectEditer::~PlacementObjectEditer() {
#ifdef _DEBUG
	debug_BasePlacementObj_.clear();
	inport_BasePlacementObj_.clear();
#endif
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void PlacementObjectEditer::Init(ObstaclesManager* obstaclesManager) {
	obstaclesManager_ = obstaclesManager;
	std::filesystem::path dire(obstaclesManager_->GetDirectoryPath());
	if (!std::filesystem::exists(obstaclesManager_->GetDirectoryPath())) {
		std::filesystem::create_directories(obstaclesManager_->GetDirectoryPath());
	}

	LoadAllFile();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　ファイルの読み込みを行う
//////////////////////////////////////////////////////////////////////////////////////////////////

void PlacementObjectEditer::LoadAllFile() {
	fileNames_.clear();
	groupMap_.clear();


	for (const auto& entry : std::filesystem::directory_iterator(obstaclesManager_->GetDirectoryPath())) {
		std::string fileName = entry.path().stem().string();
		fileNames_.push_back(fileName);
		MergeMaps(obstaclesManager_->LoadFile(fileName));
	}
}

void PlacementObjectEditer::MergeMaps(const std::map<std::string, ObstaclesManager::Group>& map) {
	for (const auto& pair : map) {
		// map1にキーが存在しない場合は新しく挿入される
		groupMap_[pair.first].loadData_.insert(
			groupMap_[pair.first].loadData_.end(),
			pair.second.loadData_.begin(),
			pair.second.loadData_.end()
		);

		groupMap_[pair.first].level = pair.second.level;
	}
}

#ifdef _DEBUG
#include "Engine/Manager/ImGuiManager.h"
//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void PlacementObjectEditer::Update() {
	Debug_Gui();

	animationDrawList_.clear();
	normalDrawList_.clear();
	// -------------------------------------------------
	// ↓ リスト内にある更新処理を行う
	// -------------------------------------------------
	for (std::list<ObstaclesManager::ObjectData>::iterator it = debug_BasePlacementObj_.begin(); it != debug_BasePlacementObj_.end();) {
		(*it).object_->Update();
		// 描画のリストにアニメーションがあるかないかで設定をする
		if ((*it).object_->IsSetAnimetor()) {
			animationDrawList_.push_back((*it).object_.get());
		} else {
			normalDrawList_.push_back((*it).object_.get());
		}
		++it;
	}

	for (std::list<ObstaclesManager::ObjectData>::iterator it = inport_BasePlacementObj_.begin(); it != inport_BasePlacementObj_.end();) {
		(*it).object_->Update();
		// 描画のリストにアニメーションがあるかないかで設定をする
		if ((*it).object_->IsSetAnimetor()) {
			animationDrawList_.push_back((*it).object_.get());
		} else {
			normalDrawList_.push_back((*it).object_.get());
		}
		++it;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　描画処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void PlacementObjectEditer::Draw() const {
	if (!isDraw_) {
		return;
	}
	// -------------------------------------------------
	// ↓ リスト内にある描画処理を行う
	// -------------------------------------------------
	/*for (std::list<ObstaclesManager::ObjectData>::const_iterator it = debug_BasePlacementObj_.begin(); it != debug_BasePlacementObj_.end();) {
		(*it).object_->Draw();
		++it;
	}

	for (std::list<ObstaclesManager::ObjectData>::const_iterator it = inport_BasePlacementObj_.begin(); it != inport_BasePlacementObj_.end();) {
		(*it).object_->Draw();
		++it;
	}*/

	Engine::SetPipeline(PipelineType::NormalPipeline);
	for (std::list<BasePlacementObject*>::const_iterator it = normalDrawList_.begin(); it != normalDrawList_.end();) {
		if ((*it) != nullptr) {
			(*it)->Draw();
		}
		++it;
	}

	Engine::SetPipeline(PipelineType::SkinningPipeline);
	for (std::list<BasePlacementObject*>::const_iterator it = animationDrawList_.begin(); it != animationDrawList_.end();) {
		if ((*it) != nullptr) {
			(*it)->Draw();
			++it;
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　Guiの表示を行う
//////////////////////////////////////////////////////////////////////////////////////////////////

void PlacementObjectEditer::Debug_Gui() {
	ImGui::Begin("PlacementObjEditer");
	ImGui::Checkbox("CheckCollision", &isCheckCollision_);
	ImGui::Checkbox("isDraw", &isDraw_);
	// Reloadを行う
	if (ImGui::Button("Reload")) {
		groupMap_.clear();
		fileNames_.clear();
		LoadAllFile();
	}
	// 新しいファイルの作成
	if (ImGui::TreeNode("newFile")) {
		NewGroup_Config();
		ImGui::TreePop();
	}
	ImGui::Separator();
	// 既存のファイルを編集
	if (ImGui::TreeNode("editFile")) {
		Edit_Config();
		ImGui::TreePop();
	}
	ImGui::End();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　新しいGroupのGuiを開く
//////////////////////////////////////////////////////////////////////////////////////////////////

void PlacementObjectEditer::NewGroup_Config() {

	// -------------------------------------------------
	// ↓ 保存を行う
	// -------------------------------------------------
	if (ImGui::Button("Save")) {
		Save(exportFileName_, debug_BasePlacementObj_, newLevel_);
	}
	ImGui::SameLine();
	ImGui::InputText(".json##SaveFineName", &exportFileName_[0], sizeof(char) * 64);

	// -------------------------------------------------
	// ↓ 生成するファイルのレベルを決める
	// -------------------------------------------------
	ImGui::Text("newLevel");
	if (ImGui::Button("-")) { newLevel_--; }
	ImGui::SameLine();
	if (ImGui::Button("+")) { newLevel_++; }
	ImGui::SameLine();
	ImGui::Text(": %d", newLevel_);
	newLevel_ = std::clamp(static_cast<int>(newLevel_), 0, 10);


	// -------------------------------------------------
	// ↓ 生成する種類を選択する
	// -------------------------------------------------
	ImGui::Text("newObjType");
	NewObjectTypeSelect();

	// -------------------------------------------------
	// ↓ 実際に生成を行う
	// -------------------------------------------------

	if (ImGui::Button("AddList")) {
		auto& newObject = debug_BasePlacementObj_.emplace_back(std::make_unique<BasePlacementObject>());
		switch (newPopType_) {
		case PlacementObjType::ROCK:
			newObject.object_.reset(new Rock);
			newObject.object_->Init();
			newObject.type_ = PlacementObjType::ROCK;
			newObject.subType_ = SubAttributeType::SMALL;
			break;
		case PlacementObjType::FISH:
			newObject.object_.reset(new Fish);
			newObject.object_->Init();
			newObject.type_ = PlacementObjType::FISH;
			newObject.subType_ = SubAttributeType::SMALL;
			break;
		case PlacementObjType::BIRD:
			newObject.object_.reset(new Bird);
			newObject.object_->Init();
			newObject.type_ = PlacementObjType::BIRD;
			newObject.subType_ = SubAttributeType::NONE;
			break;
		case PlacementObjType::DRIFTWOOD:
			newObject.object_.reset(new Driftwood);
			newObject.object_->Init();
			newObject.type_ = PlacementObjType::DRIFTWOOD;
			newObject.subType_ = SubAttributeType::NONE;
			break;
		}
	}

	if (ImGui::Button("clear")) {
		debug_BasePlacementObj_.clear();
	}

	// -------------------------------------------------
	// ↓ 生成された仮のオブジェクトの位置を調整する
	// -------------------------------------------------
	uint32_t popIndex = 0;
	for (std::list<ObstaclesManager::ObjectData>::iterator it = debug_BasePlacementObj_.begin(); it != debug_BasePlacementObj_.end();) {
		std::string name = GetObjectString((*it).type_).c_str() + std::to_string(popIndex);
		Vector3 translate = (*it).object_->GetTransform()->GetTranslation();
		Vector3 scale = (*it).object_->GetTransform()->GetScale();
		(*it).object_->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
		if (ImGui::TreeNode(name.c_str())) {
			if (ImGui::TreeNode("scale")) {
				ImGui::DragFloat3("scale", &scale.x, 0.1f);
				ImGui::TreePop();
			}
			if (ImGui::TreeNode("rotate")) {
				(*it).object_->GetTransform()->Debug_Quaternion();
				ImGui::TreePop();
			}
			if (ImGui::TreeNode("translation")) {
				ImGui::DragFloat3("translation", &translate.x, 0.1f);
				ImGui::TreePop();
			}
			(*it).object_->SetColor({ 1.0f, 0.0f, 0.0f, 1.0f });
			SubTypeChange((*it).subType_);
			(*it).object_->SetSubType((*it).subType_);

			std::string deleteName = "delete/" + name;
			if (ImGui::Button(deleteName.c_str())) {
				it = debug_BasePlacementObj_.erase(it);
				ImGui::TreePop();
				continue;
			}
			ImGui::TreePop();
		}

		(*it).object_->GetTransform()->SetTranslaion(translate);
		(*it).object_->GetTransform()->SetScale(scale);

		++it;
		++popIndex;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　編集を行う
//////////////////////////////////////////////////////////////////////////////////////////////////

void PlacementObjectEditer::Edit_Config() {
	if (ImGui::Button("Inport")) {
		inport_BasePlacementObj_.clear();
		LoadAllFile();
		Inport();
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

	ImGui::InputText(".json##ReName", &reNameInportFileName_[0], sizeof(char) * 64);

	// -------------------------------------------------
	// ↓ 生成するファイルのレベルを決める
	// -------------------------------------------------
	ImGui::Text("editLevel");
	if (ImGui::Button("-")) { editLevel_--; }
	ImGui::SameLine();
	if (ImGui::Button("+")) { editLevel_++; }
	ImGui::SameLine();
	ImGui::Text(": %d", editLevel_);
	editLevel_ = std::clamp(static_cast<int>(editLevel_), 0, 10);

	// -------------------------------------------------
	// ↓ 生成する種類を選択する
	// -------------------------------------------------
	ImGui::Text("newObjType");
	NewObjectTypeSelect();

	// -------------------------------------------------
	// ↓ 新しく追加する
	// -------------------------------------------------
	if (ImGui::Button("AddList")) {
		auto& newObject = inport_BasePlacementObj_.emplace_back(std::make_unique<BasePlacementObject>());
		switch (newPopType_) {
		case PlacementObjType::ROCK:
			newObject.object_.reset(new Rock);
			newObject.object_->Init();
			newObject.type_ = PlacementObjType::ROCK;
			newObject.subType_ = SubAttributeType::SMALL;
			break;
		case PlacementObjType::FISH:
			newObject.object_.reset(new Fish);
			newObject.object_->Init();
			newObject.type_ = PlacementObjType::FISH;
			newObject.subType_ = SubAttributeType::SMALL;
			break;
		case PlacementObjType::BIRD:
			newObject.object_.reset(new Bird);
			newObject.object_->Init();
			newObject.type_ = PlacementObjType::BIRD;
			newObject.subType_ = SubAttributeType::NONE;
			break;
		case PlacementObjType::DRIFTWOOD:
			newObject.object_.reset(new Driftwood);
			newObject.object_->Init();
			newObject.type_ = PlacementObjType::DRIFTWOOD;
			newObject.subType_ = SubAttributeType::NONE;
			break;
		}
	}

	// -------------------------------------------------
	// ↓ 生成されたオブジェクトの位置を調整する
	// -------------------------------------------------
	uint32_t popIndex = 0;
	for (std::list<ObstaclesManager::ObjectData>::iterator it = inport_BasePlacementObj_.begin(); it != inport_BasePlacementObj_.end();) {
		std::string name = GetObjectString((*it).type_).c_str() + std::to_string(popIndex);
		Vector3 translate = (*it).object_->GetTransform()->GetTranslation();
		Vector3 scale = (*it).object_->GetTransform()->GetScale();

		(*it).object_->SetIsLighting(false);
		if (ImGui::TreeNode(name.c_str())) {
			if (ImGui::TreeNode("scale")) {
				ImGui::DragFloat3("scale", &scale.x, 0.1f);
				ImGui::TreePop();
			}
			if (ImGui::TreeNode("rotate")) {
				(*it).object_->GetTransform()->Debug_Quaternion();
				ImGui::TreePop();
			}
			if (ImGui::TreeNode("translation")) {
				ImGui::DragFloat3("translation", &translate.x, 0.1f);
				ImGui::TreePop();
			}
			SubTypeChange((*it).subType_);
			(*it).object_->SetSubType((*it).subType_);

			(*it).object_->SetIsLighting(true);

			std::string deleteName = GetObjectString((*it).type_).c_str() + std::to_string(popIndex);
			deleteName = "delete/" + deleteName;
			if (ImGui::Button(deleteName.c_str())) {
				it = inport_BasePlacementObj_.erase(it);
				ImGui::TreePop();
				continue;
			}
			ImGui::TreePop();
		}

		(*it).object_->GetTransform()->SetTranslaion(translate);
		(*it).object_->GetTransform()->SetScale(scale);

		++it;
		++popIndex;
	}

	// -------------------------------------------------
	// ↓ 再保存する
	// -------------------------------------------------
	if (!inport_BasePlacementObj_.empty()) {
		if (ImGui::Button("Save_Edit")) {
			Save(inportFileName_, inport_BasePlacementObj_, editLevel_);
		}
		ImGui::SameLine();
		if (ImGui::Button("ReNameSave_Edit")) {
			std::filesystem::rename(
				obstaclesManager_->GetDirectoryPath() + inportFileName_ + ".json",
				obstaclesManager_->GetDirectoryPath() + std::to_string(reNameInportFileName_[0]) + ".json");
			//Save(reNameInportFileName_, inport_BasePlacementObj_, editLevel_);
		}
	}

	if (ImGui::Button("Clear")) {
		inport_BasePlacementObj_.clear();
	}
	
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　listに登録されているobjectの情報を外部ファイルに格納する
//////////////////////////////////////////////////////////////////////////////////////////////////

void PlacementObjectEditer::Save(const std::string& fileName,
								 const std::list<ObstaclesManager::ObjectData>& list,
								 uint32_t level) {
	// -------------------------------------------------
	// ↓ 書き込むようにデータを登録する
	// -------------------------------------------------
	json root;
	// rootにデータを書き込む
	uint32_t index = 0;
	for (const ObstaclesManager::ObjectData& obj : list) {
		std::string objId = GetObjectString(obj.type_) + std::to_string(index);
		// 格納したい情報を登録する
		const auto& transform = obj.object_->GetTransform();
		Vector3 translation = transform->GetTranslation();
		Quaternion rotate = transform->GetQuaternion();
		Vector3 scale = transform->GetScale();
		json pos = json::array({ translation.x, translation.y, translation.z });
		json rotateJoson = json::array({ rotate.x, rotate.y, rotate.z, rotate.w });
		json scaleJson = json::array({ scale.x, scale.y, scale.z });
		root[fileName]["level"] = level;
		root[fileName][objId] = {
			{"position",pos },
			{"rotate", rotateJoson},
			{"scale", scaleJson},
			{"radius", obj.object_->GetRadius()},
			{"objType", obj.type_},
			{"subType", obj.subType_}
		};
		++index;
	}

	// -------------------------------------------------
	// ↓ 実際にファイルを保存する
	// -------------------------------------------------
	std::string filePath = obstaclesManager_->GetDirectoryPath() + fileName + ".json";
	std::ofstream ofs;
	// ファイルを書き込みように開く
	ofs.open(filePath);

	// ファイルオープンが出来ているか
	if (!ofs.fail()) {
		assert("not open File");
	}

	// ファイルにjson文字列を書き込む(インデント幅4)
	ofs << std::setw(4) << root << std::endl; // rootにあるデータをjson文字列に変換してファイルへ
	// 閉じる
	ofs.close();
}

void PlacementObjectEditer::Inport() {
	for (size_t oi = 0; oi < groupMap_[inportFileName_].loadData_.size(); ++oi) {
		auto& objData = groupMap_[inportFileName_].loadData_;
		auto& obj = inport_BasePlacementObj_.emplace_back(std::make_unique<BasePlacementObject>());
		Quaternion rotate = { objData[oi].rotate_.x,objData[oi].rotate_.y,objData[oi].rotate_.z,objData[oi].rotate_.w };
		Vector3 createPos = objData[oi].pos_;
		switch (objData[oi].type_) {
		case PlacementObjType::ROCK:
			obj.object_.reset(new Rock);
			obj.object_->Init();
			obj.object_->ApplyLoadData(objData[oi].scale_, rotate, createPos, objData[oi].subType_);
			obj.type_ = PlacementObjType::ROCK;
			obj.subType_ = objData[oi].subType_;
			break;
		case PlacementObjType::FISH:
			obj.object_.reset(new Fish);
			obj.object_->Init();
			obj.object_->ApplyLoadData(objData[oi].scale_, rotate, createPos,  objData[oi].subType_);
			obj.type_ = PlacementObjType::FISH;
			obj.subType_ = objData[oi].subType_;
			break;
		case PlacementObjType::BIRD:
			obj.object_.reset(new Bird);
			obj.object_->Init();
			obj.object_->ApplyLoadData(objData[oi].scale_, rotate, createPos, objData[oi].subType_);
			obj.type_ = PlacementObjType::BIRD;
			obj.subType_ = objData[oi].subType_;
			break;
		case PlacementObjType::DRIFTWOOD:
			obj.object_.reset(new Driftwood);
			obj.object_->Init();
			obj.object_->ApplyLoadData(objData[oi].scale_, rotate, createPos,  objData[oi].subType_);
			obj.type_ = PlacementObjType::DRIFTWOOD;
			obj.subType_ = objData[oi].subType_;
			break;
		}
	}

	editLevel_ = groupMap_[inportFileName_].level;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　objectごとに対応した文字列型返す
//////////////////////////////////////////////////////////////////////////////////////////////////

std::string PlacementObjectEditer::GetObjectString(const PlacementObjType& type) {
	switch (type) {
	case PlacementObjType::ROCK:
		return std::string("ROCK_");
	case PlacementObjType::FISH:
		return std::string("FISH_");
	case PlacementObjType::BIRD:
		return std::string("BIRD_");
	case PlacementObjType::DRIFTWOOD:
		return std::string("DRIFTWOOD_");
	default:
		return std::string("???");
	}
}

void PlacementObjectEditer::NewObjectTypeSelect() {
	if (ImGui::RadioButton("ROCK", newPopType_ == PlacementObjType::ROCK)) {
		newPopType_ = PlacementObjType::ROCK;
	}
	ImGui::SameLine();
	if (ImGui::RadioButton("FISH", newPopType_ == PlacementObjType::FISH)) {
		newPopType_ = PlacementObjType::FISH;
	}
	ImGui::SameLine();
	if (ImGui::RadioButton("BIRD", newPopType_ == PlacementObjType::BIRD)) {
		newPopType_ = PlacementObjType::BIRD;
	}
	ImGui::SameLine();
	if (ImGui::RadioButton("DRIFTWOOD", newPopType_ == PlacementObjType::DRIFTWOOD)) {
		newPopType_ = PlacementObjType::DRIFTWOOD;
	}

}

void PlacementObjectEditer::SubTypeChange(SubAttributeType& subType) {
	if (ImGui::RadioButton("NONE", subType == SubAttributeType::NONE)) {
		subType = SubAttributeType::NONE;
	}
	ImGui::SameLine();
	if (ImGui::RadioButton("SMALL", subType == SubAttributeType::SMALL)) {
		subType = SubAttributeType::SMALL;
	}
	ImGui::SameLine();
	if (ImGui::RadioButton("MIDIUM", subType == SubAttributeType::MIDIUM)) {
		subType = SubAttributeType::MIDIUM;
	}
	ImGui::SameLine();
	if (ImGui::RadioButton("LARGE", subType == SubAttributeType::LARGE)) {
		subType = SubAttributeType::LARGE;
	}

	if (ImGui::RadioButton("NOTMOVE", subType == SubAttributeType::NOTMOVE)) {
		subType = SubAttributeType::NOTMOVE;
	}
}

#endif