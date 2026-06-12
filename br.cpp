#include "br.h"

#include <algorithm>
#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <limits>
#include <random>

//  Реализация Validator

bool Validator::is_valid_number(const std::string& number,
                                int required_length) {
    if (static_cast<int>(number.length()) != required_length) {
        return false;
    }

    for (size_t i = 0; i < number.length(); ++i) {
        if (!isdigit(number[i])) {
            return false;
        }
    }

    return true;
}

std::string Validator::get_input_from_random(int required_length) {
    std::string result;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 9);

    for (int i = 0; i < required_length; ++i) {
        result += std::to_string(dis(gen));
    }

    return result;
}

std::string Validator::get_input_from_keyboard(int required_length) {
    std::string input;

    while (true) {
        std::cout << "(или введите 'menu' для возврата в главное меню)\n";
        std::getline(std::cin, input);

        // Проверка на пустую строку
        if (input.empty()) {
            std::cout << "Пустой ввод! Попробуйте снова.\n";
            continue;
        }

        // Проверка на возврат в меню (без учета регистра)
        std::string lower_input = input;
        for (size_t i = 0; i < lower_input.length(); ++i) {
            lower_input[i] = tolower(lower_input[i]);
        }

        if (lower_input == "menu") {
            return "";
        }

        // Удаляем пробелы
        input.erase(std::remove_if(input.begin(), input.end(), ::isspace),
                    input.end());

        if (is_valid_number(input, required_length)) {
            return input;
        }

        std::cout << "Некорректный ввод! Нужна длина " << required_length
                  << " и только цифры. Попробуйте снова.\n";
    }
}

std::string Validator::get_input_from_file(const std::string& filename,
                                           int required_length) {
    std::ifstream file(filename);
    std::string input;

    if (file.is_open()) {
        file >> input;
        file.close();

        if (is_valid_number(input, required_length)) {
            return input;
        }
    }

    return "";
}

// --- Реализация CowsAndBullsGame ---

CowsAndBullsGame::CowsAndBullsGame() : secret_number_(""), length_(4) {}

CowsAndBullsGame::CowsAndBullsGame(const CowsAndBullsGame& other)
        : secret_number_(other.secret_number_), length_(other.length_) {}

CowsAndBullsGame& CowsAndBullsGame::operator=(const CowsAndBullsGame& other) {
    if (this != &other) {
        secret_number_ = other.secret_number_;
        length_ = other.length_;
    }
    return *this;
}

CowsAndBullsGame::~CowsAndBullsGame() {
    // Деструктор не требует специальных действий,
    // так как все ресурсы управляются автоматически через std::string.
}

void CowsAndBullsGame::generate_secret() {
    secret_number_ = Validator::get_input_from_random(length_);
}

void CowsAndBullsGame::count_bulls_and_cows(const std::string& guess,
                                            int* bulls, int* cows) const {
    *bulls = 0;
    *cows = 0;

    // Динамическое выделение памяти для массивов флагов.
    bool* secret_used = new bool[guess.length()]();
    bool* guess_used = new bool[guess.length()]();

    for (size_t i = 0; i < guess.length(); ++i) {
        if (guess[i] == secret_number_[i]) {
            (*bulls)++;
            secret_used[i] = true;
            guess_used[i] = true;
        }
    }

    for (size_t i = 0; i < guess.length(); ++i) {
        if (!guess_used[i]) {
            for (size_t j = 0; j < secret_number_.length(); ++j) {
                if (!secret_used[j] && guess[i] == secret_number_[j]) {
                    (*cows)++;
                    secret_used[j] = true;
                    break;
                }
            }
        }
    }

    delete[] secret_used;
    delete[] guess_used;
}

int CowsAndBullsGame::get_max_number() const {
    int max = 1;
    for (int i = 0; i < length_; ++i) {
        max *= 10;
    }
    return max;
}

std::string* CowsAndBullsGame::create_possible_numbers(int* size) {
    *size = get_max_number();
    std::string* possible_numbers = new std::string[*size];

    for (int i = 0; i < *size; ++i) {
        std::string num = std::to_string(i);
        while (static_cast<int>(num.length()) < length_) {
            num = "0" + num;
        }
        possible_numbers[i] = num;
    }

    return possible_numbers;
}

