#pragma once
#include <list>
#include <vector>
#include "Engine/GameObject/BaseGameObject.h"

class AnimetionEffectManager {
public:

	struct EffectData {
		std::unique_ptr<BaseGameObject> effect;
		const Vector3* pTranslation;
		const Quaternion* pQuaternion;
	};


public:

	AnimetionEffectManager() = default;
	~AnimetionEffectManager();
	AnimetionEffectManager(const AnimetionEffectManager&) = delete;
	const AnimetionEffectManager& operator=(const AnimetionEffectManager&) = delete;

	static AnimetionEffectManager* GetInstance();

	void Finalize();
	void Init();
	void Update();
	void Draw() const;

	/// <summary>
	/// Effectを作成してリストに追加
	/// </summary>
	/// <param name="directoryPath">: ディレクトリパス</param>
	/// <param name="filePath">: ファイルパス</param>
	/// <param name="parentWorldTransform">: 親がいるならworldTransformのポインタを渡す</param>
	static void AddListEffect(const std::string& directoryPath, const std::string& filePath,
							  const WorldTransform* parentWorldTransform, bool isParent);

private:

	std::list<EffectData> effectList_;

};

