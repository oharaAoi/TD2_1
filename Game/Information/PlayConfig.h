#pragma once

enum LanguageSetting{
	LANGUAGE_JP = 0,
	LANGUAGE_EN,
};

enum InputMode{
	INPUTTYPE_KEYBOARD,
	INPUTTYPE_PAD
};


struct PlayConfig{
	static inline LanguageSetting language = LANGUAGE_JP;
	static inline InputMode inputMode = INPUTTYPE_KEYBOARD;
};