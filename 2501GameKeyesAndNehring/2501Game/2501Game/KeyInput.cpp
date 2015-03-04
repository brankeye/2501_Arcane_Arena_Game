////////////////////////////////////////////////////////////////////////////////
// Filename: KeyInput.cpp
////////////////////////////////////////////////////////////////////////////////
#include "KeyInput.h"


KeyInput::KeyInput()
{
}


KeyInput::KeyInput(const KeyInput& other)
{
}


KeyInput::~KeyInput()
{
}


void KeyInput::Initialize()
{
	// Initialize all the keys to being released and not pressed.
	for(int i=0; i<256; i++)
	{
		m_keys[i] = false;
		m_keysDown[i] = false;
	}

	return;
}


void KeyInput::KeyDown(unsigned int input)
{
	// If a key is pressed then save that state in the key array.
	m_keys[input] = true;
	return;
}


void KeyInput::KeyUp(unsigned int input)
{
	// If a key is released then clear that state in the key array.
	m_keys[input] = false;
	return;
}


bool KeyInput::IsKeyDown(unsigned int key)
{
	// Return what state the key is in (pressed/not pressed).
	return m_keys[key];
}

bool KeyInput::IsKeyUp(unsigned int key)
{
	if(IsKeyDown(key) && !m_keysDown[key]) {
		m_keysDown[key] = true;
	} else if(!IsKeyDown(key) && m_keysDown[key]) {
		m_keysDown[key] = false;
		return true;
	}
	return false;
}