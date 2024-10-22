#include "AudioPlayer.h"

AudioPlayer::AudioPlayer() {
}

AudioPlayer::~AudioPlayer() {
}

void AudioPlayer::Finalize() { 
	Engine::Stop(audio_);
	audio_.pSourceVoice->DestroyVoice();
	delete[]audio_.data.pBuffer;
}

void AudioPlayer::Init(const std::string& filePath) {
	audio_ = Engine::LoadAudio(AudioManager::GetInstance()->GetAudioData(filePath));
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
	Engine::SingleShotPlay(AudioManager::GetInstance()->GetAudioData(filePath), volume);
}
