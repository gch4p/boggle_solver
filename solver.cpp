// Boggle solver using a trie
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

#define N 26 // alphabet length

struct Node {
    struct Node *children[N];
    bool EOW = false; // end of word
};

struct Node *getNode() {
    struct Node *newNode = new Node;
    newNode->EOW = false;

    for (int i = 0; i < N; ++i)
        newNode->children[i] = nullptr;

    return newNode;
}

void insert(struct Node *root, string word) {
    struct Node *cur = root;

    for (int i = 0; i < word.length(); ++i) {
        int idx = word[i] - 'A';

        if (!cur->children[idx])
            cur->children[idx] = getNode();

        cur = cur->children[idx];
    }

    cur->EOW = true;
}

bool isWord(struct Node *root, string word) {
    struct Node *cur = root;

    for (int i = 0; i < word.length(); ++i) {
        int idx = word[i] - 'A';
        if (!cur->children[idx])
            return false;

        cur = cur->children[idx];
    }

    return cur->EOW;
}

bool canMakeWord(struct Node *root, string word) {
    struct Node *cur = root;

    for (int i = 0; i < word.length(); ++i) {
        int idx = word[i] - 'A';
        if (!cur->children[idx])
            return false;

        cur = cur->children[idx];
    }

    return true;
}

void loadWords(Node *root, string path) {
    ifstream words = ifstream(path);
    string tmp;

    while (getline(words, tmp)) {
        insert(root, tmp);
    }

    words.close();
}

// void printBoard(char board[5][5]) {
//     int idx = 0;
//     while (idx < 25) {
//         cout << board[idx / 5][idx % 5];
//         ++idx;
//     }
// }

class solver {
public:
    struct Node *root;

    void load(string path);
    void solve(string input);
    void parseInput(string input);
    void getNeighbors(int idx);

private:
    char board[25];
    vector<int> neighbors;
    vector<string> foundWords;
    void showNeighbors();
    void showWords(int count);
    void findWords(vector<int> path,string word);
};

void solver::load(string path) {
    root = getNode();
    loadWords(root, path);
}

bool lengthCompare(string a, string b) {
    return a.length() > b.length();
}

void solver::solve(string input) {
    parseInput(input);

    showNeighbors();
    for (int idx = 0; idx < 25; ++idx) {
        findWords({idx},"");
    }

    sort(foundWords.begin(), foundWords.end(), lengthCompare);
    showWords(10);
}

void solver::showWords(int count=10) {
    count = min(count, (int) foundWords.size());

    cout << "Displaying top words:" << endl;
    for (int i = 0; i < count;++i) {
        cout << foundWords[i] << endl;
    }
}

bool inPath(vector<int> vec, int key) {
    for (int &i : vec) {
        if (i == key)
            return true;
    }
    return false;
}

void solver::findWords(vector<int> path,string word) {
    int idx = path.back();
    word += (board[idx]);

    if (!canMakeWord(root,word))
        return;
    
    if (isWord(root,word))
        foundWords.push_back(word);

    getNeighbors(idx);
    vector<int> tmp = neighbors;
    for (const int &i : tmp) {
        if (inPath(path,i))
            continue;
        path.push_back(i);
        findWords(path, word);
    }
}

// parsing board as follows:
// 0 1 2 3 4
// 5 6 7 8 9
// 10 11 12 13 14
// 15 16 17 18 19
// 20 21 22 23 24
// ...
void solver::parseInput(string input) {
    // int idx = 0;
    // for (char &c : input) {
    //     board[idx / 5][idx % 5] = c;
    //     ++idx;
    // }
    for (int idx = 0; idx < 25;++idx) {
        board[idx] = input[idx];
    }
}

// hardcoding a map for all 25 indices probably best solution here
// this method is not good.
void solver::getNeighbors(int idx) {
    bool TOP = idx < 5;
    bool LEFT = ((idx % 5) == 0);
    bool RIGHT = ((idx % 5) == 4);
    bool BOT = idx > 19;
    neighbors.clear();

    // center idx, no problems
    if (!TOP && !LEFT && !RIGHT && !BOT) {
        neighbors = {idx - 6, idx - 5, idx - 4, idx - 1, idx + 1, idx + 4, idx + 5, idx + 6};
        return;
    }

    // corners
    if ((TOP + LEFT + RIGHT + BOT) == 2) {
        if (idx == 0)
            neighbors = {1, 5, 6};
        else if (idx == 4)
            neighbors = {3, 8, 9};
        else if (idx == 20)
            neighbors = {15, 16, 21};
        else if (idx == 24)
            neighbors = {18, 19, 23};

        return;
    }

    if (TOP)
        neighbors = {idx - 1, idx + 1, idx + 4, idx + 5, idx + 6};
    else if (LEFT)
        neighbors = {idx - 5, idx - 4, idx + 1, idx + 5, idx + 6};
    else if (RIGHT)
        neighbors = {idx - 5, idx - 6, idx - 1, idx + 5, idx + 4};
    else if (BOT)
        neighbors = {idx - 1, idx + 1, idx - 4, idx - 5, idx - 6};
}

// debug
void solver::showNeighbors() {
    for (int idx = 0; idx < 25; ++idx) {
        getNeighbors(idx);
        cout << "Neighbors for index " << idx << ": ";
        for (int i : neighbors) {
            cout << i;
        }
        cout << endl;
    }
}

int main() {
    solver solver;

    cout << "Loading wordlist...\n";
    solver.load("words.txt");
    cout << "Wordlist loaded.\nEnter board state: ";

    // cout << inTrie(root, "WHERE") << endl;
    // cout << inTrie(root, "WWWWW") << endl;

    string input;
    cin >> input;
    if (input.length() != 25) {
        cout << "Invalid input";
        return 1;
    }

    solver.solve(input);

    return 0;
}