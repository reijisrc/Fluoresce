//==============================================================================//
// Name : Application.cpp														// 
// Describe : 	アプリケーション												// 
// Author : Ding Qi																// 
// Create Date : 2022/03/18														// 
// Modify Date : 2022/03/18														// 
//==============================================================================//

#include <iostream>

#include "Fluoresce.h"

int main()
{
	Fluoresce::Log::Init();

	FR_CLIENT_INFO("Console Application");

	int t = 5;
	FR_CLIENT_TRACE("t = {0}", t);

	std::cin.get();
}
