#include <iostream>
#include <iomanip>
#include <string>
#include <windows.h> // Required for SetConsoleOutputCP

const int SUCCESS = 1;
const int FAILED = 0;

const char* cursor;
char input[64];

// Function declarations
int E();
int Edash();
int T();
int Tdash();
int F();

void printStep(const char* rule) {
    std::cout << std::left << std::setw(15) << cursor << " " << rule << std::endl;
}

int main() {
    // Set the console output to UTF-8 to support printing epsilon
    SetConsoleOutputCP(CP_UTF8);

    std::cout << "Enter the string to parse: ";
    std::cin >> input;
    cursor = input;

    std::cout << "\nInput          Action\n";
    std::cout << "--------------------------------\n";

    if (E() && *cursor == '\0') {
        std::cout << "--------------------------------\n";
        std::cout << "String is successfully parsed.\n";
    } else {
        std::cout << "--------------------------------\n";
        std::cout << "Error in parsing string.\n";
    }

    return 0;
}

// Grammar rule: E -> T E'
int E() {
    printStep(u8"E -> T E'");
    if (T()) {
        if (Edash()) {
            return SUCCESS;
        } else {
            return FAILED;
        }
    } else {
        return FAILED;
    }
}

// Grammar rule: E' -> + T E' | ε
int Edash() {
    if (*cursor == '+') {
        printStep(u8"E' -> + T E'");
        cursor++;
        if (T()) {
            if (Edash()) {
                return SUCCESS;
            } else {
                return FAILED;
            }
        } else {
            return FAILED;
        }
    } else {
        printStep(u8"E' -> ε");
        return SUCCESS;
    }
}

// Grammar rule: T -> F T'
int T() {
    printStep(u8"T -> F T'");
    if (F()) {
        if (Tdash()) {
            return SUCCESS;
        } else {
            return FAILED;
        }
    } else {
        return FAILED;
    }
}

// Grammar rule: T' -> * F T' | ε
int Tdash() {
    if (*cursor == '*') {
        printStep(u8"T' -> * F T'");
        cursor++;
        if (F()) {
            if (Tdash()) {
                return SUCCESS;
            } else {
                return FAILED;
            }
        } else {
            return FAILED;
        }
    } else {
        printStep(u8"T' -> ε");
        return SUCCESS;
    }
}

// Grammar rule: F -> ( E ) | i
int F() {
    if (*cursor == '(') {
        printStep(u8"F -> ( E )");
        cursor++;
        if (E()) {
            if (*cursor == ')') {
                cursor++;
                return SUCCESS;
            } else {
                return FAILED;
            }
        } else {
            return FAILED;
        }
    } else if (*cursor == 'i') {
        printStep(u8"F -> i");
        cursor++;
        return SUCCESS;
    } else {
        return FAILED;
    }
}


