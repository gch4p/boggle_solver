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

int main() {
    struct Node *root = getNode();

    cout << "Loading wordlist...\n";
    loadWords(root, "words.txt");
    cout << "Wordlist loaded.\nEnter board state: ";

    // cout << inTrie(root, "WHERE") << endl;
    // cout << inTrie(root, "WWWWW") << endl;

    return 0;
}