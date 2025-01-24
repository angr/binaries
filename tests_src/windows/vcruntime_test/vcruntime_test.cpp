#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <filesystem>
#include <exception>

namespace fs = std::filesystem;

// Function to load configuration
void loadConfig(const std::string& configFile, std::string& difficulty) {
    try {
        std::ifstream config(configFile);
        if (!config.is_open()) {
            throw std::runtime_error("Could not open config file.");
        }
        getline(config, difficulty);
        config.close();
    }
    catch (const std::exception& e) {
        std::cerr << "Error loading config: " << e.what() << std::endl;
    }
}

// Function to load a level
std::vector<std::string> loadLevel(const std::string& levelFile) {
    std::vector<std::string> board;
    try {
        std::ifstream level(levelFile);
        if (!level.is_open()) {
            throw std::runtime_error("Could not open level file.");
        }

        std::string line;
        while (getline(level, line)) {
            board.push_back(line);
        }
        level.close();
    }
    catch (const std::exception& e) {
        std::cerr << "Error loading level: " << e.what() << std::endl;
    }
    return board;
}

// Function to display the menu
void displayMenu() {
    std::cout << "\nSudoku Game\n";
    std::cout << "1. Start New Game" << std::endl;
    std::cout << "2. Load Level" << std::endl;
    std::cout << "3. Exit" << std::endl;
}

// Function to display the Sudoku board
void displayBoard(const std::vector<std::string>& board) {
    for (const auto& row : board) {
        std::cout << row << std::endl;
    }
}

int main() {
    std::string configFile = "config.txt";
    std::string levelsDirectory = "levels";
    std::string difficulty;

    // Load configuration
    loadConfig(configFile, difficulty);

    std::cout << "Loaded difficulty: " << difficulty << std::endl;

    int choice = 0;
    while (true) {
        displayMenu();
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        if (choice == 1) {
            std::cout << "Starting a new game...\n";
        }
        else if (choice == 2) {
            std::cout << "Available levels:\n";

            // List levels
            try {
                if (!fs::exists(levelsDirectory)) {
                    throw std::runtime_error("Levels directory does not exist.");
                }

                int index = 1;
                for (const auto& entry : fs::directory_iterator(levelsDirectory)) {
                    std::cout << index++ << ". " << entry.path().filename() << std::endl;
                }

                int levelChoice;
                std::cout << "Select a level: ";
                std::cin >> levelChoice;

                index = 1;
                for (const auto& entry : fs::directory_iterator(levelsDirectory)) {
                    if (index == levelChoice) {
                        auto board = loadLevel(entry.path().string());
                        std::cout << "Loaded level: " << entry.path().filename() << std::endl;
                        displayBoard(board);
                        break;
                    }
                    index++;
                }

            }
            catch (const std::exception& e) {
                std::cerr << "Error: " << e.what() << std::endl;
            }

        }
        else if (choice == 3) {
            std::cout << "Exiting...\n";
            break;
        }
        else {
            std::cout << "Invalid choice. Please try again.\n";
        }
    }

    return 0;
}
