#pragma once
// MOST OF THIS SHIT IS MADE WITH HELP OF CHAT GPT BRUH
#include <iostream>
#include <cstdarg> // for va_list and va_start
#include <Windows.h>

// Function to calculate the length of a null-terminated char*
size_t custom_strlen(const char* str) {
	size_t length = 0;
	while (str[length] != '\0') {
		length++;
	}
	return length;
}

// Function to concatenate multiple null-terminated char* strings
char* custom_strcat(size_t numStrings, ...) {
	// Calculate the total length of the concatenated string
	size_t totalLen = 0;
	va_list args;
	va_start(args, numStrings);
	for (size_t i = 0; i < numStrings; i++) {
		const char* currentString = va_arg(args, const char*);
		totalLen += custom_strlen(currentString);
	}
	va_end(args);

	// Allocate memory for the result string
	char* result = new char[totalLen + 1]; // +1 for the null terminator

	// Copy the contents of each string to the result
	size_t currentIndex = 0;
	va_start(args, numStrings);
	for (size_t i = 0; i < numStrings; i++) {
		const char* currentString = va_arg(args, const char*);
		size_t currentLen = custom_strlen(currentString);
		for (size_t j = 0; j < currentLen; j++) {
			result[currentIndex++] = currentString[j];
		}
	}
	va_end(args);

	// Null-terminate the result string
	result[totalLen] = '\0';

	return result;
}
// Function to remove a substring from a string and return it as const char*
const char* removeSubstring(const char* inputString, const char* substringToRemove) {
	const char* startPos = strstr(inputString, substringToRemove);

	if (startPos != nullptr) {
		size_t substringLength = std::strlen(substringToRemove);
		size_t inputStringLength = std::strlen(inputString);

		// Calculate the length of the modified string
		size_t modifiedStringLength = inputStringLength - substringLength;

		// Create a new char* buffer to hold the modified string
		char* modifiedString = new char[modifiedStringLength + 1];

		// Copy the part before the substring
		strncpy(modifiedString, inputString, startPos - inputString);
		modifiedString[startPos - inputString] = '\0';

		// Copy the part after the substring
		strcat(modifiedString, startPos + substringLength);

		return modifiedString;
	}

	// If the substring is not found, return the original inputString
	return inputString;
}
// Function to check if a line starts with a specific substring
bool lineStartsWith(const char* line, const char* prefix) {
	return strncmp(line, prefix, strlen(prefix)) == 0;
}

wchar_t* MultiByteToWideChar(const char* string) {
	// Calculate the required buffer size for the wide string
	int bufferSize = MultiByteToWideChar(CP_UTF8, 0, string, -1, NULL, 0);

	// Allocate a buffer for the wide string
	wchar_t* wideString = new wchar_t[bufferSize];

	// Convert the UTF-8 string to UTF-16 (wide string)
	MultiByteToWideChar(CP_UTF8, 0, string, -1, wideString, bufferSize);

	return wideString;
}

