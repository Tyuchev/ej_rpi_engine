

class HighscoreSystem {
public:
    HighscoreSystem(const char* filePath);
    int currentScore;
    int previousHigh;

    // Write value of current score to filePath.
    void Write();
    bool IsHigh();
    // Write value of current score to filePath if it's high enough. Returns true if it was.
    bool Save();

private:
    // Return the value read from filePath. Private because called in constructor to set score.
    int Read();

    const char* filePath;
};