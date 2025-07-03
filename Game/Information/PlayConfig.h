#pragma once

enum LanguageSetting{
	LANGUAGE_JP = 0,
	LANGUAGE_EN,
};

struct PlayConfig{
	static inline LanguageSetting language = LANGUAGE_EN;
};