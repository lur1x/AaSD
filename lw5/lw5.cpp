#include <fstream>
#include <iostream>
#include <vector>
#include <windows.h>
#include <string>
#include <sstream>
#include <chrono>

/* 19. Имеется N костей игры домино. На каждой кости имеются 2
числа от 0 до 6. Кости могут повторяться.  Требуется  написать
программу, которая будет определять максимальное число из цифр
на костях цепочки, составленной по правилам домино из имеющихся
костей. Время счета до 1 с.
   Ввод. Первая строка входного файла содержит целое число N –
количество костей (2 <= N <= 15). Следующие N строк содержат 2
целых числа X и Y (0 <= X, Y <= 6), разделенные пробелом.
   Вывод. В выходной файл необходимо вывести максимальное целое
число, цифры которого соответствуют значению костей в цепочке(11).
Кириллов Дмитрий ПС22 VS2
*/

struct TypeDominoes {
    int firstDigit;
    int secondDigit;
    bool used = false;
};


void SetConsoleEncoding() {
    setlocale(LC_ALL, "ru");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
}

std::ifstream OpenInputFile(const std::string& prompt) {

    std::string pathFile;
    std::ifstream file;

    while (true) {
        std::cout << prompt << std::endl;
        std::cin >> pathFile;

        if (pathFile == "EXIT") {
            std::cout << "Завершение работы." << std::endl;
            exit(0);
        }

        file.open(pathFile);
        if (!file.is_open()) {
            std::cerr << "Не удалось открыть файл. Проверьте имя" << std::endl;
        }
        else {
            return file;
        }
    }
}

bool ReadDominoes(std::ifstream& fInput, std::vector<TypeDominoes*>& Dominoes, std::vector<std::vector<TypeDominoes*>>& ListDominoes) {
    int N;
    std::string line;

    while (std::getline(fInput, line)) {
        if (line.empty()) continue;
        std::istringstream iss(line);
        if (!(iss >> N)) {
            std::cerr << "Ошибка: Не удалось считать число из строки: " << line << std::endl;
            return false;
        }
        if (N < 2 || N > 15) {
            std::cerr << "Ошибка: N не входит в диапазон 2 <= N <= 15" << std::endl;
            return false;
        }
        break;
    }

    int i = 0;
    while (i < N) {
        std::getline(fInput, line);
        if (line.empty()) continue;

        std::istringstream iss(line);
        int firstDigit, secondDigit;
        if (!(iss >> firstDigit >> secondDigit)) {
            std::cerr << "Ошибка: Не удалось считать числа из строки: " << line << std::endl;
            return false;
        }

        TypeDominoes* newDomino = new TypeDominoes();
        newDomino->firstDigit = firstDigit;
        newDomino->secondDigit = secondDigit;

        Dominoes.push_back(newDomino);

        if (firstDigit != secondDigit) {
            ListDominoes[firstDigit].push_back(newDomino);
            ListDominoes[secondDigit].push_back(newDomino);
        }
        else {
            ListDominoes[firstDigit].push_back(newDomino);
        }

        i++;
    }

    if (fInput.eof() && i < N) {
        std::cerr << "Ошибка: неверный ввод данных или количество доминошек меньше, чем N" << std::endl;
        return false;
    }

    return true;
}

void PrintDominoes(const std::vector<TypeDominoes*>& Dominoes) {
    for (const auto& domino : Dominoes) {
        std::cout << domino->firstDigit << " " << domino->secondDigit << std::endl;
    }
}

void PrintListDominoes(const std::vector<std::vector<TypeDominoes*>>& ListDominoes) {
    for (size_t i = 0; i < ListDominoes.size(); ++i) {
        std::cout << "Digit " << i << ": ";
        for (const TypeDominoes* domino : ListDominoes[i]) {
            std::cout << "(" << domino->firstDigit << ", " << domino->secondDigit << " used: " << domino->used << ") ";
        }
        std::cout << std::endl;
    }
}

