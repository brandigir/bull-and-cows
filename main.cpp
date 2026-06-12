#include "br.h"

#include <iostream>
#include <string>

#ifdef _WIN32
#include <windows.h>
#endif

void show_rules() {
    std::cout << "\n ПРАВИЛА ИГРЫ 'БЫКИ И КОРОВЫ'\n\n";
    std::cout << "Компьютер загадывает число.\n";
    std::cout << "Ваша задача - угадать это число.\n\n";
    std::cout << "ЦИФРЫ В ЧИСЛЕ МОГУТ ПОВТОРЯТЬСЯ!!!!\n\n";
    std::cout << "После каждой попытки компьютер сообщает:\n";
    std::cout << "- Быки: количество цифр, которые стоят на своих местах\n";
    std::cout << "- Коровы: количество цифр, которые есть в числе, "
              << "но стоят на других позициях\n\n";
    std::cout << "Режимы игры:\n";
    std::cout << "1. Пользователь угадывает число (4 или 6 знаков)\n";
    std::cout << "   - Клавиатура: вы вводите числа вручную\n";
    std::cout << "   - Умный рандом: компьютер автоматически угадывает число\n";
    std::cout << "   - Файл: попытки читаются из текстового файла\n\n";
    std::cout << "2. ПК угадывает число пользователя\n";
    std::cout << "   - Вы загадываете число (вводите с клавиатуры, "
              << "генерируете или из файла)\n";
    std::cout << "   - Компьютер пытается угадать ваше число\n\n";
    std::cout << "Для файлового ввода создайте файл .txt в папке с программой.\n";
    std::cout << "ЗАПРЕШЕНО НАЗЫВАТЬ ФАЙЛ menu.txt ОН НЕ БУДЕТ РАБОТАТЬ!\n\n";
    std::cout << "Файл должен содержать одно число нужной длины.\n\n";
    std::cout << "В любом месте программы можно ввести 'menu' "
              << "для возврата в главное меню.\n\n";

    std::cout << "Нажмите Enter для продолжения...";
    std::string dummy;
    std::getline(std::cin, dummy);
}

// Проверяет, является ли строка целым числом
bool is_integer(const std::string& str) {
    if (str.empty()) {
        return false;
    }

    size_t start = 0;

    // Проверяем на отрицательное число
    if (str[0] == '-') {
        if (str.length() == 1) {
            return false;  // Только минус без цифр
        }
        start = 1;
    }

    // Проверяем, что все остальные символы - цифры
    for (size_t i = start; i < str.length(); ++i) {
        if (!isdigit(str[i])) {
            return false;
        }
    }

    // Проверяем на переполнение
    try {
        std::stoi(str);
        return true;
    } catch (const std::out_of_range&) {
        return false;
    } catch (const std::invalid_argument&) {
        return false;
    }
}

int main() {
#ifdef _WIN32
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
#endif

    CowsAndBullsGame game;
    int mode = -1;

    while (mode != 0) {
        std::cout << "\nИГРА КОРОВЫ И БЫКИ\n"
                  << "1. Правила игры\n"
                  << "2. Режим: 4 знака (пользователь угадывает)\n"
                  << "3. Режим: 6 знаков (пользователь угадывает)\n"
                  << "4. Режим: ПК угадывает число пользователя\n"
                  << "0. Выход\n"
                  << "Выберите пункт меню: ";

        std::string input;
        std::getline(std::cin, input);

        // Проверка на пустую строку.
        if (input.empty()) {
            std::cout << "Пустой ввод! Введите число от 0 до 4.\n";
            continue;
        }

        // Проверка на команду menu (без учета регистра).
        std::string lower_input = input;
        for (size_t i = 0; i < lower_input.length(); ++i) {
            lower_input[i] = tolower(lower_input[i]);
        }

        if (lower_input == "menu") {
            std::cout << "Вы уже в главном меню.\n";
            continue;
        }

        // Проверяем, является ли ввод целым числом.
        if (!is_integer(input)) {
            std::cout << "Ошибка: Введите целое число от 0 до 4.\n";
            continue;
        }

        // Преобразуем в число.
        try {
            mode = std::stoi(input);

            if (mode < 0 || mode > 4) {
                std::cout << "Ошибка: Введите число от 0 до 4.\n";
                continue;
            }
        } catch (const std::out_of_range&) {
            std::cout << "Ошибка: Число слишком большое. Введите число от 0 до 4.\n";
            continue;
        } catch (const std::invalid_argument&) {
            std::cout << "Ошибка: Введите целое число от 0 до 4.\n";
            continue;
        }

        switch (mode) {
            case 1:
                show_rules();
                break;
            case 2:
                game.play_4_digits_mode();
                break;
            case 3:
                game.play_6_digits_mode();
                break;
            case 4:
                game.play_pc_guesses_user_number_mode();
                break;
            case 0:
                std::cout << "Завершение программы.\n";
                break;
            default:
                std::cout << "Неверный пункт меню. Попробуйте еще раз.\n";
        }
    }

    return 0;
}