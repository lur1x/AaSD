/* 27. Информация  о  некотором  изделии  задана   с   помощью
ориентированного графа.Вершина графа соответствует отдельной
операции, а дуги показывают очередность  выполнения  операций
(каждая   операция   может   начаться   не   ранее   окончания
предшествующих операций).Продолжительность  каждой  операции
известна.Имеется  один  станок  для  выполнения каждого типа
операций.Если станок освобождается, то производится  та  из
готовых  к выполнению операций, для которой максимальный путь
от соответствующей вершины до конца  имеет  наибольшую  длину.
Определить  время  изготовления  изделия.Выдать общий график
выполнения операций и график загрузки каждого станка(12).

Кириллов Дмитрий ПС-22
Visual Studio 22
*/

#include <iostream>>
#include <fstream>
#include <vector>
#include <windows.h>
#include <string>
#include <sstream>


struct Vertex {
    int number;
    std::string detail;
    std::string workspace;
    int time = 0;
    int startTime = 0;
    int endTime = 0;
    int state= 1; // 1 - ожидание, 2 - готов к выполнению, 3 - выполняется, 4 - выполнен
    std::vector <int> nextVertexs; 
    std::vector <int> prevVertexs;
};
struct Graph {
    std::vector <Vertex> vertexs;
};

