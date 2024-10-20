#pragma once
#include <map>
#include <string>
#include <assert.h>

class AudioManager {
public:

	AudioManager() = default;
	~AudioManager();
	AudioManager(const AudioManager&) = delete;
	const AudioManager& operator=(const AudioManager&) = delete;

	static AudioManager* GetInstance();

	void Init();
	void AddMap(const std::string& directoryPath, const std::string& fileName);

	std::string GetAudioName(const std::string& fileName);

	static void LoadAudio(const std::string& directoryPath, const std::string& fileName);

private:

	std::map<std::string, std::string> audioNames_;

};

