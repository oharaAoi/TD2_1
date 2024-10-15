#pragma once
#include <cstdint>

enum class ObjectType : int32_t{
	// 属性無し
	NONE = -1,
	// 最下位ビットにプレイヤーかそれ以外かの属性を定義
	PLAYER = 0b0,
	OTHER = 0b1,
	// 組み合わせ
	OBSTACLE = 0b10,
	ROCK = OBSTACLE | 0b100,
	FISH = OBSTACLE | 0b1000,
	BIRD = OBSTACLE | 0b10000,
	ITEM = OBSTACLE | 0b100000,
};