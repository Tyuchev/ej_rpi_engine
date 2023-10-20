#include "config.h"
#include "highscore.h"
#include <fstream>
#include <iostream>
#include <string>
//#include <ios>

HighscoreSystem::HighscoreSystem() {
    filePath = nullptr;
    previousHigh = 0;
    currentScore = 0;
}

void HighscoreSystem::Load(const char* filePath) {
    this->filePath = filePath;
    previousHigh = Read();
    currentScore = 0;
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

bool HighscoreSystem::IsHigh() {
    return (currentScore > previousHigh);
}

bool HighscoreSystem::Save() {
    if (IsHigh()) {
        Write();
        return true;
    }
    return false;
}

void HighscoreSystem::Write() {
    std::ofstream fs;
    fs.open(filePath, std::ios::out | std::ios::trunc);
    fs << std::to_string(currentScore);
    fs.close();
}
