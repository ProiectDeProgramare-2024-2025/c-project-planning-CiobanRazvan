#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cctype>

#ifdef _WIN32
#include <windows.h>
#endif

using namespace std;

void enableVirtualTerminalProcessing() {
#ifdef _WIN32
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE) return;

    DWORD dwMode = 0;
    if (!GetConsoleMode(hOut, &dwMode)) return;

    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    if (!SetConsoleMode(hOut, dwMode)) return;
#endif
}

class App1 {
private:
    vector<string> words;

    bool isValidWord(const string& word) {
        for (char c : word) {
            if (!isalpha(c)) return false;
        }
        return true;
    }

public:
    App1() {
        loadWords();
    }

    void loadWords() {
        words.clear();
        ifstream in("words.txt");
        string word;
        while (in >> word) {
            words.push_back(word);
        }
    }

    void saveWords() {
        ofstream out("words.txt");
        for (const string& word : words) {
            out << word << endl;
        }
    }

    void addWord(const string& newWord) {
        if (!isValidWord(newWord)) {
            cout << "\033[31mInvalid word: must contain only letters (A–Z).\033[0m\n";
            return;
        }
        words.push_back(newWord);
        saveWords();
        cout << "\033[34mWord added: " << newWord << "\033[0m\n";
    }

    void deleteWord(const string& target) {
        words.erase(remove(words.begin(), words.end(), target), words.end());
        saveWords();
        cout << "\033[33mWord deleted (if it existed): " << target << "\033[0m\n";
    }

    void clearHistory() {
        ofstream out("history.txt", ios::trunc);
        out.close();
        cout << "\033[32mGame history cleared.\033[0m\n";
    }

    void showWords() {
        if (words.empty()) {
            cout << "\033[31mNo words found.\033[0m\n";
            return;
        }
        cout << "\033[36mWords in the list:\033[0m\n";
        for (const string& word : words) {
            cout << " - " << word << endl;
        }
    }
};

int main(int argc, char* argv[]) {
    enableVirtualTerminalProcessing();

    App1 app;

    if (argc == 3 && string(argv[1]) == "add") {
        app.addWord(argv[2]);
    } else if (argc == 3 && string(argv[1]) == "delete") {
        app.deleteWord(argv[2]);
    } else if (argc == 2 && string(argv[1]) == "clear") {
        app.clearHistory();
    } else if (argc == 2 && string(argv[1]) == "show") {
        app.showWords();
    } else {
        cout << "\033[31mInvalid argument or wrong number of arguments.\033[0m\n";
        cout << "Usage:\n";
        cout << argv[0] << " add <word>\n";
        cout << argv[0] << " delete <word>\n";
        cout << argv[0] << " clear\n";
        cout << argv[0] << " show\n";
    }

    return 0;
}
