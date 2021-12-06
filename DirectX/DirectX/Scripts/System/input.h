#ifndef __INPUT_H_
#define __INPUT_H_

#include <Windows.h>
enum class KeyCode : int
{
	LBUTTON = 0x01,
	RBUTTON,
	MBUTTON = 0x04,
	XXBUTTON1,	// 仮名
	XXBUTTON2,	// 仮名

	BACK = 0x08,
	TAB,
	RETURN = 0x0D,
	
	SHIFT = 0x10,
	CONTROL,
	ALT,
	PAUSE,
	CAPITAL,
	KANA = 0x15,
	HANGEUL = 0x15,
	HANGUL = 0x15,

	JUNJA = 0x17,
	FINAL,
	HANJA = 0x19,
	KAMJI = 0x19,

	ESCAPE = 0x1B,
	CONVERT,
	NONCONVERT,
	ACCEPT,

	SPACE = 0x20,
	PRIOR,NEXT,	END,HOME,
	LEFT,UP,RIGHT,DOWN,

	SNAPSHOT = 0x2C,
	INSERT,
	DELETEX,

	ZERO = 0x30,
	ONE,TWO,THREE,FORE,
	FIVE,SIX,SEVEN,EIGHT,NINE,

	A = 0x41,
	B,C,D,E,F,G,H,I,J,K,L,M,N,
	O,P,Q,R,S,T,U,V,W,X,Y,Z,

	NUM_0 = 0x60,
	NUM_1,NUM_2, NUM_3, NUM_4, NUM_5,
	NUM_6, NUM_7, NUM_8, NUM_9,

	MULTIPLY = 0x6A,
	ADD,
	SEPARATOR,
	SUBTRACT,
	DECIMAL,
	DIVIDE,

	F1 = 0x70,
	F2,F3,F4,F5,F6,F7,F8,F9,F10,F11,F12,F13,F14,
	F15,F16,F17,F18,F19,F20,F21,F22,F23,F24,

	NUMLOCK = 0x90,
	SCROLL = 0x91,
	
	LSHIFT = 0xA0,
	RSHIFT,
	LCONTROL,
	RCONTROL,
	LALT,
	RALT,

	MAX = 0xFF + 1
};
/**
* キーを押しているか
* @param[in] KeyCode
* @return bool
*/
bool GetKeyPress(KeyCode keycode);
/**
* キーを押したか
* @param[in] KeyCode
* @return bool
*/
bool GetKeyTrigger(KeyCode keycode);
/**
* キーを離したか
* @param[in] KeyCode
* @return bool
*/
bool GetKeyRelease(KeyCode keycode);
/**
* key配列の情報を更新
* @brief 毎フレーム呼び出す
*/
void KeyUpdate();

#endif // !__INPTR_H_