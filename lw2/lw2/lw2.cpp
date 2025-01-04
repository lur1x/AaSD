/*25. Программа  на ПАСКАЛЕ включает такие сочетания ключевых
слов, как  REPEAT..UNTIL, RECORD..END, CASE..END и BEGIN..END.
Некоторые части программы могут быть  закомментированы, другие
части текста могут представлять собой  константы в апострофах.
Требуется проверить правильность вложенности этих  конструкций
с  учетом  допустимости  взаимных  вложений.  В случае  ошибок
указать номер первой некорректной строки (11).
Кириллов Дмитрий ПС-22
VS 22
*/

#include <iostream>
#include <fstream>
#include <windows.h>

struct St {
    std::string key;
    St *next;
};

void push(St*& p, std::string elem) {
    St* t = new St;
    t->key = elem;
    t->next = p; // Устанавливаем связь с предыдущим элементом
    p = t;       // Обновляем вершину стека
};


void pop(St *&p) {
    St* t = p;
    p = p->next;
    delete t;
};

void vivod(St* p)            // вывод содержимого стека на экран
{
    St* t = p;
    while (t)
    {
        std::cout << t->key << std::endl;
        t = t->next;
    }
    std::cout << std::endl;;
};

void clear(St* p)            // очистка стека
{
    while (p)
    {
        St* t = p;
        p = p->next;
        delete t;
    }
}


char changeCase(char ch) {
    const std::string engUpLetter = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    switch (ch) {
    case 'a': ch = 'A'; break;
    case 'b': ch = 'B'; break;
    case 'c': ch = 'C'; break;
    case 'd': ch = 'D'; break;
    case 'e': ch = 'E'; break;
    case 'f': ch = 'F'; break;
    case 'g': ch = 'G'; break;
    case 'h': ch = 'H'; break;
    case 'i': ch = 'I'; break;
    case 'j': ch = 'J'; break;
    case 'k': ch = 'K'; break;
    case 'l': ch = 'L'; break;
    case 'm': ch = 'M'; break;
    case 'n': ch = 'N'; break;
    case 'o': ch = 'O'; break;
    case 'p': ch = 'P'; break;
    case 'q': ch = 'Q'; break;
    case 'r': ch = 'R'; break;
    case 's': ch = 'S'; break;
    case 't': ch = 'T'; break;
    case 'u': ch = 'U'; break;
    case 'v': ch = 'V'; break;
    case 'w': ch = 'W'; break;
    case 'x': ch = 'X'; break;
    case 'y': ch = 'Y'; break;
    case 'z': ch = 'Z'; break;
    }
    return ch;
};

bool isEngLetter(char ch) {

    const std::string engUpLetter = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    const std::string engLowLetter = "abcdefghijklmnopqrstuvwxyz";
    const std::string engLetter = engLowLetter + engUpLetter;

    for (int i = 0; i < engLetter.length(); i++) {
        if (ch == engLetter[i]) {
            return true;
        }
    }

    return false;

};

bool checkNesting(St*& top, const std::string& word, int lineNum, bool& hasBegin, bool& insideRecord) {
    if (insideRecord && word != "END") {
        std::cerr << "Ошибка: " << word << " не могут быть внутри 'RECORD' на строке " << lineNum << std::endl;
        return false;
    }
    if (word == "BEGIN") {
        hasBegin = true;
        push(top, word); // Добавляем в стек
    }
    
    if (word == "REPEAT" || word == "CASE") {
        if (!top) {
            std::cerr << "Ошибка: " << word << " могут быть только внутри 'BEGIN' на строке " << lineNum << std::endl;
            return false;
        }
        else {
            push(top, word); // Добавляем в стек
        }
    }
    else if (word == "RECORD") {
        // Проверка на вложенность RECORD
        insideRecord = true;
        if (hasBegin) {
            std::cerr << "Ошибка: 'RECORD' не может быть внутри другой конструкции на строке " << lineNum << std::endl;
            return false; // Выход при ошибке
        }
        push(top, word); // Если все в порядке, добавляем RECORD в стек
    }
    else if (word == "UNTIL" || word == "END") {
        // Если стек пуст, возникает ошибка
        if (!top) {
            std::cerr << "Ошибка: Не соответствующая " << word << " на строке " << lineNum << std::endl;
            return false; // Выход при ошибке
        }

        // Проверяем, соответствует ли закрывающая конструкция последней открывающей
        if (word == "UNTIL" && top->key != "REPEAT") {
            std::cerr << "Ошибка: Неправильное соответствие " << word << " для " << top->key << " на строке " << lineNum << std::endl;
            return false;
        }
        else if ((word == "UNTIL" && top->key == "REPEAT")) {
            pop(top);
            return true;
        }
        if (word == "END" && top->key == "RECORD") {
            insideRecord = false;
            pop(top);
            return true;
        }
        if (word == "END") {
            pop(top); // Убираем элемент из стека
            return true;
        }
    }
    return true; // Если ничего не произошло, возвращаем true
}


void checkFileOfNesting(std::ifstream& fInput) {
    char ch;
    std::string word;
    St* top = nullptr;
    int lineNum = 1;
    bool hasBegin = false; // Флаг для отслеживания наличия BEGIN
    bool insideRecord = false;
    while (fInput.get(ch)) {
        if (isEngLetter(ch)) {
            ch = changeCase(ch);
            word += ch;
        }
       
        else if (ch == '{') {
            while (ch != '}') {
                fInput.get(ch);
            }
        }
        else if (ch == '\'') {
            fInput.get(ch);
            while (ch != '\'') {
                fInput.get(ch);
            }
        }
        else if (ch == '"') {
            fInput.get(ch);
            while (ch != '"') {
                fInput.get(ch);
            }
        }
        else if (ch == '/') {
            while (ch != '\n') {
                fInput.get(ch);
            }
        }
        else {
            if (!word.empty()) { // Проверка, что слово не пустое
                if (word == "REPEAT" || word == "UNTIL" || word == "BEGIN" || word == "CASE" || word == "RECORD" || word == "END") {
                    if (!checkNesting(top, word, lineNum, hasBegin, insideRecord)) {
                        clear(top);  // Сначала очистка стека
                        top = 0;     // функция clear не возвращает top!
                        return;
                    }
                }
                word.clear();
            }
        }
        if (ch == '\n') {
            lineNum += 1;
        }
    }

    // Проверяем последнее слово, если оно не пустое
    if (!word.empty()) {
        if (!checkNesting(top, word, lineNum, hasBegin, insideRecord)) {
            clear(top);  
            top = 0; 
        }
    }
    // Проверка, завершены ли все конструкции
    if (top) {
        std::cerr << "Ошибка: Остались незакрытые конструкции в конце файла." << std::endl;
        while (top) { // Освобождение памяти
            std::cerr << "Незакрытая конструкция: " << top->key << std::endl;
            pop(top);
        }
    }
    else {
        std::cout << "Все конструкции успешно закрыты." << std::endl;
    }

    /*;
    vivod(top); */
};


int main() {
    setlocale(LC_CTYPE, "Russian");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    std::ifstream fInput;
    std::string pathIn;

    while (true) { 
        std::cout << "Введите имя файла, либо \"EXIT\" для выхода из программы" << std::endl;
        std::cin >> pathIn;

        if (pathIn == "EXIT") {
            break;
        }
        fInput.open(pathIn);
        if (!fInput.is_open()) {
            std::cerr << "Не удалось открыть файл, проверьте его имя" << std::endl;
        }
        else {
            checkFileOfNesting(fInput);
        }
        fInput.close();

    }

    return 0;
}
