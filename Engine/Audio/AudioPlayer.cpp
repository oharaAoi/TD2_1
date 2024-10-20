#include "AudioPlayer.h"

AudioPlayer::AudioPlayer() {
}

AudioPlayer::~AudioPlayer() {
}

void AudioPlayer::Finalize() {
	audio_.pSourceVoice->DestroyVoice();
	delete[]audio_.data.pBuffer;
}

void AudioPlayer::Init(const std::string& filePath) {
	std::string path = AudioManager::GetInstance()->GetAudioName(filePath);
	audio_ = Engine::LoadAudio(path);
}

void AudioPlayer::Play(bool isLoop, float volume, bool checkPlaying) {
	Engine::PlayAudio(audio_, isLoop, volume, checkPlaying);
}

void AudioPlayer::Pause() {
	Engine::Pause(audio_);
}

void AudioPlayer::ReStart() {
	Engine::ReStart(audio_);
}

void AudioPlayer::Stop() {
	Engine::Stop(audio_);
}

void AudioPlayer::SinglShotPlay(const std::string& filePath, float volume) {
	std::string path = AudioManager::GetInstance()->GetAudioName(filePath);
	Engine::SingleShotPlay(path, volume);
}
