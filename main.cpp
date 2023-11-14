#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <map>
#include <cctype>
#include <algorithm>
#include <windows.h>

using namespace std;

class HangmanGame {
private:
    vector<string> words;
    string secretWord;
    string guessedWord;
    int maxAttempts;
    int attemptsLeft;
    map<string, vector<string>> categories;
    string currentCategory;
    int hintsLeft;
    int score;
    vector<pair<string, int>> highScores;

public:
    HangmanGame(const string& filename, int maxAttempts)
        : maxAttempts(maxAttempts), attemptsLeft(maxAttempts), hintsLeft(1), score(0) {
        loadWordsFromFile(filename);
        srand(static_cast<unsigned>(time(0))); // Seed srand with unsigned time
        selectCategory();
        selectWord();
        loadHighScores(); // Load high scores from the file

        // Initialize guessedWord with the first letter of secretWord
        guessedWord = secretWord[0] + string(secretWord.length() - 1, '_');
    }

    void loadWordsFromFile(const string& filename) {
        ifstream file(filename);
        if (file.is_open()) {
            string line;
            while (getline(file, line)) {
                size_t pos = line.find(':');
                if (pos != string::npos) {
                    string category = line.substr(0, pos);
                    string wordsStr = line.substr(pos + 1);
                    vector<string> words;
                    size_t start = 0, end;
                    while ((end = wordsStr.find(',', start)) != string::npos) {
                        words.push_back(wordsStr.substr(start, end - start));
                        start = end + 1;
                    }
                    words.push_back(wordsStr.substr(start));
                    categories[category] = words;
                }
            }
            file.close();
        } else {
            cerr << "Error: Unable to open file " << filename << endl;
            exit(1);
        }
    }

    void selectCategory() {
        cout << "Choose a category: " << endl;
        cout << "-----------------------" << endl;
        for (const auto& category : categories) {
            cout << category.first << endl;
        }
        cout << ".................." << endl;
        cout<<"\n";

        cin >> currentCategory;
        while (categories.find(currentCategory) == categories.end()) {
            cout << "Invalid category. Please choose a valid category: " << endl;
            cin >> currentCategory;
        }
    }

    void selectWord() {
        const vector<string>& wordsInCategory = categories[currentCategory];
        secretWord = wordsInCategory[rand() % wordsInCategory.size()];
    }

    // int calculateScore() {
    //     return attemptsLeft * 100;
    // }

    void play() {
        string playerName = getPlayerName();
        cout << "Welcome to Hangman, " << playerName << "!" << endl;
        cout<<"\n";
        cout<<"**You have only one hint**.\n**So use it wisely **.\n";
        cout<<"\n";

        while (attemptsLeft > 0) {
            displayHangman();
            displayGuessedWord();
            if (guessedWord == secretWord) {
                displayHangman();
                cout << "  _   _   ____   ____  " << endl;
                cout << " / \\ / \\ |  _ \\ |  _ \\ " << endl;
                cout << "| N | I || C || E ||" << endl;
                cout << " \\_/ \\_/ |_|_||_||_||" << endl;
                cout << " ......................" << endl;
                score = calculateScore();
                cout << "Congratulations, " << playerName << "! You guessed the word: " << secretWord << endl;
                Beep(700, 400);
                Beep(700, 500);
                Beep(700, 900);
                break;
            }

            char guess;
            cout << "Guess a letter or type '!' for a hint: ";
            cin >> guess;
            //guess = tolower(guess);
            if (!isalpha(guess) && guess != '!') {
                cout << "Invalid input. Please enter a letter or type '!' for a hint." << endl;
                continue;
            }

            if (guess == '!' && hintsLeft > 0) {
                provideHint();
                continue;
            }

            if (guessedWord.find(guess) != string::npos) {
                cout << "You've already guessed that letter." << endl;
                continue;
            }

            bool correctGuess = false;
            for (size_t i = 0; i < secretWord.length(); ++i) {
                if (isalpha(secretWord[i])) {
                    if (secretWord[i] == guess) {
                        guessedWord[i] = guess;
                        correctGuess = true;
                    }
                } else {
                    guessedWord[i] = secretWord[i];
                }
            }

            if (!correctGuess) {
                --attemptsLeft;
                cout << "Incorrect guess. " << attemptsLeft << " attempts left." << endl;
                Beep(700, 500);
            }
        }

        if (attemptsLeft == 0) {
            displayHangman();
            cout << "  ____  " << endl;
            cout << " / _  _\\ " << endl;
            cout << "|  0 0 |" << endl;
            cout << " \\ ^  / " << endl;
            cout << "  \\_-__/  " << endl;
            cout << "Hanged" << endl;
            cout << "Sorry, " << playerName << ", you ran out of attempts. The word was: " << secretWord << endl;
        }
        checkHighScore(playerName);
    }

