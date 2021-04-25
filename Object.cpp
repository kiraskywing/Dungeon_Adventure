#include "Object.h"

int inputFilter(const int n) {
    int i;
    char c;
    do {
        cin >> c;
        cin.clear();
        cin.ignore(INT_MAX, '\n');
        if (isdigit(c)) i = c - '0';
        if (isalpha(c)) i = tolower(c) - 'a';
        if (i < 0 || i >= n) cout << "Wrong input. Please enter again: ";
    } while (i < 0 || i >= n);
    return i;
}