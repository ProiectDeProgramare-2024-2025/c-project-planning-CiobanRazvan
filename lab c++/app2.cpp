#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>
#include <cstdlib>
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

class App2 {
private:
    vector<string> words;
    vector<char> guessed;
    string word;
    int lives;

public:
    App2() {
        loadWords();
    }

    void loadWords() {
        words.clear();
        ifstream in("words.txt");
        string w;
        while (in >> w) words.push_back(w);
    }

    void saveHistory(const string& word) {
        ofstream out("history.txt", ios::app);
        out << word << endl;
    }

    void showHistory() {
        ifstream in("history.txt");
        string word;
        cout << "\033[35m--- Game History ---\033[0m\n";
        while (getline(in, word)) {
            cout << "\033[35m" << word << "\033[0m" << endl;
        }
        cout << "\033[35m--------------------\033[0m\n";
    }

    string playGame(int difficultyLives = 6) {
        if (words.empty()) return "No words available.";

        srand(time(0));
        word = words[rand() % words.size()];
        guessed.clear();
        lives = difficultyLives;
        saveHistory(word);

        while (lives > 0) {
            for (char c : word) {
                if (find(guessed.begin(), guessed.end(), c) != guessed.end())
                    cout << c << ' ';
                else
                    cout << "_ ";
            }
            cout << "\nLives left: " << lives << "\n";

            char guess;
            cout << "Guess a letter: ";
            cin >> guess;

            if (!isalpha(guess)) {
                cout << "\033[31mInvalid input. Please enter a letter (a-z).\033[0m\n";
                continue;
            }

            if (find(guessed.begin(), guessed.end(), guess) != guessed.end()) {
                cout << "Already guessed.\n";
                continue;
            }

            guessed.push_back(guess);

            if (word.find(guess) == string::npos) {
                cout << "Wrong!\n";
                lives--;
            } else {
                cout << "Correct!\n";
            }

            bool allGuessed = true;
            for (char c : word) {
                if (find(guessed.begin(), guessed.end(), c) == guessed.end()) {
                    allGuessed = false;
                    break;
                }
            }

            if (allGuessed) {
                return "\033[32mYou won! Word was: " + word + "\033[0m";
            }
        }

        return "\033[31mYou lost! Word was: " + word + "\033[0m";
    }
};

int main(int argc, char* argv[]) {
    enableVirtualTerminalProcessing();

    App2 app;

    if (argc == 2 && string(argv[1]) == "history") {
        app.showHistory();

    } else if (argc >= 2 && string(argv[1]) == "play") {
        int lives = 6;

        if (argc == 3) {
            string difficulty = argv[2];
            if (difficulty == "easy") {
                lives = 8;
            } else if (difficulty == "medium") {
                lives = 6;
            } else if (difficulty == "hard") {
                lives = 4;
            } else {
                cout << "\033[33mInvalid difficulty level.\n";
                cout << "Please choose: easy, medium, or hard.\033[0m\n";
                return 1;
            }
        }

        string choice;
        do {
            cout << "\n===== HANGMAN GAME =====\n";
            cout << app.playGame(lives) << endl;
            cout << "Play again? (y/n): ";
            cin >> choice;
        } while (choice == "y" || choice == "Y");

    } else {
        cout << "Usage:\n";
        cout << argv[0] << " play [easy|medium|hard]\n";
        cout << argv[0] << " history\n";
    }

    return 0;
}