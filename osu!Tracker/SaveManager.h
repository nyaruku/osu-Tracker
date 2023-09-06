// A REWRITE OF MY OWN SAVE MANAGER FROM C#
// LINK: https://github.com/nyaruku/SaveManager
#pragma once
#include <iostream>
#include "extension.h"
#include <fstream>
#include <cstring> // For strstr
#include <sstream> // For std::istringstream
#include <filesystem>
class SaveManager
{
public:

	const char* SaveData;

	void AddKey(const char* keyname, int keyvalue)
	{
		SaveData = custom_strcat(5, SaveData, keyname, "=", keyvalue, "\n");
	}

	void AddKey(const char* keyname, float keyvalue)
	{
		SaveData = custom_strcat(5, SaveData, keyname, "=", keyvalue, "\n");
	}

	void AddKey(const char* keyname, double keyvalue)
	{
		SaveData = custom_strcat(5, SaveData, keyname, "=", keyvalue, "\n");
	}

	void AddKey(const char* keyname, bool keyvalue)
	{
		SaveData = custom_strcat(5, SaveData, keyname, "=", keyvalue, "\n");
	}

	void AddKey(const char* keyname, const char* keyvalue)
	{
		SaveData = custom_strcat(5, SaveData, keyname, "=", keyvalue, "\n");
	}

	const char* GetKey(const char* keyname)
	{
		// Convert the inputString to a stream
		std::istringstream inputStream(SaveData);

		std::string line;
		while (std::getline(inputStream, line)) {
			// Check if the line contains the searchString
			if (strstr(line.c_str(), custom_strcat(2, keyname, "=")) != nullptr) {
				if (lineStartsWith(line.c_str(), "//") || lineStartsWith(line.c_str(), " ")) {
					// SKIP COMMENTS
				}
				else {
					return removeSubstring(line.c_str(), custom_strcat(2, keyname, "="));
				}
			}
		}
		return "";
	}

	bool GetKey_bool(const char* keyname)
	{
		// Convert the inputString to a stream
		std::istringstream inputStream(SaveData);

		std::string line;
		while (std::getline(inputStream, line)) {
			// Check if the line contains the searchString
			if (strstr(line.c_str(), custom_strcat(2, keyname, "=")) != nullptr) {
				if (lineStartsWith(line.c_str(), "//") || lineStartsWith(line.c_str(), " ")) {
					// SKIP COMMENTS
				}
				else {
					if (removeSubstring(line.c_str(), custom_strcat(2, keyname, "=")) == "1") {
						return true;
					}
					else if (removeSubstring(line.c_str(), custom_strcat(2, keyname, "=")) == "0") {
						return false;
					}
				}
			}
		}
		return false;
	}

	int GetKey_int(const char* keyname)
	{
		// Convert the inputString to a stream
		std::istringstream inputStream(SaveData);

		std::string line;
		while (std::getline(inputStream, line)) {
			// Check if the line contains the searchString
			if (strstr(line.c_str(), custom_strcat(2, keyname, "=")) != nullptr) {
				if (lineStartsWith(line.c_str(), "//") || lineStartsWith(line.c_str(), " ")) {
					// SKIP COMMENTS
				}
				else {
					return std::stoi(removeSubstring(line.c_str(), custom_strcat(2, keyname, "=")));
				}
			}
		}
		return 0;
	}

	double GetKey_double(const char* keyname)
	{
		// Convert the inputString to a stream
		std::istringstream inputStream(SaveData);

		std::string line;
		while (std::getline(inputStream, line)) {
			// Check if the line contains the searchString
			if (strstr(line.c_str(), custom_strcat(2, keyname, "=")) != nullptr) {
				if (lineStartsWith(line.c_str(), "//") || lineStartsWith(line.c_str(), " ")) {
					// SKIP COMMENTS
				}
				else {
					return std::stod(removeSubstring(line.c_str(), custom_strcat(2, keyname, "=")));
				}
			}
		}
		return 0;
	}

	float GetKey_float(const char* keyname)
	{
		// Convert the inputString to a stream
		std::istringstream inputStream(SaveData);

		std::string line;
		while (std::getline(inputStream, line)) {
			// Check if the line contains the searchString
			if (strstr(line.c_str(), custom_strcat(2, keyname, "=")) != nullptr) {
				if (lineStartsWith(line.c_str(), "//") || lineStartsWith(line.c_str(), " ")) {
					// SKIP COMMENTS
				}
				else {
					return std::stof(removeSubstring(line.c_str(), custom_strcat(2, keyname, "=")));
				}
			}
		}
		return 0;
	}

	void Clear_Data() // This will clear the temporary data.
	{
		SaveData = "";
	}

	void Read_Data(const char* path)
	{
		SaveData = "";
		std::ifstream file(path);
		if (file.is_open()) {
			std::string line;
			while (std::getline(file, line)) {
				SaveData = custom_strcat(3, SaveData, line, "\n");
			}
			file.close();
		}
	}

	bool Exist_Data(const char* path)
	{
		return std::filesystem::exists(path);
	}

	void Save_Data(const char* path)
	{
		std::ofstream out(path);
		out << SaveData;
		out.close();
	}

	void Delete_Data(const char* path)
	{
		try {
			std::filesystem::remove(path);
		}
		catch (const std::filesystem::filesystem_error& err) {
			//std::cout << "filesystem error: " << err.what() << '\n';
		}
	}
};
