#include "ProjectComponents.h"
#include <iostream>

void dae::ProjectComponent::Update(const float)
{
	static bool isPrinted = false;
	if (isPrinted == false)
	{
		isPrinted = true;
		std::cout << "ProjectComponent reached\n";
	}
}
