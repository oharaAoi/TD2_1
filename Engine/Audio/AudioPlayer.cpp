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
	audio_ = Engine::LoadAudio(filePath);
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
