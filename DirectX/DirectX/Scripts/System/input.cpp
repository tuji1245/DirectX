#include "input.h"

static BYTE key[(int)KeyCode::MAX];
static BYTE prevKey[(int)KeyCode::MAX];

bool GetKeyPress(KeyCode keycode)
{
	return key[(int)keycode];
}
bool GetKeyTrigger(KeyCode keycode)
{
	return !prevKey[(int)keycode] && key[(int)keycode];
}
bool GetKeyRelease(KeyCode keycode)
{
	return prevKey[(int)keycode] && !key[(int)keycode];
}

void KeyUpdate()
{
	memcpy(prevKey, key, sizeof(key));
	for (int i = 0; i < (int)KeyCode::MAX; i++)
		key[i] = GetAsyncKeyState(i);
}
