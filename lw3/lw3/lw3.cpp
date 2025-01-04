#include <iostream>
#include <fstream>
#include <vector>
#include <windows.h>
#include <string>

/*
24. В  листьях  И-ИЛИ  дерева,  соответствующего некоторому
множеству  конструкций,  заданы   значения   массы.   Известно
максимально допустимое значение массы изделия. Требуется усечь
дерево   так,   чтобы   дерево    включало    все    элементы,
соответствующие  допустимым  значениям массы,  но не содержало
"лишних" вершин.  Конечное дерево выдать на экран в  наглядном
виде (12).
Кириллов Дмитрий ПС-22
*/


struct Tree {
    std::string name;
    int level;
    std::string branchType;
    Tree* father;                  
    std::vector<Tree*> sons;   
    int minWeight;
    int maxWeight;
    bool isTruncate = false;
};

int ReadTree(std::ifstream& F, Tree*& root) {

    std::string str;
    int i, j, level, prevLevel, len;
    Tree* currRoot, * top, * prevRoot;      
    prevLevel = -1;                 
    prevRoot = 0;      

    while (!F.eof()) {

        getline(F, str);

        len = str.length();
        if (len == 0) continue;

        level = 0;
        while (str[level] == '.') level++;
        
        currRoot = new Tree;
        i = 0;
        j = level;
        while ((j < len) && (str[j] != ' ')) {
            currRoot->name += str[j++];
        }

        if (str[j] == ' ') {
            str = str.substr(j + 1);
            try {
                currRoot->maxWeight = stoi(str);
                currRoot->minWeight = currRoot->maxWeight;
                currRoot->branchType = "D";
            }
            catch (...) {
                if (str == "A" || str == "a") {
                    currRoot->branchType = "A";
                    currRoot->maxWeight = -1;
                    currRoot->minWeight = -1;
                }
                else if (str == "O" || str == "o") {
                    currRoot->branchType = "O";
                    currRoot->maxWeight = -1;
                    currRoot->minWeight = -1;
                }
                else {
                    std::cerr << "Ошибка ввода: проверьте своё дерево" << std::endl;
                    return 1;
                }
            }
        }
        else {
            std::cerr << "Ошибка ввода: проверьте своё дерево" << std::endl;
            return 1;
        }

        currRoot->level = level;

        if (level == 0) {
            root = currRoot;
            prevRoot = root;
            prevLevel = 0;
            continue;
        }
        if (level > prevLevel) {
            prevRoot->sons.push_back(currRoot);
            currRoot->father = prevRoot;
        }
        else if (level == prevLevel) {
            top = prevRoot->father;
            top->sons.push_back(currRoot);
            currRoot->father = top;
        }
        else {
            top = prevRoot;
            for (i = 0; i <= prevLevel - level; i++) top = top->father;
            
            top->sons.push_back(currRoot);
            currRoot->father = top;
        }
        prevLevel = level;     
        prevRoot = currRoot;      
    }
    return 0;
}

int CountWeights(Tree* root) {

    int sumMax = 0;
    int sumMin = 0;

    if (!root->sons.empty()) {

        for (int i = 0; i < root->sons.size(); i++) {
            CountWeights(root->sons[i]);
        }

        if (root->branchType == "O") {

            for (int j = 0; j < root->sons.size(); j++) {
                if (root->sons[j]->maxWeight != -1) {
                    if (root->sons[j]->maxWeight > root->maxWeight) {
                        root->maxWeight = root->sons[j]->maxWeight;
                    }
                }
                if (root->sons[j]->minWeight != -1) {
                    if (root->sons[j]->minWeight < root->minWeight || root->minWeight == -1) {
                        root->minWeight = root->sons[j]->minWeight;
                    }

                }
            }
        }
        else if (root->branchType == "A") {

            for (int k = 0; k < root->sons.size(); k++) {
                if (root->sons[k]->maxWeight != -1) {
                    sumMax += root->sons[k]->maxWeight;
                }
                if (root->sons[k]->minWeight != -1) {
                    sumMin += root->sons[k]->minWeight;
                }
            }

            root->maxWeight = sumMax;
            root->minWeight = sumMin;
        }
    }
    
    return 0;
}

