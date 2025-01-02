#pragma once

#include <iostream>
#include <vector>
#include <string>
using namespace std;

class ExceptionError : public std::exception {
protected:
    std::string mMsg;
public:
    ExceptionError() {
        mMsg = "Error!!!";
    }

    ExceptionError(const std::string &msg) {
        mMsg = "Error: " + msg;
    }

    const char *what() const noexcept override {
        return mMsg.c_str();
    }
};

/**
 * Функция разделяет входную строку на слова
 * @param text - строка-текст;
 * @return вектор слов;
 */
vector<string> Split_Into_Words(const string &text);

/**
* Функция преобразования строки в набор слов разделённых пробелом, преобразует заглавные буквы в строчные,
* игнорирует все символы кроме букв латинского алфавита
* @input исходная строка
* @return строку слов разделённых пробелами, без знаков припинания, регист low
*/
string Skip_Spec_Symbols(const string &inputString);