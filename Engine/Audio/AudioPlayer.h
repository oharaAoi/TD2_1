#pragma once
#include "Engine/Engine.h"
#include "Engine/Audio/Audio.h"

class AudioPlayer {
public:

	AudioPlayer();
	~AudioPlayer();

	void Finalize();
	void Init(const std::string& filePath);
	/// <summary>
	/// 音をプレイする
	/// </summary>
	/// <param name="isLoop">: ループするか</param>
	/// <param name="volume">: 音量</param>
	/// <param name="checkPlaying">: プレイ中かの判定を行うかどうか(デフォルトではfalse)</param>
	void Play(bool isLoop, float volume, bool checkPlaying = false);
	void Pause();
	void ReStart();
	void Stop();

private:

	AudioData audio_;

};