struct Workspace {
    std::string type;
    bool free = true;
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
std::ofstream OpenOutputFile(const std::string& prompt) {
    std::string pathFile;
    std::ofstream file;

    while (true) {
        std::cout << prompt << std::endl;
        std::cin >> pathFile;
        file.open(pathFile);

        if (!file.is_open()) {
            std::cerr << "Не удалось открыть файл. Проверьте имя" << std::endl;
        }
        else {
            return file;
        }
    }
}
bool AddEdge(Graph& graph, int from, int to) {
   
    if (from == to) {
        std::cerr << "Ошибка: Вершина не может вести к самому себе" << std::endl;
        return false;
    }

    for (auto& vertex : graph.vertexs) {
        if (vertex.number == from) {
            for (int next : vertex.nextVertexs) {
                if (next == to) {
                    std::cerr << "Ошибка: Дуга уже существует в графе: " << from << " -> " << to << std::endl;
                    return false; 
                }
            }

            vertex.nextVertexs.push_back(to);

            for (auto& vertex : graph.vertexs) {
                if (vertex.number == to) {
                    vertex.prevVertexs.push_back(from);
                    return true;
                }
            }

            Vertex newVertex;
            newVertex.number = to;
            newVertex.prevVertexs.push_back(from);
            graph.vertexs.push_back(newVertex);
            return true; 
        }
    }

    bool found = false;
    for (auto& vertex : graph.vertexs) {
        if (vertex.number == to) {
            vertex.prevVertexs.push_back(from);
            found = true;
        }
    }
    Vertex newVertex;
    if (!found) {
        newVertex.number = to;
        newVertex.prevVertexs.push_back(from);
        graph.vertexs.push_back(newVertex);
        newVertex = Vertex(); 
    }

    newVertex.number = from;
    newVertex.state = 2;
    newVertex.nextVertexs.push_back(to); 
    graph.vertexs.push_back(newVertex);
    return true;
}

bool ReadGraph(std::ifstream& fGraph, Graph& graph) {
    std::string line;

    while (std::getline(fGraph, line)) {

        if (line.empty()) continue;

        std::istringstream iss(line);
        int from, to;

        if (!(iss >> from >> to)) {
            std::cerr << "Ошибка: Не удалось считать числа из строки: " << line << std::endl;
            return false;
        }
        
        if (!AddEdge(graph, from, to)) {
            std::cerr << "Ошибка: Не удалось считать числа из строки: " << line << std::endl;
            return false; 
        }
    }
}

void CheckExistenceWorkspace(std::string typeWorkspace, std::vector <Workspace>& ListWorkspace) {
    bool found = false;

    for (auto& workspace : ListWorkspace) {
        if (workspace.type == typeWorkspace) {
            found = true;
            break;
        }
    }

    if (!found) {
        Workspace NewWorkspace;
        NewWorkspace.type = typeWorkspace;
        ListWorkspace.push_back(NewWorkspace);
    }
}

bool ReadOperations(std::ifstream& fOperations, Graph& graph, std::vector <Workspace>& ListWorkspace) {
    std::string line;
    while (std::getline(fOperations, line)) {

        if (line.empty()) continue;

        std::istringstream iss(line);
       
        int number, time;
        std::string detail, workspace;
        if (!(iss >> number >> detail >> workspace >> time)) {
            std::cerr << "Ошибка: Не удалось считать данные из строки: " << line << std::endl;
            return false;
        }

        bool found = false;
        for (auto& vertex : graph.vertexs) {
            if (vertex.number == number) {
                vertex.detail = detail;
                CheckExistenceWorkspace(workspace, ListWorkspace);
                vertex.workspace = workspace;
                vertex.time = time;
                found = true;
                break;
            }
        }

        if (!found) {
            std::cerr << "Ошибка: Вершина с номером " << number << " не найдена в графе." << std::endl;
            return false; 
        }
    }
    for (auto& vertex : graph.vertexs) {
        if (vertex.detail.empty() && vertex.workspace.empty() && vertex.time == 0) {
            std::cerr << "Ошибка: Вершина с номером " << vertex.number << " полностью не заполнена." << std::endl;
            return false;
        }
    }
    return true;
}

void UpdateStateVertexs(Graph& graph) {

    for (auto& vertex : graph.vertexs) {
        if (vertex.state == 1) {
            bool allPrevFinished = true;
            for (int prev : vertex.prevVertexs) {
                bool found = false;
                for (const auto& v : graph.vertexs) {
                    if (v.number == prev) {
                        found = true;
                        if (v.state != 4) { 
                            allPrevFinished = false;
                        }
                        break;
                    }
                }
                if (!found) {
                    allPrevFinished = false;
                }
            }

            if (allPrevFinished) {
                vertex.state = 2;
            }
        }
    }
    //for (auto& vertex : graph.vertexs) {
    //    std::cout << "Вершина " << vertex.number << ": статус " << vertex.state << std::endl;
    //}
    //std::cout << std::endl;
}

bool IsRunning(const Graph& graph) {
    for (const auto& vertex : graph.vertexs) {
        if (vertex.state != 4) {
            return true;
        }
    }
    return false;
}

int GetMaxPrevEndTime(const Graph& graph, const std::vector<int>& prevVertexs) {
    int maxEndTime = 0;
    for (int prev : prevVertexs) {
        for (const auto& vertex : graph.vertexs) {
            if (vertex.number == prev) {
                if (vertex.endTime > maxEndTime) {
                    maxEndTime = vertex.endTime;
                }
                break;
            }
        }
    }
    return maxEndTime;
}

bool CheckFreeWorkspace(std::string TypeWorkspace, std::vector<Workspace>& ListWorkspace) {
    for (const auto& workspace : ListWorkspace) {
        if (workspace.type == TypeWorkspace) {
            return workspace.free;
        }
    }
}
int GetMaxLengthFromVertex(const Graph& graph, const Vertex& vertex) {
    
    if (vertex.nextVertexs.empty()) {
        return vertex.time;
    }

    int maxLength = 0;

    
    for (int next : vertex.nextVertexs) {
        for (const auto& v : graph.vertexs) {
            if (v.number == next) {
                int lengthFromNext = GetMaxLengthFromVertex(graph, v) + vertex.time;
                if (lengthFromNext > maxLength) {
                    maxLength = lengthFromNext; 
                }
                break; 
            }
        }
    }

    return maxLength; 
}


Vertex FindVertexWithMaxPath(const std::string& typeWorkspace, const Graph& graph) {
    Vertex maxVertex; 
    maxVertex.time = -1;
    int maxLength = -1;

    for (const auto& vertex : graph.vertexs) {
        if (vertex.workspace == typeWorkspace && vertex.state == 2) {
            int pathLength = GetMaxLengthFromVertex(graph, vertex);
            if (pathLength > maxLength) {
                maxLength = pathLength;
                maxVertex = vertex; 
            }
        }
    }

    return maxVertex; 
}
int GetEndTimeWorkspace(std::string workspace, Graph& graph) {
    int maxEndTime = 0; 

    for (const auto& vertex : graph.vertexs) {
        if (vertex.workspace == workspace) { 
            if (vertex.endTime > maxEndTime) {
                maxEndTime = vertex.endTime;
            }
        }
    }

    return maxEndTime; 
}


void HandleGraph(Graph& graph, std::vector<Workspace>& ListWorkspace) {
    while (IsRunning(graph)) {
        for (auto& vertex : graph.vertexs) {
            if (vertex.state == 2) {
                bool f;
                if (f = CheckFreeWorkspace(vertex.workspace, ListWorkspace)) {
                    Vertex maxVertex = FindVertexWithMaxPath(vertex.workspace, graph);
                    if (maxVertex.time >= 0) { 
                        
                        int maxPrevEndTime = GetMaxPrevEndTime(graph, maxVertex.prevVertexs);
                        int endTimeWorkspace = GetEndTimeWorkspace(maxVertex.workspace, graph);
                        if (maxPrevEndTime > endTimeWorkspace) {
                            maxVertex.startTime = maxPrevEndTime;
                        }
                        else {
                            maxVertex.startTime = endTimeWorkspace;
                        }
                        maxVertex.endTime = maxVertex.startTime + maxVertex.time;
                        maxVertex.state = 4; 

                        for (auto& v : graph.vertexs) {
                            if (v.number == maxVertex.number) {
                                v = maxVertex; 
                                break; 
                            }
                        }
                        UpdateStateVertexs(graph);
                    }

                    
                }
                
            }
        }
    }
}


void InsertionSort(Graph& graph) {
    for (size_t i = 1; i < graph.vertexs.size(); ++i) {
        Vertex key = graph.vertexs[i];
        size_t j = i;

        while (j > 0 && (graph.vertexs[j - 1].endTime > key.endTime || (graph.vertexs[j - 1].endTime == key.endTime && graph.vertexs[j - 1].startTime > key.startTime))) {
            graph.vertexs[j] = graph.vertexs[j - 1]; 
            --j;
        }
        graph.vertexs[j] = key; 
    }
}


void PrintOperationsReport(Graph& graph, std::ofstream& fOutput) {
    
    InsertionSort(graph);
    int lengthCell = 12;
    fOutput << "##################################################################"  << std::endl;
    fOutput << "#  Операция  #   Деталь   # Тип станка #   Начало   #   Конец    #" << std::endl;
    fOutput << "##################################################################" << std::endl;
    for (auto& vertex : graph.vertexs) {
        fOutput << "#";
        for (int i = 0; i < lengthCell - std::to_string(vertex.number).length(); i++) {
            fOutput << " ";
            if (i == (lengthCell - std::to_string(vertex.number).length()) / 2) {
                fOutput << vertex.number;
            }
        }
        fOutput << "#";
        for (int i = 0; i < lengthCell - vertex.detail.length(); i++) {
            fOutput << " ";
            if (i == (lengthCell - vertex.detail.length()) / 2) {
                fOutput << vertex.detail;
            }
        }
        fOutput << "#";
        for (int i = 0; i < lengthCell - vertex.workspace.length(); i++) {
            fOutput << " ";
            if (i == (lengthCell - vertex.workspace.length()) / 2) {
                fOutput << vertex.workspace;
            }
        }
        fOutput << "#";
        for (int i = 0; i < lengthCell - std::to_string(vertex.startTime).length(); i++) {
            fOutput << " ";
            if (i == (lengthCell - std::to_string(vertex.startTime).length()) / 2) {
                fOutput << vertex.startTime;
            }
        }
        fOutput << "#";
        for (int i = 0; i < lengthCell - std::to_string(vertex.endTime).length(); i++) {
            fOutput << " ";
            if (i == (lengthCell - std::to_string(vertex.endTime).length()) / 2) {
                fOutput << vertex.endTime;
            }
        }
        fOutput << "#" << std::endl;
        
    }
    fOutput << "##################################################################" << std::endl;
   

}

void PrintWorkspacesReport(Graph& graph, std::ofstream& fOutput, std::vector<Workspace>& ListWorkspace) {
    for (const auto& workspace : ListWorkspace) { 
        fOutput << "Станок " << workspace.type << ": ";
        for (const auto& vertex : graph.vertexs) {
            if (vertex.workspace == workspace.type) { 
                fOutput << vertex.number << "[" << vertex.startTime << ", " << vertex.endTime << "]" << "; ";
            }
        }
        fOutput << std::endl;
    }
}

void InsertSort(Graph& graph) {
    for (size_t i = 1; i < graph.vertexs.size(); ++i) {
        Vertex key = graph.vertexs[i];
        size_t j = i;

        while (j > 0 && (graph.vertexs[j - 1].number > key.number || (graph.vertexs[j - 1].number == key.number))) {
            graph.vertexs[j] = graph.vertexs[j - 1];
            --j;
        }
        graph.vertexs[j] = key;
    }
}

int HandleFiles(std::ifstream& fGraph, std::ifstream& fOperations, std::ofstream& fOperationReport, std::ofstream& fWorkspaceReport) {
    
    Graph graph; 
    std::vector <Workspace> ListWorkspace;

    if (!ReadGraph(fGraph, graph)) {
        return 1;
    }

    if (!ReadOperations(fOperations, graph, ListWorkspace)) {
        return 1;
    };

    //InsertSort(graph);

    /*for (auto& vertex : graph.vertexs) {
        std::cout << "Вершина " << vertex.number << ": ";
        std::cout << "Статус: " << vertex.state << "; ";
        std::cout << "Деталь: " << vertex.detail << "; ";
        std::cout << "Станок: " << vertex.workspace << "; ";
        std::cout << "Время: "  << vertex.time << "; ";
        std::cout << "Начало: " << vertex.startTime << "; ";
        std::cout << "Конец: " << vertex.endTime << "; " << std::endl;
        for (auto& next : vertex.nextVertexs) {
            std::cout << next << "; ";
        }
        std::cout << std::endl;
        for (auto& prev : vertex.prevVertexs) {
            std::cout << prev << "; ";
        }
        std::cout << std::endl;

    }
    */

    HandleGraph(graph, ListWorkspace);

    PrintOperationsReport(graph, fOperationReport);
    PrintWorkspacesReport(graph, fWorkspaceReport, ListWorkspace);
    std::cout << "Обработка прошла успешно" << std::endl;
    return 0;
}

int main() {

    SetConsoleEncoding();
    std::ifstream fOperations, fGraph;
    std::ofstream fOperationReport, fWorkspaceReport;

    while (true) {
        
        /*std::ifstream fOperations = OpenInputFile("Введите имя файла c операциями или \"EXIT\" для выхода из программы:");
        std::ifstream fGraph = OpenInputFile("Введите имя файла c графом:");

        std::ofstream fOperationReport = OpenOutputFile("Введите имя файла, куда выветси отчёт по операциям:");
        std::ofstream fWorkspaceReport = OpenOutputFile("Введите имя файла, куда выветси отчёт по станкам:");
        */
        fOperations.open("1.txt");
        fGraph.open("2.txt");
        fOperationReport.open("3.txt");
        fWorkspaceReport.open("4.txt");
       

        HandleFiles(fGraph, fOperations, fOperationReport, fWorkspaceReport);
        std::cout << std::endl;

        fOperations.close();
        fGraph.close();
        fOperationReport.close();
        fWorkspaceReport.close();
        break;
    }

    return 0;
}