bool CowsAndBullsGame::is_integer(const std::string& str) {
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

bool CowsAndBullsGame::safe_read_integer(int* value, int min_value,
                                         int max_value) {
    std::string input;
    std::getline(std::cin, input);

    // Проверка на пустую строку.
    if (input.empty()) {
        std::cout << "Пустой ввод! Введите число от " << min_value
                  << " до " << max_value << ": ";
        return false;
    }

    // Проверка на возврат в меню (без учета регистра).
    std::string lower_input = input;
    for (size_t i = 0; i < lower_input.length(); ++i) {
        lower_input[i] = tolower(lower_input[i]);
    }

    if (lower_input == "menu") {
        *value = -1;
        return true;
    }

    // Проверяем, является ли строка целым числом.
    if (!is_integer(input)) {
        std::cout << "Неверный ввод. Введите целое число от " << min_value
                  << " до " << max_value << ": ";
        return false;
    }

    // Пробуем преобразовать в число.
    try {
        *value = std::stoi(input);
        if (*value >= min_value && *value <= max_value) {
            return true;
        } else {
            std::cout << "Неверный ввод. Введите число от " << min_value
                      << " до " << max_value << ": ";
            return false;
        }
    } catch (const std::out_of_range&) {
        std::cout << "Число слишком большое. Введите число от " << min_value
                  << " до " << max_value << ": ";
        return false;
    } catch (const std::invalid_argument&) {
        std::cout << "Неверный ввод. Введите целое число от " << min_value
                  << " до " << max_value << ": ";
        return false;
    }
}

int CowsAndBullsGame::choose_new_input_method() {
    std::cout << "\nВыберите новый способ ввода:\n";
    std::cout << "1 - Клавиатура\n";
    std::cout << "2 - Умный рандом\n";
    std::cout << "3 - Файл\n";
    std::cout << "0 - Вернуться в главное меню\n";
    std::cout << "Ваш выбор: ";

    int choice;
    while (!safe_read_integer(&choice, 0, 3)) {
        // Цикл продолжается, пока не получим корректный ввод.
    }

    return choice;
}

std::string CowsAndBullsGame::get_secret_number_from_user() {
    std::cout << "Метод ввода числа:\n";
    std::cout << "1 - Клавиатура\n";
    std::cout << "2 - Рандом\n";
    std::cout << "3 - Файл\n";
    std::cout << "0 - Вернуться в главное меню\n";
    std::cout << "Ваш выбор: ";

    int choice;
    while (!safe_read_integer(&choice, 0, 3)) {
        // Цикл продолжается, пока не получим корректный ввод.
    }

    if (choice == -1 || choice == 0) {
        return "";
    }

    std::string user_number;

    switch (choice) {
        case 1: {
            std::cout << "Введите " << length_ << "-значное число.\n";
            user_number = Validator::get_input_from_keyboard(length_);
            if (user_number.empty()) {
                return "";
            }
            break;
        }

        case 2:
            user_number = Validator::get_input_from_random(length_);
            std::cout << "Сгенерировано случайное число: " << user_number << "\n";
            break;

        case 3: {
            std::string filename;
            std::cout << "Введите название файла (без расширения)\n";
            std::cout << "(или 'menu' для возврата в меню): ";
            std::getline(std::cin, filename);

            if (filename.empty()) {
                std::cout << "Пустой ввод! Переход на ручной ввод.\n";
                std::cout << "Введите " << length_ << "-значное число.\n";
                user_number = Validator::get_input_from_keyboard(length_);
                if (user_number.empty()) {
                    return "";
                }
                break;
            }

            // Проверка на возврат в меню (без учета регистра).
            std::string lower_filename = filename;
            for (size_t i = 0; i < lower_filename.length(); ++i) {
                lower_filename[i] = tolower(lower_filename[i]);
            }

            if (lower_filename == "menu") {
                return "";
            }

            filename += ".txt";
            user_number = Validator::get_input_from_file(filename, length_);

            if (user_number.empty()) {
                std::cout << "Ошибка чтения файла. Переход на ручной ввод.\n";
                std::cout << "Введите " << length_ << "-значное число.\n";
                user_number = Validator::get_input_from_keyboard(length_);
                if (user_number.empty()) {
                    return "";
                }
            } else {
                std::cout << "Число прочитано из файла: " << user_number << "\n";
            }
            break;
        }
    }

    return user_number;
}

std::string CowsAndBullsGame::get_user_guess_from_file(
        int attempt_number, const std::string& filename) {
    std::string full_filename = filename + ".txt";
    std::string guess = Validator::get_input_from_file(full_filename, length_);

    if (guess.empty()) {
        std::cout << "Ошибка чтения файла. Переход на ручной ввод.\n";
        std::cout << "\nПопытка " << attempt_number << ". Ваш вариант:\n";
        guess = Validator::get_input_from_keyboard(length_);
        if (guess.empty()) {
            return "";
        }
    } else {
        std::cout << "\nПопытка " << attempt_number
                  << ". Ваш вариант: " << guess << " (из файла)\n";
    }

    return guess;
}

void CowsAndBullsGame::filter_possible_numbers(std::string* possible_numbers,
                                               int* size,
                                               const std::string& guess,
                                               int expected_bulls,
                                               int expected_cows) {
    int filtered_count = 0;
    std::string* temp = new std::string[*size];

    // Сохраняем текущее секретное число.
    std::string saved_secret = secret_number_;

    for (int i = 0; i < *size; ++i) {
        // Временно подменяем secret_number_ для подсчета.
        secret_number_ = possible_numbers[i];

        int test_bulls = 0;
        int test_cows = 0;
        count_bulls_and_cows(guess, &test_bulls, &test_cows);

        if (test_bulls == expected_bulls && test_cows == expected_cows) {
            temp[filtered_count++] = possible_numbers[i];
        }
    }

    // Восстанавливаем secret_number_.
    secret_number_ = saved_secret;

    // Копируем отфильтрованные числа обратно.
    for (int i = 0; i < filtered_count; ++i) {
        possible_numbers[i] = temp[i];
    }

    *size = filtered_count;
    delete[] temp;
}

bool CowsAndBullsGame::user_smart_random_mode() {
    generate_secret();
    std::cout << "Компьютер загадал " << length_ << "-значное число.\n";
    std::cout << "Умный рандом начинает поиск...\n\n";

    int size;
    std::string* possible_numbers = create_possible_numbers(&size);

    // Перемешиваем массив для случайного порядка.
    std::random_device rd;
    std::mt19937 gen(rd());
    for (int i = size - 1; i > 0; --i) {
        std::uniform_int_distribution<> dis(0, i);
        int j = dis(gen);
        std::string temp = possible_numbers[i];
        possible_numbers[i] = possible_numbers[j];
        possible_numbers[j] = temp;
    }

    int attempts = 0;

    while (size > 0) {
        attempts++;

        // Случайный выбор из оставшихся возможных чисел.
        std::uniform_int_distribution<> dis(0, size - 1);
        int index = dis(gen);
        std::string guess = possible_numbers[index];

        // Удаляем использованное число.
        for (int i = index; i < size - 1; ++i) {
            possible_numbers[i] = possible_numbers[i + 1];
        }
        size--;

        int bulls = 0;
        int cows = 0;
        count_bulls_and_cows(guess, &bulls, &cows);

        std::cout << "Попытка " << attempts << ". Вариант: " << guess
                  << " -> Быки: " << bulls << ", Коровы: " << cows << "\n";

        if (bulls == length_) {
            std::cout << "\nПобеда! Число угадано за " << attempts << " попыток.\n";
            std::cout << "Загаданное число: " << secret_number_ << "\n";
            delete[] possible_numbers;
            return true;
        }

        // Фильтруем оставшиеся числа на основе полученного результата.
        filter_possible_numbers(possible_numbers, &size, guess, bulls, cows);
    }

    delete[] possible_numbers;
    return true;
}

bool CowsAndBullsGame::user_guessing_mode() {
    generate_secret();
    std::cout << "Компьютер загадал " << length_ << "-значное число.\n";

    std::cout << "Метод ввода попыток:\n";
    std::cout << "1 - Клавиатура\n";
    std::cout << "2 - Умный рандом\n";
    std::cout << "3 - Файл\n";
    std::cout << "0 - Вернуться в главное меню\n";
    std::cout << "Ваш выбор: ";

    int source_choice;
    while (!safe_read_integer(&source_choice, 0, 3)) {
        // Цикл продолжается, пока не получим корректный ввод.
    }

    if (source_choice == -1 || source_choice == 0) {
        return false;
    }

    if (source_choice == 2) {
        return user_smart_random_mode();
    }

    bool game_over = false;

    while (!game_over) {
        std::string filename = "";

        if (source_choice == 3) {
            std::cout << "Введите название файла (без расширения)\n";
            std::cout << "(или 'menu' для возврата в меню): ";
            std::getline(std::cin, filename);

            if (filename.empty()) {
                std::cout << "Пустой ввод! Попробуйте снова.\n";
                continue;
            }

            // Проверка на возврат в меню (без учета регистра).
            std::string lower_filename = filename;
            for (size_t i = 0; i < lower_filename.length(); ++i) {
                lower_filename[i] = tolower(lower_filename[i]);
            }

            if (lower_filename == "menu") {
                return false;
            }
        }

        int bulls = 0;
        int cows = 0;
        int attempts = 0;

        while (bulls != length_) {
            attempts++;

            std::string guess;

            if (source_choice == 1) {
                std::cout << "\nПопытка " << attempts << ". Ваш вариант:\n";
                guess = Validator::get_input_from_keyboard(length_);
                if (guess.empty()) {
                    return false;
                }
            } else if (source_choice == 3) {
                guess = get_user_guess_from_file(attempts, filename);
                if (guess.empty()) {
                    return false;
                }
            }

            count_bulls_and_cows(guess, &bulls, &cows);
            std::cout << "Быки: " << bulls << ", Коровы: " << cows << "\n";

            if (bulls == length_) {
                std::cout << "Победа! Число угадано за " << attempts << " попыток.\n";
                std::cout << "Загаданное число: " << secret_number_ << "\n";
                game_over = true;
                break;
            }

            if (source_choice == 3 && bulls != length_) {
                std::cout << "Файловый ввод завершен. Число не угадано.\n";
                break;
            }
        }

        if (!game_over && source_choice == 3) {
            source_choice = choose_new_input_method();

            if (source_choice == -1 || source_choice == 0) {
                return false;
            }

            if (source_choice == 2) {
                return user_smart_random_mode();
            }
        } else if (game_over || source_choice == 1) {
            game_over = true;
        }
    }

    return true;
}

void CowsAndBullsGame::pc_guessing_strategy(std::string* possible_numbers,
                                            int* size, int attempt_number) {
    if (*size == 0) {
        return;
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, *size - 1);

    int index = dis(gen);
    std::string guess = possible_numbers[index];

    for (int i = index; i < *size - 1; ++i) {
        possible_numbers[i] = possible_numbers[i + 1];
    }
    (*size)--;

    int bulls = 0;
    int cows = 0;
    count_bulls_and_cows(guess, &bulls, &cows);

    std::cout << "ПК предполагает: " << guess << " -> Быки: " << bulls
              << ", Коровы: " << cows << "\n";

    if (bulls == length_) {
        std::cout << "ПК угадал число за " << attempt_number << " попыток!\n";
        *size = 0;
        return;
    }

    filter_possible_numbers(possible_numbers, size, guess, bulls, cows);
}

void CowsAndBullsGame::play_4_digits_mode() {
    length_ = 4;
    user_guessing_mode();
}

void CowsAndBullsGame::play_6_digits_mode() {
    length_ = 6;
    user_guessing_mode();
}

void CowsAndBullsGame::play_pc_guesses_user_number_mode() {
    std::cout << "\nРежим: ПК угадывает число пользователя\n";
    std::cout << "Выберите длину числа:\n";
    std::cout << "1. 4 знака\n";
    std::cout << "2. 6 знаков\n";
    std::cout << "0. Вернуться в главное меню\n";
    std::cout << "Ваш выбор: ";

    int length_choice;
    while (!safe_read_integer(&length_choice, 0, 2)) {
        // Цикл продолжается, пока не получим корректный ввод.
    }

    if (length_choice == -1 || length_choice == 0) {
        return;
    }

    length_ = (length_choice == 1) ? 4 : 6;

    std::string user_number = get_secret_number_from_user();
    if (user_number.empty()) {
        return;
    }

    secret_number_ = user_number;

    std::cout << "\nПК начинает угадывать ваше число...\n\n";

    int size;
    std::string* possible_numbers = create_possible_numbers(&size);

    // Перемешиваем массив для случайного порядка.
    std::random_device rd;
    std::mt19937 gen(rd());
    for (int i = size - 1; i > 0; --i) {
        std::uniform_int_distribution<> dis(0, i);
        int j = dis(gen);
        std::string temp = possible_numbers[i];
        possible_numbers[i] = possible_numbers[j];
        possible_numbers[j] = temp;
    }

    int attempts = 0;
    while (size > 0) {
        attempts++;
        pc_guessing_strategy(possible_numbers, &size, attempts);
    }

    delete[] possible_numbers;
}