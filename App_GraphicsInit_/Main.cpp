//-------------------------------------------------------------------------------------------------
//
// Copyright (c) Ryan Alasandro
//
// Windows Application: App_GraphicsInit
//
// File: Main.cpp
//
//-------------------------------------------------------------------------------------------------

#include "Main.h"
#include "Application/CApplication.h"
#include <Windows.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdLine) {
	App::CApplication app;

	app.Create();
	const int exitCode = app.Run();
	app.Destroy();

	return exitCode;
}
