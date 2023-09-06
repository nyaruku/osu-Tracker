#include "wrapper.h"
#include <iostream>
#include "config.h"
#include <windows.h>
#include "extension.h"
#include "SaveManager.h"

void init() { // Initialize everything
	char* system_drive{ nullptr };
	size_t count{ 0 };
	_dupenv_s(&system_drive, &count, "SystemDrive");
	driveLetter = system_drive;
	MessageBox(NULL, MultiByteToWideChar(driveLetter), L"Drive Letter", MB_ICONINFORMATION | MB_OK);
	free(system_drive);
	const char* location_path = custom_strcat(2, driveLetter, "\\osu!Tracker");

}

void createSaveFile() {

}