

class HighscoreSystem {
public:
    HighscoreSystem(char* filePath);
    int score;

    // Return the value read from filePath. Called in constructor to set score.
    int Read();
    // Write value of score to filePath
    void Write();

private:
    char* filePath;
};
