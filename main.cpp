#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include <iomanip>

class ConsoleTextEditor {
 private:
    std::vector<std::string> lines;
    std::string filename;
    bool modified;
    size_t cursorLine;
    size_t cursorPos;
    
    void showStatus() {
        std::cout << "\n--- ";
        if (!filename.empty()) {
            std::cout << filename;
            if (modified) std::cout << " *";
        } else {
            std::cout << "(Новый файл)";
            if (modified) std::cout << " *";
        }
        
        std::cout << " ---\n";
        if (!lines.empty() && cursorLine < lines.size()) {
            std::cout << "Строка " << (cursorLine + 1) << " из " << lines.size() << " | ";
            std::cout << "Позиция: " << cursorPos << " | ";
            std::cout << "Длина строки: " << lines[cursorLine].length() << "\n";
        }
    }
    
    bool processCommand(const std::string& cmd) {
        std::stringstream ss(cmd);
        std::string command;
        ss >> command;
        
        std::transform(command.begin(), command.end(), command.begin(), ::tolower);   //функцционал
        
        if (command == "help" || command == "h") {
            Help();
        }
        else if (command == "new" || command == "n") {
            newDocument();
        }
        else if (command == "save" || command == "s") {
            std::string fname;
            ss >> fname;
            saveFile(fname);
        }
        else if (command == "delete" || command == "d") {
            deleteLine();
        }
        else if (command == "quit" || command == "q") {
            if (checkUnsavedChanges()) {
                return true;
            }
        }
        else if (command == "edit" || command == "e") {
            editLine();
        }
        else if (command == "clear" || command == "c") {
            clearScreen();
        }
        else if (command == "undo" || command == "u") {
            std::cout << "Функция отмены пока не реализована\n";
        }
        else {
            std::cout << "Неизвестная команда. Введите 'help' для списка команд.\n";
        }
        return false;
    }
    
    void Help() {                                           // тут команды
        std::cout << "\n=== Список команд ===\n";
        std::cout << "help/h              - показать справку\n";
        std::cout << "new/n               - создать новый документ\n";
        std::cout << "save/s [файл]       - сохранить файл\n";
        std::cout << "delete/d            - удалить текущую строку\n";
        std::cout << "edit/e              - редактировать текущую строку\n";
        std::cout << "clear/c             - очистить экран\n";
        std::cout << "quit/q              - выйти\n";
        std::cout << "\nEnter чтобы продолжить ";
    }
    
    void newDocument() {
        if (checkUnsavedChanges()) {
            lines.clear();
            filename = "";
            modified = false;
            cursorLine = 0;
            cursorPos = 0;
            std::cout << "Создан новый документ\n";
        }
    }
    
    void saveFile(const std::string& fname = "") {
        std::string saveName = fname.empty() ? filename : fname;     // сохраняем файл здесь
        
        if (saveName.empty()) {
            std::cout << "Введите имя файла для сохранения: ";
            std::getline(std::cin, saveName);
        }
        
        }
        
        for (const auto& line : lines) {
            file << line << "\n";
        }
        
        filename = saveName;
        modified = false;
        std::cout << "Файл сохранен: " << saveName << " (" << lines.size() << " строк)\n";
    }
    
    bool checkUnsavedChanges() {                                    //проверка изменялся ли текс чи нет
        if (modified) {
            std::cout << "Есть несохраненные изменения. Сохранить? (y/n): ";
            std::string answer;
            std::getline(std::cin, answer);
            std::transform(answer.begin(), answer.end(), answer.begin(), ::tolower);
            
            if (answer == "y" || answer == "yes") {
                saveFile();
                return true;
            }
            else if (answer == "n" || answer == "no") {
                return true;
            }
            return false;
        }
        return true;
    }
    
    void deleteLine() {                                                 //это удаление
        if (cursorLine < lines.size()) {
            lines.erase(lines.begin() + cursorLine);
            if (cursorLine >= lines.size() && !lines.empty()) {
                cursorLine = lines.size() - 1;
                cursorPos = lines[cursorLine].length();
            }
            else if (lines.empty()) {
                cursorLine = 0;
                cursorPos = 0;
            }
            modified = true;
            std::cout << "Строка удалена\n";
        }
    }
    
    void editLine() {
        if (cursorLine < lines.size()) {
            std::cout << "Текущая строка: " << lines[cursorLine] << "\n";    //это эдит
            std::cout << "Введите новое содержимое: ";
            std::string newLine;
            std::getline(std::cin, newLine);
            lines[cursorLine] = newLine;
            cursorPos = newLine.length();
            modified = true;
            std::cout << "Строка изменена\n";
        }
    }
    
    void clearScreen() {                                         //очистить экран
        #ifdef _WIN32
            system("cls");
        #else
            system("clear");
        #endif
    }
};

public:
    ConsoleTextEditor() : modified(false), cursorLine(0), cursorPos(0) {} //типо интерфейс
    
    void run() {
        std::cout << "=== Консольный текстовый редактор ===\n";
        std::cout << "Введите команду (help - список команд):\n";
        
        std::string command;
        while (true) {
            showStatus();
            std::cout << "> ";
            std::getline(std::cin, command);
            
            if (processCommand(command)) {
                break;
            }
        }
    }

int main() {
    ConsoleTextEditor editor;
    editor.run();
    return 0;
}