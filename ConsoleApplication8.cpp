
// Заглушки для игнорирования предупреждений об устаревании в C++17
#define _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS

// Подключение библиотек и файлов
#include <iostream>
#include <filesystem>
#include <string>
#include <locale>
#include <cstdio>
#include <cstring>
#include <winsock2.h>
#include <codecvt>
#include <thread>
#pragma comment(lib, "WS2_32.lib")

//Издавна ещё в каменном веке в каждой книжке про С++ на заголовке пишется что такое сяут цин и юзинг неймспейс
using namespace std;

// Объявление класса BackGround
class BackGround {
    SOCKET server;
public:
    // Конструктор класса BackGround
    BackGround() {
        SetCurrentDirectory(string_to_LPCWSTR("C:\\"));
        SocketSetting();
    }
    // 
    bool isFolder(string name);
    // 
    LPCWSTR string_to_LPCWSTR(string str);
    //
    void SearchFileInDirectory(const std::string& searchDirectory);
    // 
    string showDirectoryContents();
    // 
    string open_folder(string& path, string name);
    //
    void recieve();
    // 
    void SocketSetting();
};

//Сокет глобальный супостатский чертями пайтонскими создан
SOCKET server;

// Бул проверяет считается ли директория неймом??
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

// Наш прекрасный стринги, которые были под запретом в начале переделываются или переходят или переплывают или превращаются в LPCWSTR
LPCWSTR BackGround::string_to_LPCWSTR(string str) {
    int bufferSize = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, nullptr, 0);
    wchar_t* buffer = new wchar_t[bufferSize];
    MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, buffer, bufferSize);
    return buffer; //возвращает буфер
    delete[] buffer;  //удаляет буфер
}

// Поиск пнг картинки по директориям и ставит на рабочий стол
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

// Показує Что вообще в той директории
string BackGround::showDirectoryContents() {
    filesystem::path currentDir = filesystem::current_path();
    string temp = "";
    for (const auto& entry : filesystem::directory_iterator(currentDir))
    {
        temp += entry.path().filename().string() + "\n";
    }
    return temp;
}

// Открытие директории
string BackGround::open_folder(string& path, string name) {
    if (isFolder(name)) {
        path += ("\\" + name);
        int result = SetCurrentDirectory(string_to_LPCWSTR(path));
        send(server, showDirectoryContents().c_str(), showDirectoryContents().size(), NULL);
        return path;
    }
    return path;
}

// Прием данных по сокету ч'ртову
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

// Настройки сокета супостатского
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
    //Чому не державною
    setlocale(LC_ALL, "rus");

    // Запускаем первую белку в космос
    BackGround a = BackGround();


}