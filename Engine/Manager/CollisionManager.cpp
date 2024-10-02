#include "CollisionManager.h"

CollisionManager::CollisionManager() {}
CollisionManager::~CollisionManager() {}


/////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void CollisionManager::Init() {
	colliders_.clear();
}

void CollisionManager::Draw(const Matrix4x4& vpMat)  const {
	std::list<Collider*>::const_iterator iter = colliders_.begin();
	for (; iter != colliders_.end(); ++iter) {
		AABB aabb = { (*iter)->GetWorldTranslation(),(*iter)->GetRadius()};
		DrawAABB(aabb, vpMat, (*iter)->GetIsHitting() ? Vector4{1.0f, 0.0f, 0.0f, 1.0f} : Vector4{1.0f, 1.0f, 1.0f, 1.0f});
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　すべての当たり判定チェック
//////////////////////////////////////////////////////////////////////////////////////////////////

void CollisionManager::CheckAllCollision() {
	// リスト内のペアの総当たり判定
	std::list<Collider*>::iterator iterA = colliders_.begin();
	for (; iterA != colliders_.end(); ++iterA) {
		Collider* colliderA = *iterA;

		// イテレータBはイテレータAの次の要素から回す
		std::list<Collider*>::iterator iterB = iterA;
		iterB++;

		for (; iterB != colliders_.end(); ++iterB) {
			Collider* colliderB = *iterB;
			// ペアの当たり判定
			CheckCollisionPair(colliderA, colliderB);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　コライダー2つの衝突判定と応答
//////////////////////////////////////////////////////////////////////////////////////////////////

void CollisionManager::CheckCollisionPair(Collider* colliderA, Collider* colliderB) {
	colliderA->SetIsHitting(false);
	colliderB->SetIsHitting(false);

	Vector3 posA = colliderA->GetWorldTranslation();
	Vector3 posB = colliderB->GetWorldTranslation();
	// 差分ベクトル
	Vector3 subtract = posB - posA;
	// 座標AとBの距離を求める
	float distance = subtract.Length();
	// 球と球の交差判定
	if (colliderA->GetRadius() + colliderB->GetRadius() > distance) {
		// それぞれの衝突時コールバック関数を呼び出す
		colliderA->OnCollision(colliderB);
		colliderB->OnCollision(colliderA);

		colliderA->SetIsHitting(true);
		colliderB->SetIsHitting(true);
	}
}

