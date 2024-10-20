#include "AudioManager.h"


AudioManager::~AudioManager() {
	audioNames_.clear();
}

AudioManager* AudioManager::GetInstance() {
	static AudioManager instance;
	return &instance;
}

void AudioManager::Init() {
	audioNames_.clear();
}

void AudioManager::AddMap(const std::string& directoryPath, const std::string& fileName) {
	std::string name = directoryPath + fileName;
	if (auto it = audioNames_.find(fileName); it != audioNames_.end()) {
		return;
	}
	audioNames_.try_emplace(fileName, name);
}

std::string AudioManager::GetAudioName(const std::string& fileName) {
	if (auto it = audioNames_.find(fileName); it == audioNames_.end()) {
		assert(false && "Audio not found!");
	}

	return audioNames_[fileName];
}

void AudioManager::LoadAudio(const std::string& directoryPath, const std::string& fileName) {
	GetInstance()->AddMap(directoryPath, fileName);
}
