#include "Object.h"

int inputOptimizer(const int n, string mode) {
    int i;
    char c;
    do {
        if (mode == "pause") {
            cout << endl << "Press Enter to continue ... ";
            getline(cin, mode);
            return 0;
        }
        cin >> c;
        cin.clear();
        cin.ignore(INT_MAX, '\n');
        
        if (isdigit(c)) i = c - '0';
        if (isalpha(c)) i = tolower(c) - 'a';
        if (i < 0 || i >= n) cout << "Wrong input. Please enter again: ";
    } while (i < 0 || i >= n);
    return i;
}