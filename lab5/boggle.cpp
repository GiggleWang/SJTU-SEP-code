#include "lexicon.h"
#include <iostream>
#include <vector>
#include <cctype>
#include <algorithm>

using namespace std;

class Point {
public:
    int x;
    int y;
    char letter;
    vector<Point *> familyPoint;
    bool used = true;

    Point(int i, int j);

};

Point::Point(int i, int j) {
    x = i;
    y = j;
}

class Boggle {
public:
    void setData();

    void printSituation();

    void getWord();

    void computerFind();

    void setFamilyPoint(int i, int j);

    void autoWord(int i, int j, string str);

    string changeToSmall(string str);

    string changeToBig(string str);

    bool computerAlreadyFound(string str);//判断是否已经被机器找到

    bool isWord(string str);//判断是否包含在英语词典中

    bool canBeFound(string str);//判断是否可以从表中找到

    bool AlreadyFound(string str);//判断是否已经被找到

    bool isLongEnough(string str);//判断是否长度满足要求

    void computerTime();

private:
    int width;
    vector<vector<Point *>> data;
    vector<string> myWord;
    vector<string> computerWord;
    int score = 0;
    int computerScore = 0;
    Lexicon *lex;
};

void Boggle::setData() {
    lex = new Lexicon("./EnglishWords.txt");
    cin >> this->width;
    this->data.resize(width, vector<Point *>(width));
    cin.get();
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < width; j++) {
            data[i][j] = new Point(i, j);
            data[i][j]->letter = cin.get();
        }
        cin.get();
    }
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < width; j++) {
            setFamilyPoint(i, j);
        }
    }
}

void Boggle::printSituation() {
    for (int i = 0; i < this->width; i++) {
        for (int j = 0; j < this->width; j++) {
            cout << this->data[i][j]->letter;
        }
        cout << endl;
    }
    cout << "Your Score: " << this->score << endl;
    cout << "Your words: ";
    for (int i = 0; i < myWord.size(); i++) {
        cout << myWord[i];
        if (i != myWord.size() - 1)
            cout << " ";
    }
    cout << endl;
}

bool Boggle::isWord(std::string str) {
    return lex->contains(str);
}

bool Boggle::AlreadyFound(std::string str) {
    for (int i = 0; i < this->myWord.size(); i++) {
        if (myWord[i] == str) {
            return false;
        }
    }
    return true;
}

bool Boggle::isLongEnough(std::string str) {
    if (str.length() < 4)
        return false;
    return true;
}

bool Boggle::canBeFound(std::string str) {
    for (int i = 0; i < this->computerWord.size(); i++) {
        if (computerWord[i] == changeToBig(str))
            return true;
    }
    return false;
}

void Boggle::computerTime() {
    sort(computerWord.begin(), computerWord.end());
    for (int i = 0; i < computerWord.size(); i++) {
        if (AlreadyFound(changeToSmall(computerWord[i])))
            computerScore += (computerWord[i].length() - 3);
    }
    cout << "Computer Score: " << computerScore << endl;
    cout << "Computer Words: ";
    for (int i = 0; i < computerWord.size(); i++) {
        if (AlreadyFound(changeToSmall(computerWord[i])))
            cout << computerWord[i] << " ";
    }
}

void Boggle::getWord() {
    computerFind();
    while (1) {
        printSituation();
        string str;
        getline(cin, str);
        if (str == "???") {
            computerTime();
            break;
        }
        if (!isLongEnough(str)) {
            cout << str << " is too short." << endl;
            continue;
        }
        if (!isWord(this->changeToSmall(str))) {
            cout << str << " is not a word." << endl;
            continue;
        }
        if (!canBeFound(str)) {
            cout << str << " is not on board." << endl;
            continue;
        }
        if (!AlreadyFound(this->changeToSmall(str))) {
            cout << str << " is already found." << endl;
            continue;
        }
        this->myWord.push_back(this->changeToSmall(str));
        this->score += (str.length() - 3);


    }
}

void Boggle::computerFind() {
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < width; j++) {
            string str;
            autoWord(i, j, str);
        }
    }
//    for (int i = 0; i < computerWord.size(); i++)
//        cout << computerWord[i] << " ";
}

void Boggle::autoWord(int i, int j, std::string str) {
    if (data[i][j]->used == false)
        return;
//    cout<<i<<" "<<j<<" "<<str<<"     ";
    str.push_back(data[i][j]->letter);
//    cout<<i<<" "<<j<<" "<<str<<endl;
    data[i][j]->used = false;
    if (str.size() >= 4 && this->computerAlreadyFound(changeToBig(str)) == true) {
        if (lex->contains(str))
            computerWord.push_back(changeToBig(str));
    }
    if (lex->containsPrefix(str)) {
        for (int k = 0; k < data[i][j]->familyPoint.size(); k++) {
            autoWord(data[i][j]->familyPoint[k]->x, data[i][j]->familyPoint[k]->y, str);
        }
    }
    data[i][j]->used = true;
    str.pop_back();
}

void Boggle::setFamilyPoint(int i, int j) {
    if (i - 1 >= 0 && i - 1 < width) {
        if (j - 1 >= 0 && j - 1 < width)
            data[i][j]->familyPoint.push_back(data[i - 1][j - 1]);
        if (j >= 0 && j < width)
            data[i][j]->familyPoint.push_back(data[i - 1][j]);
        if (j + 1 >= 0 && j + 1 < width)
            data[i][j]->familyPoint.push_back(data[i - 1][j + 1]);
    }
    if (i >= 0 && i < width) {
        if (j - 1 >= 0 && j - 1 < width)
            data[i][j]->familyPoint.push_back(data[i][j - 1]);
        if (j + 1 >= 0 && j + 1 < width)
            data[i][j]->familyPoint.push_back(data[i][j + 1]);
    }
    if (i + 1 >= 0 && i + 1 < width) {
        if (j - 1 >= 0 && j - 1 < width)
            data[i][j]->familyPoint.push_back(data[i + 1][j - 1]);
        if (j >= 0 && j < width)
            data[i][j]->familyPoint.push_back(data[i + 1][j]);
        if (j + 1 >= 0 && j + 1 < width)
            data[i][j]->familyPoint.push_back(data[i + 1][j + 1]);
    }
//    cout<<data[i][j]->familyPoint.size();
}

string Boggle::changeToSmall(string str) {
    string tmp;
    for (int i = 0; i < str.length(); i++) {
        tmp.push_back(tolower(str[i]));
    }
    return tmp;
}

string Boggle::changeToBig(string str) {
    string tmp;
    for (int i = 0; i < str.length(); i++) {
        tmp.push_back(toupper(str[i]));
    }
    return tmp;
}

bool Boggle::computerAlreadyFound(std::string str) {
    for (int i = 0; i < computerWord.size(); i++) {
        if (computerWord[i] == str) {
            return false;
        }
    }
    return true;
}

int main() {
    // TODO
    Boggle boggle;
    boggle.setData();
    boggle.getWord();
    return 0;
}
