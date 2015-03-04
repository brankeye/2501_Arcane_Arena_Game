// main.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "collisionBoxLinkedList.h"

#include <windows.h>	// Sleep Function
#include <iostream>     // for std::cout
#include <vector>		// for std::vector and std::string

using namespace DirectX;    // for std::cout, std::endl, std::vector, and std::string

int _tmain(int argc, _TCHAR* argv[])
{
	// Create linked list
	collisionBoxLinkedList collisionBoxList;

	// Create collisionBox pointers
	collisionBox myFirstCollisionBox(XMFLOAT2(35.0f, 45.0f), 5.0f, 5.0f);
	collisionBox mySecondCollisionBox(XMFLOAT2(20.0f, 20.0f), 20.0f, 20.0f);

	// Add to the linked list
	collisionBoxList.add(&myFirstCollisionBox);
	collisionBoxList.add(&mySecondCollisionBox);

	// Set a Test Speed and Direction
	float speed = 1.0f;
	std::string str = "up";

	// Move The collision box and check for collisions
	while(true) {
		std::cout << "Collision Detected = " << collisionBoxList.checkForCollision(speed, str, &myFirstCollisionBox) << std::endl;

		std::cout << "X Position 1: " << myFirstCollisionBox.getXPosition() << std::endl;
		std::cout << "Y Position 1: " << myFirstCollisionBox.getYPosition() << std::endl;

		std::cout << "X Position 2: " << mySecondCollisionBox.getXPosition() << std::endl;
		std::cout << "Y Position 2: " << mySecondCollisionBox.getYPosition() << std::endl;


		// Delay 5 seconds and countinue.
		Sleep(5000);
	}
	return 0;
}

