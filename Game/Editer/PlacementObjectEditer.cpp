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
	std::filesystem::path dire(kDirectoryPath_);
	if (!std::filesystem::exists(kDirectoryPath_)) {
		std::filesystem::create_directories(kDirectoryPath_);
	}

	isDraw_ = true;

	LoadAllFile();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　ファイルの読み込みを行う
//////////////////////////////////////////////////////////////////////////////////////////////////

void PlacementObjectEditer::LoadAllFile() {
	fileNames_.clear();
	groupMap_.clear();
	for (const auto& entry : std::filesystem::directory_iterator(kDirectoryPath_)) {
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
	}
}

#ifdef _DEBUG
#include "Engine/Manager/ImGuiManager.h"
//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void PlacementObjectEditer::Update() {
	Debug_Gui();

	// -------------------------------------------------
	// ↓ リスト内にある更新処理を行う
	// -------------------------------------------------
	for (std::list<ObstaclesManager::ObjectData>::iterator it = debug_BasePlacementObj_.begin(); it != debug_BasePlacementObj_.end();) {
		(*it).object_->Update();
		++it;
	}

	for (std::list<ObstaclesManager::ObjectData>::iterator it = inport_BasePlacementObj_.begin(); it != inport_BasePlacementObj_.end();) {
		(*it).object_->Update();
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
	for (std::list<ObstaclesManager::ObjectData>::const_iterator it = debug_BasePlacementObj_.begin(); it != debug_BasePlacementObj_.end();) {
		(*it).object_->Draw();
		++it;
	}

	for (std::list<ObstaclesManager::ObjectData>::const_iterator it = inport_BasePlacementObj_.begin(); it != inport_BasePlacementObj_.end();) {
		(*it).object_->Draw();
		++it;
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
		Save(exportFileName_, debug_BasePlacementObj_);
	}
	ImGui::SameLine();
	ImGui::InputText(".json##SaveFineName", &exportFileName_[0], sizeof(char) * 64);

	// -------------------------------------------------
	// ↓ 生成する種類を選択する
	// -------------------------------------------------
	ImGui::Text("newObjType");
	if (ImGui::RadioButton("Test1_Type", newPopType_ == PlacementObjType::Test1_Type)) {
		newPopType_ = PlacementObjType::Test1_Type;
	}
	ImGui::SameLine();
	if (ImGui::RadioButton("Test2_Type", newPopType_ == PlacementObjType::Test2_Type)) {
		newPopType_ = PlacementObjType::Test2_Type;
	}

	// -------------------------------------------------
	// ↓ 実際に生成を行う
	// -------------------------------------------------
	if (ImGui::Button("AddList")) {
		auto& newObject = debug_BasePlacementObj_.emplace_back(std::make_unique<BasePlacementObject>());
		newObject.object_->Init();
		switch (newPopType_) {
		case PlacementObjType::Test1_Type:
			newObject.object_->SetObject("Rock.obj");
			newObject.type_ = PlacementObjType::Test1_Type;
			break;
		case PlacementObjType::Test2_Type:
			newObject.object_->SetObject("teapot.obj");
			newObject.type_ = PlacementObjType::Test2_Type;
			break;
		}
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
			ImGui::TreePop();
		}

		(*it).object_->GetTransform()->SetTranslaion(translate);
		(*it).object_->GetTransform()->SetScale(scale);

		std::string deleteName = GetObjectString((*it).type_).c_str() + std::to_string(popIndex);
		deleteName = "delete/" + deleteName;
		if (ImGui::Button(deleteName.c_str())) {
			it = debug_BasePlacementObj_.erase(it);
		} else {
			++it;
			++popIndex;
		}
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

	// -------------------------------------------------
	// ↓ 生成する種類を選択する
	// -------------------------------------------------
	ImGui::Text("newObjType");
	if (ImGui::RadioButton("Test1_Type", newPopType_ == PlacementObjType::Test1_Type)) {
		newPopType_ = PlacementObjType::Test1_Type;
	}
	ImGui::SameLine();
	if (ImGui::RadioButton("Test2_Type", newPopType_ == PlacementObjType::Test2_Type)) {
		newPopType_ = PlacementObjType::Test2_Type;
	}

	// -------------------------------------------------
	// ↓ 新しく追加する
	// -------------------------------------------------
	if (ImGui::Button("AddList")) {
		auto& newObject = inport_BasePlacementObj_.emplace_back(std::make_unique<BasePlacementObject>());
		newObject.object_->Init();
		switch (newPopType_) {
		case PlacementObjType::Test1_Type:
			newObject.object_->SetObject("Rock.obj");
			newObject.type_ = PlacementObjType::Test1_Type;
			break;
		case PlacementObjType::Test2_Type:
			newObject.object_->SetObject("teapot.obj");
			newObject.type_ = PlacementObjType::Test2_Type;
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

			ImGui::TreePop();
		}

		(*it).object_->GetTransform()->SetTranslaion(translate);
		(*it).object_->GetTransform()->SetScale(scale);

		std::string deleteName = GetObjectString((*it).type_).c_str() + std::to_string(popIndex);
		deleteName = "delete/" + deleteName;
		if (ImGui::Button(deleteName.c_str())) {
			it = inport_BasePlacementObj_.erase(it);
		} else {
			++it;
			++popIndex;
		}
	}

	// -------------------------------------------------
	// ↓ 再保存する
	// -------------------------------------------------
	if (!inport_BasePlacementObj_.empty()) {
		if (ImGui::Button("Save_Edit")) {
			Save(inportFileName_, inport_BasePlacementObj_);
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　listに登録されているobjectの情報を外部ファイルに格納する
//////////////////////////////////////////////////////////////////////////////////////////////////

void PlacementObjectEditer::Save(const std::string& fileName, const std::list<ObstaclesManager::ObjectData>& list) {
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
		root[fileName][objId] = {
			{"position",pos },
			{"rotate", rotateJoson},
			{"scale", scaleJson},
			{"radius", obj.object_->GetRadius()},
			{"objType", obj.type_}
		};
		++index;
	}

	// -------------------------------------------------
	// ↓ 実際にファイルを保存する
	// -------------------------------------------------
	std::string filePath = kDirectoryPath_ + fileName + ".json";
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
		obj.object_->Init();
		Quaternion rotate = { objData[oi].rotate_.x,objData[oi].rotate_.y,objData[oi].rotate_.z,objData[oi].rotate_.w };
		Vector3 createPos = objData[oi].pos_;
		switch (objData[oi].type_) {
		case PlacementObjType::Test1_Type:
			obj.object_->ApplyLoadData(objData[oi].scale_, rotate, createPos, objData[oi].radius_);
			obj.object_->SetObject("skin.obj");
			obj.type_ = PlacementObjType::Test1_Type;
			break;
		case PlacementObjType::Test2_Type:
			obj.object_->ApplyLoadData(objData[oi].scale_, rotate, createPos, objData[oi].radius_);
			obj.object_->SetObject("teapot.obj");
			obj.type_ = PlacementObjType::Test2_Type;
			break;
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　objectごとに対応した文字列型返す
//////////////////////////////////////////////////////////////////////////////////////////////////

std::string PlacementObjectEditer::GetObjectString(const PlacementObjType& type) {
	switch (type) {
	case PlacementObjType::Test1_Type:
		return std::string("Test1_");
	case PlacementObjType::Test2_Type:
		return std::string("Test2_");
	default:
		return std::string("???");
	}
}

#endif