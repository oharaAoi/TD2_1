#pragma once
#include <xaudio2.h>
#pragma comment(lib, "xaudio2.lib")
#include <fstream>
#include <wrl.h>
#include <cassert>
#include <list>

// チャンクヘッダ
struct ChunkHeader {
	char id[4];		// チャンク毎のID
	int32_t size;	// チャンクサイズ
};

// RIFFヘッダチャンク
struct RiffHeader {
	ChunkHeader Chunk;	// RIFF
	char type[4];		// WAVE
};

// FMTチャンク
struct FormatChunk {
	ChunkHeader chunk;	// fmt
	WAVEFORMATEX fmt;	// 波形フォーマット
};

// 音声データ
struct SoundData {
	// 波形フォーマット
	WAVEFORMATEX wfex;
	// バッファ先頭アドレス
	BYTE* pBuffer;
	// バッファサイズ
	unsigned int bufferSize;
};

struct LoadData {
	WAVEFORMATEX fmt;
	char* pBuffer;
	uint32_t dataSize;
};

struct AudioData {
	SoundData data;
	IXAudio2SourceVoice* pSourceVoice;
};

class Audio {
public: // データ構造体

public:

	template<typename T>
	using ComPtr = Microsoft::WRL::ComPtr <T>;

public:

	Audio() {};
	~Audio();

	/// <summary>
	/// 初期化
	/// </summary>
	void Init();

	void Update();

	/// <summary>
	/// 終了処理
	/// </summary>
	void Finalize();

	/// <summary>
	/// 音声データの読み込み
	/// </summary>
	/// <param name="filename"></param>
	/// <returns></returns>
	SoundData SoundLoadWave(const char* filename);

	LoadData LoadWave(const char* filename);

	AudioData LoadAudio(const LoadData& loadAudioData);

	/// <summary>
	/// 音声データの解放
	/// </summary>
	/// <param name="soundData"></param>
	void SoundUnload(SoundData* soundData);

	/// <summary>
	/// サウンドの再生
	/// </summary>
	/// <param name="xAudio2"></param>
	/// <param name="soundData"></param>
	void SoundPlayWave(const SoundData& soundData);

	void PlayAudio(const AudioData& audioData, bool isLoop, float volume, bool checkPlaying = false);

	void SinglShotPlay(const LoadData& loadAudioData, float volume);

	/// <summary>
	/// サウンドの停止
	/// </summary>
	/// <param name="soundData"></param>
	void StopAudio(IXAudio2SourceVoice* pSourceVoice);

	/// <summary>
	/// サウンドの一時停止
	/// </summary>
	/// <param name="soundData"></param>
	void PauseAudio(IXAudio2SourceVoice* pSourceVoice);

	/// <summary>
	/// 一時停止からの再生
	/// </summary>
	/// <param name="soundData"></param>
	void ReStartAudio(IXAudio2SourceVoice* pSourceVoice);

	/// <summary>
	/// 音量の設定
	/// </summary>
	/// <param name="pSourceVoice"></param>
	void SetVolume(IXAudio2SourceVoice* pSourceVoice, float volume);

	/// <summary>
	/// サウンドが再生中かどうか
	/// </summary>
	/// <param name="soundData"></param>
	bool IsPlaying(IXAudio2SourceVoice* pSourceVoice);

private:

	ComPtr<IXAudio2> xAudio2_;
	IXAudio2MasteringVoice* masterVoice_;

	std::list<IXAudio2SourceVoice*> playingSourceList_;
};

