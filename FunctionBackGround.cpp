#define _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS
#include <iostream>
#include <filesystem>
#include <Windows.h>
#include <shlobj.h>
#include <string>
#include <locale>
#include <codecvt>
using namespace std;

void SearchFileInDirectory(const std::string& targetFileName, const std::string& searchDirectory) {
    try {
        for (const auto& entry : filesystem::directory_iterator(searchDirectory)) {
            if (entry.is_regular_file()) {
                std::string filename = entry.path().filename().string();
                if (filename == "qwerty.png") {
                    cout << "qwer";
                }

                string format;
                format.resize(4);
                int j = 0;
                int i = filename.size() - 4;
                for (; j < 4; i++, j++) {
                    format[j] = filename[i];
                }

                if (format == ".png") {
                    // Файл с искомым именем найден
                    std::wcout << L"Файл найден: " << entry.path() << std::endl;

                    // Установить фотографию в качестве обоев
                    SystemParametersInfo(SPI_SETDESKWALLPAPER, 0, (void*)entry.path().c_str(), SPIF_UPDATEINIFILE | SPIF_SENDCHANGE);
                    system("pause");
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
    string targetFileName = "qwerty.png";

    //// Начнем поиск с корневого каталога системного диска (обычно C:\)
    string searchDirectory = "C:\\Users\\Вадим\\Desktop";

    SearchFileInDirectory(targetFileName, searchDirectory);

}

