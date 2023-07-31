// Boggle solver using a trie
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

#define N 26 // alphabet length

struct Node {
    struct Node *children[N];
    bool EOW; // end of word
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

bool inTrie(struct Node *root, string word) {
    struct Node *cur = root;

    for (int i = 0; i < word.length(); ++i) {
        int idx = word[i] - 'A';
        if (!cur->children[idx])
            return false;

        cur = cur->children[idx];
    }

    return cur->EOW;
}

void loadWords(Node *root, string path) {
    ifstream words = ifstream(path);
    string tmp;

    while (getline(words, tmp)) {
        insert(root, tmp);
    }

    words.close();
}

// parsing board as follows:
// 1 2 3 4 5
// 6 7 8 9 10
// ...
void parseInput(char board[5][5], string input) {
    int idx = 0;
    for (char &c : input) {
        board[idx / 5][idx % 5] = c;
        ++idx;
    }
}

int main() {
    struct Node *root = getNode();

    cout << "Loading wordlist...\n";
    loadWords(root, "words.txt");
    cout << "Wordlist loaded.\nEnter board state: ";

    // cout << inTrie(root, "WHERE") << endl;
    // cout << inTrie(root, "WWWWW") << endl;

    string input;
    cin >> input;
    if (input.length() != 25) {
        cout << "Invalid input";
        return 1;
    }

    char board[5][5];
    parseInput(board, input);

    return 0;
}