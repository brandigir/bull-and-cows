#ifndef BR_H_
#define BR_H_

#include <string>

// Класс для статических проверок ввода.
class Validator {
public:
    // Проверяет, состоит ли строка только из цифр и имеет ли она нужную длину.
    static bool is_valid_number(const std::string& number, int required_length);

    // Получает строку заданной длины из стандартного потока ввода.
    // Возвращает пустую строку, если пользователь хочет вернуться в меню.
    static std::string get_input_from_keyboard(int required_length);

    // Генерирует строку заданной длины из случайных цифр.
    static std::string get_input_from_random(int required_length);

    // Читает строку заданной длины из файла.
    static std::string get_input_from_file(const std::string& filename,
                                           int required_length);

private:
    // Запрещаем создание экземпляров класса (только статические методы).
    Validator() = delete;
    ~Validator() = delete;
    Validator(const Validator&) = delete;
    Validator& operator=(const Validator&) = delete;
};

// Основной класс игры "Коровы и Быки".
class CowsAndBullsGame {
public:
    CowsAndBullsGame();

    // Конструктор копирования.
    CowsAndBullsGame(const CowsAndBullsGame& other);

    // Оператор присваивания.
    CowsAndBullsGame& operator=(const CowsAndBullsGame& other);

    // Деструктор.
    ~CowsAndBullsGame();

    // Запускает игру в режиме 4 цифр (пользователь угадывает).
    void play_4_digits_mode();

    // Запускает игру в режиме 6 цифр (пользователь угадывает).
    void play_6_digits_mode();

    // Запускает режим "ПК угадывает число пользователя".
    void play_pc_guesses_user_number_mode();

private:
    std::string secret_number_;  // Загаданное число.
    int length_;                 // Текущая длина числа.

    // Генерирует секретное число.
    void generate_secret();

    // Подсчитывает количество быков и коров для заданной попытки.
    void count_bulls_and_cows(const std::string& guess, int* bulls,
                              int* cows) const;

    // Режим, где пользователь угадывает число (с клавиатуры, умный рандом или файл).
    // Возвращает true, если игра завершена, false - если пользователь хочет в меню.
    bool user_guessing_mode();

    // Режим "умного" рандома для пользователя.
    // Возвращает true, если игра завершена, false - если пользователь хочет в меню.
    bool user_smart_random_mode();

    // Обрабатывает одну попытку компьютера в режиме VsPc.
    void pc_guessing_strategy(std::string* possible_numbers, int* size,
                              int attempt_number);

    // Фильтрует массив возможных чисел, оставляя только совместимые.
    void filter_possible_numbers(std::string* possible_numbers, int* size,
                                 const std::string& guess, int expected_bulls,
                                 int expected_cows);

    // Создает массив всех возможных чисел заданной длины.
    std::string* create_possible_numbers(int* size);

    // Генерирует максимальное число для заданной длины.
    int get_max_number() const;

    // Получает секретное число от пользователя разными способами.
    // Возвращает пустую строку, если пользователь хочет вернуться в меню.
    std::string get_secret_number_from_user();

    // Получает попытку пользователя из файла.
    // Возвращает пустую строку, если пользователь хочет вернуться в меню.
    std::string get_user_guess_from_file(int attempt_number,
                                         const std::string& filename);

    // Предлагает пользователю выбрать новый способ ввода после неудачи.
    // Возвращает 0, если пользователь хочет вернуться в меню.
    int choose_new_input_method();

    // Проверяет, является ли строка целым числом.
    static bool is_integer(const std::string& str);

    // Безопасное чтение целого числа с проверкой на menu и пустой ввод.
    // Возвращает true, если ввод успешен, false - если требуется повторный ввод.
    // value устанавливается в -1 при вводе "menu".
    bool safe_read_integer(int* value, int min_value, int max_value);
};

#endif  // BR_H_