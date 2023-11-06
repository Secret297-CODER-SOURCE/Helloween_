#define _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS
#include <iostream>
#include <filesystem>
#include <Windows.h>
#include <shlobj.h>
#include <string>
#include <locale>
#include <codecvt>
using namespace std;

//test comment

void SearchFileInDirectory(const std::string& targetFileName, const std::string& searchDirectory) {
    try {
        for (const auto& entry : filesystem::directory_iterator(searchDirectory)) {
            if (entry.is_regular_file()) {
                std::string filename = entry.path().filename().string();
                if (filename == targetFileName) {
                    // Файл с искомым именем найден
                    std::wcout << L"Файл найден: " << entry.path() << std::endl;

                    // Установить фотографию в качестве обоев
                    SystemParametersInfo(SPI_SETDESKWALLPAPER, 0, (void*)entry.path().c_str(), SPIF_UPDATEINIFILE | SPIF_SENDCHANGE);
                    return; // Если файл найден, завершаем поиск
                }
            }
            else if (entry.is_directory()) {
                // Рекурсивно ищем в подпапках
                SearchFileInDirectory(targetFileName, entry.path().string());
            }
        }
    }
    catch (const exception& exceptions) {
        cout << exceptions.what() << endl;
    }
}

int main()
{
    setlocale(LC_ALL, "rus");
    string targetFileName = "hq720.jpg";

    //// Начнем поиск с корневого каталога системного диска (обычно C:\)
    string searchDirectory = "C:\\Users\\Вадим\\Desktop";

    SearchFileInDirectory(targetFileName, searchDirectory);

}

