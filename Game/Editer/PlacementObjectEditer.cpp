#include "PlacementObjectEditer.h"

PlacementObjectEditer::PlacementObjectEditer() {}
PlacementObjectEditer::~PlacementObjectEditer() {}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void PlacementObjectEditer::Init() {
	std::filesystem::path dire(kDirectoryPath_);
	if (!std::filesystem::exists(kDirectoryPath_)) {
		std::filesystem::create_directories(kDirectoryPath_);
	}

	LoadAllFile();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　ファイルの読み込みを行う
//////////////////////////////////////////////////////////////////////////////////////////////////

void PlacementObjectEditer::Load(const std::string& fileName) {
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

			groupMap_[topKey].loadData_.emplace_back(scale, rotate, position, radius, objType);
		}
	}
}

void PlacementObjectEditer::LoadAllFile() {
	for (const auto& entry : std::filesystem::directory_iterator(kDirectoryPath_)) {
		std::string fileName = entry.path().stem().string();
		fileNames_.push_back(fileName);
		Load(fileName);
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
	for (std::list<ObjectData>::iterator it = debug_BasePlacementObj_.begin(); it != debug_BasePlacementObj_.end();) {
		(*it).object_->Update();
		++it;
	}

	for (std::list<ObjectData>::iterator it = inport_BasePlacementObj_.begin(); it != inport_BasePlacementObj_.end();) {
		(*it).object_->Update();
		++it;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　描画処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void PlacementObjectEditer::Draw() const {
	// -------------------------------------------------
	// ↓ リスト内にある描画処理を行う
	// -------------------------------------------------
	for (std::list<ObjectData>::const_iterator it = debug_BasePlacementObj_.begin(); it != debug_BasePlacementObj_.end();) {
		(*it).object_->Draw();
		++it;
	}

	for (std::list<ObjectData>::const_iterator it = inport_BasePlacementObj_.begin(); it != inport_BasePlacementObj_.end();) {
		(*it).object_->Draw();
		++it;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　Guiの表示を行う
//////////////////////////////////////////////////////////////////////////////////////////////////

void PlacementObjectEditer::Debug_Gui() {
	ImGui::Begin("PlacementObjEditer");
	if (ImGui::Button("Reload")) {
		groupMap_.clear();
		LoadAllFile();
	}
	NewGroup_Config();
	ImGui::Separator();
	Edit_Config();
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
			newObject.object_->SetObject("skin.obj");
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
	for (std::list<ObjectData>::iterator it = debug_BasePlacementObj_.begin(); it != debug_BasePlacementObj_.end();) {
		Vector3 translate = (*it).object_->GetTransform()->GetTranslation();
		std::string name = GetObjectString((*it).type_).c_str() + std::to_string(popIndex);
		ImGui::DragFloat3(name.c_str(), &translate.x, 0.1f);
		(*it).object_->GetTransform()->SetTranslaion(translate);

		ImGui::SameLine();
		if (ImGui::Button("delete")) {
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
	// ↓ 生成されたオブジェクトの位置を調整する
	// -------------------------------------------------
	uint32_t popIndex = 0;
	for (std::list<ObjectData>::iterator it = inport_BasePlacementObj_.begin(); it != inport_BasePlacementObj_.end();) {
		Vector3 translate = (*it).object_->GetTransform()->GetTranslation();
		std::string name = GetObjectString((*it).type_).c_str() + std::to_string(popIndex);
		ImGui::DragFloat3(name.c_str(), &translate.x, 0.1f);
		(*it).object_->GetTransform()->SetTranslaion(translate);

		ImGui::SameLine();
		if (ImGui::Button("delete")) {
			it = inport_BasePlacementObj_.erase(it);
		} else {
			++it;
			++popIndex;
		}
	}

	// -------------------------------------------------
	// ↓ 再保存する
	// -------------------------------------------------
	if(!inport_BasePlacementObj_.empty()){
		if (ImGui::Button("Save_Edit")) {
			Save(inportFileName_, inport_BasePlacementObj_);
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　listに登録されているobjectの情報を外部ファイルに格納する
//////////////////////////////////////////////////////////////////////////////////////////////////

void PlacementObjectEditer::Save(const std::string& fileName, const std::list<ObjectData>& list) {
	// -------------------------------------------------
	// ↓ 書き込むようにデータを登録する
	// -------------------------------------------------
	json root;
	// rootにデータを書き込む
	uint32_t index = 0;
	for (const ObjectData& obj : list) {
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

		switch (objData[oi].type_) {
		case PlacementObjType::Test1_Type:
			obj.object_->ApplyLoadData(objData[oi].scale_, rotate, objData[oi].pos_, objData[oi].radius_);
			obj.object_->SetObject("skin.obj");
			obj.type_ = PlacementObjType::Test1_Type;
			break;
		case PlacementObjType::Test2_Type:
			obj.object_->ApplyLoadData(objData[oi].scale_, rotate, objData[oi].pos_, objData[oi].radius_);
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