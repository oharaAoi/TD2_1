#include "PlayerAnimator.h"

PlayerAnimator::PlayerAnimator() {
}

PlayerAnimator::~PlayerAnimator() {
}

void PlayerAnimator::Init() {
}

void PlayerAnimator::Update() {
	Quaternion moveRotate;
	// joint(bone)の変更を行う
	//for (auto& pair : joints_) {
	//	ImGui::DragFloat4(pair.first.c_str(), &moveRotate.x, 0.01f);
	//	pair.second->transform.rotate = moveRotate * pair.second->transform.rotate;
	//	moveRotate = Quaternion();
	//}

	/*if (ImGui::TreeNode("originQuaternion")) {
		for (auto& pair : joints_) {
			ImGui::DragFloat4(pair.first.c_str(), &pair.second->transform.rotate.x, 0.01f);
		}
		ImGui::TreePop();
	}*/

	// jointの更新
	skeleton_->Update();
	for (size_t oi = 0; oi < skinning_.size(); ++oi) {
		skinning_[oi]->Update(skeleton_.get());
	}
}

void PlayerAnimator::LoadAnimation(Model* model) {
	skeleton_ = std::make_unique<Skeleton>();
	skeleton_->CreateSkeleton(model->GetNode());
	skeleton_->Init();
	for (size_t oi = 0; oi < model->GetMeshsNum(); ++oi) {
		skinning_.push_back(Engine::CreateSkinning(skeleton_.get(), model, (uint32_t)oi));
	}
	/*skinning_ = Engine::CreateSkinning(skeleton_.get(), model);*/

	// jointの参照をポインタとして保持
	std::vector<Skeleton::Joint>& joints = skeleton_->GetJoints();
	for (const Skeleton::Joint& joint : joints) {
		joints_[joint.name] = const_cast<Skeleton::Joint*>(&joint);
	}
}

