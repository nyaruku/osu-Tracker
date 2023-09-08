#include "wrapper.h"
#include <iostream>
#include "config.h"
#define NOMINMAX
#include <windows.h>
#pragma comment(lib, "Shell32.lib")    
#include <ShellAPI.h> // without space also
#include "extension.h"
#include "SaveManager.h"

void init() { // Initialize everything
	char* system_drive{ nullptr };
	size_t count{ 0 };
	_dupenv_s(&system_drive, &count, "SystemDrive");
	driveLetter = system_drive;
	MessageBox(NULL, MultiByteToWideChar(driveLetter), L"Drive Letter", MB_ICONINFORMATION | MB_OK);
	free(system_drive);
	const char* location_path = custom_strcat(2, driveLetter, "\\osu!Tracker\\");
	if (std::filesystem::exists(custom_strcat(2, location_path, "config.txt"))) {
		// file exists, load config
	}
	else {
		// create file and folder
	}
}

void createSaveFile() {
}

void OpenLink(const char* link) {
	ShellExecute(NULL, NULL, MultiByteToWideChar(link), NULL, NULL, SW_SHOWNORMAL);
}

// Function to create gradient text
const char* gradientText(const char* text, const char* color_start, const char* color_end) {
	std::stringstream result;
	Color color1 = HexColorToRGB(color_start);
	Color color2 = HexColorToRGB(color_end);

	int totalSteps = strlen(text);
	for (int i = 0; i < totalSteps; i++) {
		result << "[color=" << RGBColorToHex(getColorForStep(color1, color2, totalSteps, i)) << "]" << text[i] << "[/color]";
	}

	std::string resultStr = result.str();
	char* cString = new char[resultStr.size() + 1];
	strcpy(cString, resultStr.c_str());

	return cString;
}

const char* solidText(const char* text, const char* color) {
	return custom_strcat(5, "[color=", color, "]", text, "[/color]");
}