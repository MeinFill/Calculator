#include "httplib.h"
#include "Calculator.h"
#include <iostream>
#include <cstdlib>
#include <string>
#include <locale>
#include <cmath>
#include <vector>

using namespace std;
using namespace httplib;
string errors = "";
int counter = 1; // счетчик запросов
int main() {
    setlocale(LC_ALL, "Russian");
    Server svr;
    svr.Post("/", [](const Request& req, Response& res) { // на сайте будет выведена строка с ответом. В консоли будет выведен результат запроса (сделан или ошибка), а также время выполнения запроса
        auto begin = chrono::high_resolution_clock::now(); // время начала
        bool check = true;
        int check_tocki = 1; //нужны для проверок
        int ch_n = 1;
        string output = req.get_param_value("a"); // получение строки из input
        if (output.length() > 0) { // проверка на наличие ввода !! - в html файле стоит ограничение на длину ввода, поэтому проверки на длину нет
            for (int i = 0; i < output.length(); i++) { // проверка на отсутствие буковок
                if (output[i] != '^' && output[i] != '*' && output[i] != '/' && output[i] != '+' && output[i] != '-' && output[i] != ' ' && output[i] != '0' && output[i] != '1' && output[i] != '2' && output[i] != '3' && output[i] != '4' && output[i] != '5' && output[i] != '6' && output[i] != '7' && output[i] != '8' && output[i] != '9' && output[i] != '.' && output[i] != ',') {
                    check = false;
                }
            }
            errors = check_errors_player(output); // метод проверяет на наличие повторяющихся действий. НИЧЕГО НЕ МЕНЯЕТ
            errors = check_tochek(output, check_tocki, errors); // метод прверяет на несколько точек в числе. НИЧЕГО НЕ МЕНЯЕТ
            if (check && errors.length() == 0) { // если все гуд, можно считать
                double out = calculate(output, check_tocki, ch_n); // собственно, сам счет. Если в нем будет деление на 0, это передастся ch_n и выведется ошибка
                if (ch_n != 0 && out < 999999999999999 && out > -999999999999999) { // проверка на длинные числа
                    string output_web = to_string(out); //здесь число переводится в строку, чтобы его можно было отформатировать (в следующей строке) и красиво вывести пользователю
                    privedenie(output_web);
                    res.set_content("Ответ: " + output_web, "enctype ");
                    cout << "\n" << "Запрос №" << counter << " исполнен успешно\n\n";
                }
                else {// если деление на 0 или число больше 15 разрядов
                    if (ch_n == 0) {
                        res.set_content("Деление на 0(", "enctype ");
                    }
                    else {
                        res.set_content("Слишком длинное число", "enctype ");
                    }
                }
            }
            else { // если есть буквы ИЛИ ошибки в вводе
                if (errors.length() > 0) { // если ошибки в вводе, выводится ошибка
                    res.set_content(errors, "enctype ");
                    cout << "\n" << "Запрос №" << counter << ": " << errors << "\n\n";
                }
                // если была встречена хотя бы одна буква check это увидит и выведет ошибку 
                else {
                    res.set_content("А можно вводить числа?", "enctype ");
                    cout << "\n" << "Запрос №" << counter << ": ввод буковок \n\n";
                }
            }
        }
        else {
            // если все пусто или же у переменной нет числа, где запрос???
            res.set_content("Где запрос?", "enctype");
            cout << "\n" << "Где запрос?" << "\n";
        }
        counter++; // счет запросов
        auto end = chrono::high_resolution_clock::now(); // время конца
        cout << "Время выполнения запроса: " << chrono::duration_cast<chrono::nanoseconds>(end - begin).count() << "ns (Возможно)\n _____________________________\n"; //здесь вывод времени
        });
    // Это просто если перейти по ссылке, а не по форме, правда, там будет только приветствие 
    svr.Get("/", [](const Request& req, Response& res) {
        res.set_content("Hello!", "text/plain");
        });
    std::cout << "Сервер находится по ссылке http://localhost:5000\n";
    svr.listen("0.0.0.0", 5000);
    return 0;
}