    void provideHint() {
        for (size_t i = 0; i < secretWord.length(); ++i) {
            if (guessedWord[i] == '_') {
                guessedWord[i] = secretWord[i];
                --hintsLeft;
                score -= 30;
                cout << "Hint: " << guessedWord << endl;
                Beep(1000, 300);
                break;
            }
        }
    }
    int calculateScore() {
        return attemptsLeft * 50;
    }

    void displayHangman() {
        int remainingAttempts = attemptsLeft;
        cout << "Category: " << currentCategory << endl;
        cout << "Attempts left: " << remainingAttempts << endl;
        switch (remainingAttempts) {
            case 6:
                cout << "  ________" << endl;
                cout << "  |      |" << endl;
                cout << "         |" << endl;
                cout << "         |" << endl;
                cout << "         |" << endl;
                cout << "         |" << endl;
                cout << "_________|___" << endl;
                break;
            case 5:
                cout << "  ________" << endl;
                cout << "  |      |" << endl;
                cout << "  O      |" << endl;
                cout << "         |" << endl;
                cout << "         |" << endl;
                cout << "         |" << endl;
                cout << "_________|___" << endl;
                break;
            case 4:
                cout << "  ________" << endl;
                cout << "  |      |" << endl;
                cout << "  O      |" << endl;
                cout << "  |      |" << endl;
                cout << "         |" << endl;
                cout << "         |" << endl;
                cout << "_________|___" << endl;
                break;
            case 3:
                cout << "  ________" << endl;
                cout << "  |      |" << endl;
                cout << "  O      |" << endl;
                cout << " /|      |" << endl;
                cout << "         |" << endl;
                cout << "         |" << endl;
                cout << "_________|___" << endl;
                break;
            case 2:
                cout << "  ________" << endl;
                cout << "  |      |" << endl;
                cout << "  O      |" << endl;
                cout << " /|\\    |" << endl;
                cout << "         |" << endl;
                cout << "         |" << endl;
                cout << "_________|___" << endl;
                break;
            case 1:
                cout << "  ________" << endl;
                cout << "  |      |" << endl;
                cout << "  O      |" << endl;
                cout << " /|\\    |" << endl;
                cout << " / \\    |" << endl;
                cout << "         |" << endl;
                cout << "_________|___" << endl;
                break;
            case 0:
                cout << "  ________" << endl;
                cout << "  |      |" << endl;
                cout << "  O      |" << endl;
                cout << " /|\\    |" << endl;
                cout << " / \\    |" << endl;
                cout << "         |" << endl;
                cout << "_________|___" << endl;
                break;
        }
    }

    void displayGuessedWord() {
        cout << "Word to guess: ";
        for (char c : guessedWord) {
            cout << c << " ";
        }
        cout << endl;
    }

    // Add a function to input the player's name
    string getPlayerName() {
        string name;
        cout << "Enter your name: ";
        cin >> name;
        return name;
    }

    void checkHighScore(const string& playerName) {
        if (score > 0) {
            highScores.push_back(make_pair(playerName, score));
        }

        cout << "Your Score: " << score << endl;

        // Sort and display high scores
        sort(highScores.begin(), highScores.end(),
            [](const pair<string, int>& a, const pair<string, int>& b) {
                return a.second > b.second;
            });

        cout << "High Scores:" << endl;
        for (const auto& entry : highScores) {
            cout << entry.first << ": " << entry.second << " points" << endl;
        }

        // Save high scores to a file
        saveHighScores();
    }

    void saveHighScores() {
        ofstream file("high_scores.txt");
        if (file.is_open()) {
            for (const auto& entry : highScores) {
                file << entry.first << " " << entry.second << endl;
            }
            file.close();
        } else {
            cerr << "Error: Unable to open high scores file." << endl;
        }
    }

    void loadHighScores() {
        ifstream file("high_scores.txt");
        if (file.is_open()) {
            string name;
            int score;
            while (file >> name >> score) {
                highScores.push_back(make_pair(name, score));
            }
            file.close();
        }
    }
};

int main() {
    HangmanGame game("E:\\Programming\\C++ project\\hnagman\\Hangman-Game-main\\word.txt", 6);
    game.play();
    return 0;
}
