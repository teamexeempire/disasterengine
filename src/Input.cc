#include "Input.h"

bool Input::IsKeyDown(int keycode)
{
	return (keyStates.count(keycode) && keyStates[keycode] && prevKeyStates.count(keycode) && !prevKeyStates[keycode]);
}

bool Input::IsKeyPressed(int keycode)
{
	return (keyStates.count(keycode) && keyStates[keycode]); 
}

bool Input::IsKeyUp(int keycode)
{
	return (keyStates.count(keycode) && !keyStates[keycode] && prevKeyStates.count(keycode) && prevKeyStates[keycode]);
}
