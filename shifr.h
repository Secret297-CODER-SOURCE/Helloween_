#pragma once
#include <Windows.h>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <lmcons.h> // Äëÿ UNLEN
#pragma warning(disable : 4996)

using namespace std;

class shifr
{
public:
	void cryptFile(const string filePath);
	void decryptFile(const string filePath);
	void encryptFilesInDirectory(const string& directoryPath);
	void decryptFilesInDirectory(const std::string& directoryPath);
	string getUserName();
};