void CompareNums(std::string& tempNum, std::string& maxNum) {
    if (tempNum.length() == maxNum.length()) {
        for (int i = 0; i < tempNum.length(); i++) {
            if (tempNum[i] > maxNum[i]) {
                maxNum = tempNum;
                break;
            }
            else if (tempNum[i] < maxNum[i]) {
                break;
            }

        }
    }
    else if (tempNum.length() > maxNum.length()) {
        maxNum = tempNum;

    }
}
void MakeMaxNumRecursivly(std::string& maxNum, std::string tempNum, std::vector<TypeDominoes*>& Dominoes, std::vector<std::vector<TypeDominoes*>>& ListDominoes, int lastNum) {
    std::string newNum;
    for (auto& d : ListDominoes[lastNum]) {
        if (d->used == false) {
            d->used = true;


            if (d->firstDigit == lastNum) {
                newNum = tempNum + std::to_string(d->firstDigit) + std::to_string(d->secondDigit);
            }
            else {
                newNum = tempNum + std::to_string(d->secondDigit) + std::to_string(d->firstDigit);
            }

            CompareNums(newNum, maxNum);
            MakeMaxNumRecursivly(maxNum, newNum, Dominoes, ListDominoes, d->firstDigit == lastNum ? d->secondDigit : d->firstDigit);

            d->used = false;
        }

    }


}

std::string MakeMaxNum(std::vector<TypeDominoes*>& Dominoes, std::vector<std::vector<TypeDominoes*>>& ListDominoes) {
    std::string maxNum = "";

    for (auto& d : Dominoes) {
        d->used = true;
        if (d->firstDigit != d->secondDigit) {

            std::string newNum = std::to_string(d->firstDigit) + std::to_string(d->secondDigit);
            MakeMaxNumRecursivly(maxNum, newNum, Dominoes, ListDominoes, d->secondDigit);

            newNum = std::to_string(d->secondDigit) + std::to_string(d->firstDigit);
            MakeMaxNumRecursivly(maxNum, newNum, Dominoes, ListDominoes, d->firstDigit);

            d->used = false;
        }
        else {
            std::string newNum = std::to_string(d->firstDigit) + std::to_string(d->secondDigit);
            MakeMaxNumRecursivly(maxNum, newNum, Dominoes, ListDominoes, d->secondDigit);

            newNum = std::to_string(d->secondDigit) + std::to_string(d->firstDigit);
            MakeMaxNumRecursivly(maxNum, newNum, Dominoes, ListDominoes, d->firstDigit);

            d->used = false;
        }
    }
    return maxNum;
}

std::string FindMaxNumFromDominoes(std::ifstream& fInput) {

    std::vector<TypeDominoes*> Dominoes;
    std::vector<std::vector<TypeDominoes*>> ListDominoes(7);

    if (!ReadDominoes(fInput, Dominoes, ListDominoes)) {
        return "";
    }

    /*PrintDominoes(Dominoes);
    std::cout << std::endl;
    PrintListDominoes(ListDominoes);
    std::cout << std::endl;
    */

    std::string maxNum = MakeMaxNum(Dominoes, ListDominoes);


    for (auto& domino : Dominoes) {
        delete domino;
    }

    return maxNum;
}

int main() {

    SetConsoleEncoding();
    std::ifstream fInput;

    while (true) {
        //std::ifstream fInput = OpenInputFile("Введите имя файла c домино  или \"EXIT\" для выхода из программы:");

        fInput.open("1.txt");
        auto start = std::chrono::high_resolution_clock::now();
        std::string maxNum = FindMaxNumFromDominoes(fInput);
        if (!maxNum.empty()) {
            std::cout << maxNum;
        }

        else {
            std::cout << "При считывании возникли ошибки" << std::endl;
        }

        std::cout << std::endl;

        fInput.close();
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> duration = end - start;
        std::cout << duration.count() << std::endl;
        break;
    }

    return 0;
}