int TruncateTree(Tree* root, int weight) {

    int newWeight = 0;

    for (int i = 0; i < root->sons.size(); i++) {

        if (root->sons[i]->minWeight > weight) {
            root->sons[i]->isTruncate = true;
        }

        else if (root->sons[i]->branchType == "A" && root->sons[i]->minWeight > weight) {
            root->sons[i]->isTruncate = true;
        }

        else if (root->sons[i]->branchType == "O" && root->branchType == "O") {
            newWeight = weight;
            TruncateTree(root->sons[i], newWeight);
        }

        else if (root->sons[i]->branchType == "O" && root->branchType == "A") {
            newWeight = weight - root->minWeight + root->sons[i]->minWeight;
            TruncateTree(root->sons[i], newWeight);
        }

        else if (root->sons[i]->branchType == "D" && root->sons[i]->maxWeight > weight) {
            root->sons[i]->isTruncate = true;
        }
    }
    
    return 0;
}

int PrintTree(Tree* root, int level) {

    if (!root->isTruncate) {

        for (int i = 0; i < level; i++) std::cout << '.';
        std::cout << root->name;
        if (root->branchType == "D") {
            std::cout << " " << root->minWeight << std::endl;
        }
        else {
            std::cout << " " << root->branchType << " " << std::endl;
        }

        for (int i = 0; i < root->sons.size(); i++)
            PrintTree(root->sons[i], level + 1);
    }
    return 0;
}

int DeleteTree(Tree*& root) {

    for (int i = 0; i < root->sons.size(); i++) {
        DeleteTree(root->sons[i]);
    }
    delete root;
    root = 0;

    return 0;
}

void HandleTree(std::ifstream& fInput) {

    int weight;
    Tree* root = NULL;

    if (ReadTree(fInput, root) == 0) {

        std::cout << "Файл удачно удалось прочитать. Вот ваше дерево:" << std::endl;
        std::cout << std::endl;
        PrintTree(root, 0);
        std::cout << std::endl;

        CountWeights(root);
        std::cout << "Введите вес, по которому хотите усечь дерево" << std::endl;
        std::cin >> weight;

        if (weight >= root->maxWeight) {
            std::cout << "Ваше значение не меньше максимального веса дерева. Деревео не будет усечено" << std::endl;
        }
        else if (weight < root->minWeight) {
            std::cout << "Ваше значение меньше минимального веса дерева. Дерево полностью будет усечено" << std::endl;
        }
        else {
            TruncateTree(root, weight);
            std::cout << "Ваше усеченное дерево:" << std::endl;
            std::cout << std::endl;
            PrintTree(root, 0);
            std::cout << std::endl;
        }
        
    }

    if (root) {
        DeleteTree(root);
        std::cout << "Дерево удалено, память освобождена" << std::endl;
    }
}

int main() {

    setlocale(LC_ALL, "ru");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    std::ifstream fInput;
    std::string pathIn;


    while (true) {
        
        std::cout << "Введите имя входящего файла или \"EXIT\" для выхода из программы:" << std::endl;
        std::cin >> pathIn;

        if (pathIn == "EXIT") {
            std::cout << "Завершение работы.";
            break;
        }

        fInput.open(pathIn);

        if (!fInput.is_open()) {
            std::cerr << "Не удалось открыть входящий файл. Проверьте имя" << std::endl;
        }
        else {
            std::cout << "Открыли файл" << std::endl;
            HandleTree(fInput);

        };

        fInput.close(); 
        std::cout << "Закрыли файл" << std::endl;
    }

    return 0;
}
