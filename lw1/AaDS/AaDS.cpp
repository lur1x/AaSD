#include <iostream>
#include <fstream>
/*
16. Разработать программу перекодировки текстового файла из
кодировки 1251 в 866 и обратно (7).
Кириллов Дмитрий ПС-22
*/

void convertFrom1251To866(std::ifstream& fInput, std::ofstream& fOutput) {
    char ch, newCh;
    while (fInput.get(ch)) {
        if ((ch >= 'А' && ch <= 'Я') || (ch >= 'а' && ch <= 'п')) {
            newCh = int(ch) - 64; 
            fOutput << newCh;
        }
        else if (ch >= 'р' && ch <= 'я') {
            newCh = int(ch) - 16; 
            fOutput << newCh;
        }
        else if (ch == 'Ё') {
            newCh = int(ch) + 72;
            fOutput << newCh;
        }
        else if (ch == 'ё') {
            newCh = int(ch) + 57;
            fOutput << newCh;
        }
        else {
            fOutput << ch;
        }
    }
};

void convertFrom866To1251(std::ifstream& fInput, std::ofstream& fOutput) {
    char ch, newCh;
    
    while (fInput.get(ch)) {
        if (int(ch) >= -128 && -81 >= int(ch)) {
            newCh = int(ch) + 64;
            fOutput << newCh;
        }
        else if (int(ch) >= -32 && int(ch) <= -17) {
            newCh = int(ch) + 16;
            fOutput << newCh;
        }
        else if (int(ch) == -16) {
            newCh = int(ch) - 72;
            fOutput << newCh;
        }
        else if (int(ch) == -15) {
            newCh = int(ch) - 57;
            fOutput << newCh;
        }
        else {
            fOutput << ch;
        } 
    }
};

int main() {

    setlocale(LC_ALL, "ru");

    std::ifstream fInput;
    std::ofstream fOutput;
    std::string pathIn, pathOut;
    int codeFile;

    std::cout << "Приветствую, какой файлик вы хотите отработать?\n" ;
    std::cin >> pathIn;

    std::cout << "Куда вы хотите сохранить ковертированный файл?\n";
    std::cin >> pathOut;

    pathIn = "input.txt";

    pathOut = "output.txt";
    // Открываем входящий файл для чтения
    fInput.open(pathIn);
    // Открываем выходящий файл для записи, очищая его при открытии
    fOutput.open(pathOut);

    if (!fInput.is_open() && !fOutput.is_open()) {
        std::cerr << "Не удалось открыть входящие и выходящие файлы. Проверьте имена.\n";
        return 1; // Завершаем программу с ошибкой
    }
    else {
        std::cout << "Какая кодировка вашего файла?\n";
        std::cin >> codeFile;
        switch (codeFile) {
        case 1251:
            convertFrom1251To866(fInput, fOutput);
            std::cout << "Перекодировка прошла успешно, ваш файлик готов";
            break;
        case 866:
            convertFrom866To1251(fInput, fOutput);
            std::cout << "Перекодировка прошла успешно, ваш файлик готов";
            break;
        default:
            std::cout << "К сожалению, мы с такой кодировкой не умеем работать.\n";
            return 1;

        } 

    };

    
    fInput.close(); // Закрытие файла
    fOutput.close();

    return 0;
}
