#include "dynamic_data_library.h"
#include <iostream>
#include <windows.h>

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    int choice;
    do {
        showMainMenu();
        std::cin >> choice;
        switch (choice) {
        case 1:
            showMenu<int>();
            break;
        case 2:
            showMenu<float>();
            break;
        case 3:
            showMenu<char>();
            break;
        case 4:
            showMenu<std::string>();
            break;
        case 0:
            std::cout << "Вихід\n";
            break;
        default:
            std::cout << "Неправильний вибір\n";
            break;
        }
    } while (choice != 0);

    return 0;
}
