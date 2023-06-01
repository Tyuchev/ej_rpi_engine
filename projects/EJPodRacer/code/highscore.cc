#include "config.h"
#include "highscore.h"
#include <fstream>
#include <iostream>
#include <string>
//#include <ios>

HighscoreSystem::HighscoreSystem(char* filePath) : filePath(filePath) {
    score = Read();
}

int HighscoreSystem::Read() {
    std::string line;
    std::ifstream fs(filePath);
    if (!fs.is_open())
        return 0;
    std::getline(fs, line);
    fs.close();
    return std::stoi(line);
}

void HighscoreSystem::Write() {
    std::ofstream fs;
    fs.open(filePath, std::ios::out | std::ios::trunc);
    fs << std::to_string(score);
    fs.close();
}
