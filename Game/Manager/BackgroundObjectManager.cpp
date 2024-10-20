#include "BackgroundObjectManager.h"

BackgroundObjectManager::~BackgroundObjectManager(){}

////////////////////////////////////////////////////////////////////////////////
//									初期化
////////////////////////////////////////////////////////////////////////////////

void BackgroundObjectManager::Initialize(Player* pPlayer){

	pPlayer_ = pPlayer;
	currentRightEnd_ = 0.0f;

	AddObjects();

}

////////////////////////////////////////////////////////////////////////////////
//									更新
////////////////////////////////////////////////////////////////////////////////

void BackgroundObjectManager::Update(){

	AddObjects();
	EraseObjects();
}

////////////////////////////////////////////////////////////////////////////////
//									描画
////////////////////////////////////////////////////////////////////////////////

void BackgroundObjectManager::Draw(){

	for(auto& wood : woods_){
		wood->Draw();
	}

	for(auto& rock : rocks_){
		rock->Draw();
	}

	for(auto& grass : grasses_){
		grass->Draw();
	}
}

////////////////////////////////////////////////////////////////////////////////
//								オブジェクトの作成
////////////////////////////////////////////////////////////////////////////////

void BackgroundObjectManager::AddObjects(){

	int x = int32_t((pPlayer_->GetWorldTranslation().x + kGenerateRange_.x) - currentRightEnd_) / subdivision_;
	int z = int32_t(kGenerateRange_.y) / subdivision_;

	// 配置
	for(int32_t i = 0; i < x; i++){
		for(int32_t j = 0; j < z; j++){
			int32_t rand = RandomInt(0, 10);

			if(rand == 0){

				auto& wood = woods_.emplace_back(std::make_unique<BaseGameObject>());
				float scale = RandomFloat(1.0f, 1.5f);
				float rotate = RandomFloat(0.0f, 6.28f);

				wood->Init();
				wood->SetObject("Wood.obj");
				wood->GetTransform()->SetTranslaion({ currentRightEnd_ + float(subdivision_ * i),0.0f,baseZ_ + float(z * j) });
				wood->GetTransform()->SetQuaternion(Quaternion::AngleAxis(rotate, { 0,1,0 }));
				wood->GetTransform()->SetScale({ scale,scale,scale });
				wood->Update();
				continue;

			} else if(rand == 1 or rand == 2){

				auto& rock = rocks_.emplace_back(std::make_unique<BaseGameObject>());
				float scale = RandomFloat(1.0f, 1.5f);
				float rotate = RandomFloat(0.0f, 6.28f);

				rock->Init();
				rock->SetObject("Rock.obj");
				rock->GetTransform()->SetTranslaion({ currentRightEnd_ + float(subdivision_ * i),0.0f,baseZ_ + float(z * j) });
				rock->GetTransform()->SetQuaternion(Quaternion::AngleAxis(rotate, { 0,1,0 }));
				rock->GetTransform()->SetScale({ scale,scale,scale });
				rock->Update();
				continue;

			} else if(rand >= 3 &&  rand <= 6){

				auto& grass = rocks_.emplace_back(std::make_unique<BaseGameObject>());
				float scale = RandomFloat(1.0f, 1.5f);
				float rotate = RandomFloat(0.0f, 6.28f);

				grass->Init();
				grass->SetObject("Grass2.obj");
				grass->GetTransform()->SetTranslaion({ currentRightEnd_ + float(subdivision_ * i),0.0f,baseZ_ + float(z * j) });
				grass->GetTransform()->SetQuaternion(Quaternion::AngleAxis(rotate, { 0,1,0 }));
				grass->GetTransform()->SetScale({ scale,scale,scale });
				grass->Update();
				continue;
			}

		}

		currentRightEnd_ += float(subdivision_);
	}
}

////////////////////////////////////////////////////////////////////////////////
//							オブジェクトの削除
////////////////////////////////////////////////////////////////////////////////

void BackgroundObjectManager::EraseObjects(){
	
	// プレイヤーの座標を取得
	static Vector3 playerPos{};
	playerPos = pPlayer_->GetWorldTranslation();


	// 削除
	woods_.remove_if([](auto& object){return object->GetTransform()->GetTranslation().x < playerPos.x - 20.0f;});
	rocks_.remove_if([](auto& object){return object->GetTransform()->GetTranslation().x < playerPos.x - 20.0f; });
	grasses_.remove_if([](auto& object){return object->GetTransform()->GetTranslation().x < playerPos.x - 20.0f; });
}
