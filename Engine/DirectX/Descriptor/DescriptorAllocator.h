#pragma once
#include <stack>
#include "Engine/DirectX/Descriptor/DescriptorHeap.h"

class DescriptorAllocator {
public:

	DescriptorAllocator(uint32_t totalDescriptors, uint32_t descriptorSize)
		: totalDescriptors_(totalDescriptors), descriptorSize_(descriptorSize), currentIndex_(0) {
	}
	~DescriptorAllocator();

	DescriptorHeap::DescriptorHandles Allocate(ID3D12DescriptorHeap* descriptorHeap);

	void Free(uint32_t index);

	DescriptorHeap::DescriptorHandles GetDescriptorHandle(ID3D12DescriptorHeap* descriptorHeap,
														  uint32_t index);

private:


	uint32_t totalDescriptors_;			// ディスクリプタの総数
	uint32_t descriptorSize_;			// 各ディスクリプタのサイズ
	uint32_t currentIndex_;				// 新規割り当て用のインデックス
	std::stack<uint32_t> freeStack_;	// 再利用可能なインデックスを保持するスタック

};

