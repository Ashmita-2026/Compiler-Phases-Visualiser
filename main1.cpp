#include <iostream>
#include <fstream>
#include <unordered_set>
#include <string>
using namespace std;

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Usage: lexer <filename>" << endl;
        return 1;
    }

    ifstream file(argv[1]);
    if (!file) {
        cerr << "Cannot open file.\n";
        return 1;
    }

    unordered_set<string> keywords = {"int", "float", "char", "return", "if", "else", "while", "for", "double", "void"};
    unordered_set<string> operators = {"+", "-", "*", "/", "=", ">", "<", ">=", "<=", "==", "!=", "!"};
    unordered_set<char> separators = {';', '(', ')', '{', '}', '[', ']', ','};

    string code((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    int i = 0, n = code.length();

    while (i < n) {
        if (isspace(code[i])) {
            i++;
            continue;
        }
        if (isalpha(code[i]) || code[i] == '_') {
            string word;
            while (isalnum(code[i]) || code[i] == '_') {
                word += code[i++];
            }
            cout << (keywords.count(word) ? "[Keyword]  -->  " : "[Identifier] --> ") << word << endl;
        } else if (isdigit(code[i])) {
            string num;
            while (isdigit(code[i]) || code[i] == '.') {
                num += code[i++];
            }
            cout << "[Number]    --> " << num << endl;
        } else {
            string op;
            op += code[i];
            if (i + 1 < n && (op + code[i + 1] == ">=" || op + code[i + 1] == "<=" || op + code[i + 1] == "==")) {
                op += code[i + 1];
                i += 2;
            } else {
                i++;
            }

            if (operators.count(op)) {
                cout << "[Operator]  --> " << op << endl;
            } else if (separators.count(op[0])) {
                cout << "[Separator] --> " << op << endl;
            } else {
                cout << "[Unknown]   --> " << op << endl;
            }
        }
    }

    return 0;
}
