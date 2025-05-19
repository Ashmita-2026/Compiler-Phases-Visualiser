
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cctype>
#include <unordered_set>

using namespace std;

struct Token {
    string type;
    string value;
};

// Helper functions
bool isValidNumber(const string& s) {
    if (s.empty()) return false;
    for (char c : s) if (!isdigit(c)) return false;
    return true;
}

bool isValidOperator(const string& op) {
    static unordered_set<string> validOps = {"+", "-", "*", "/", "=", ">", "<", "==", ">=", "<=", "!=", "&", "|", "%"};
    return validOps.count(op) > 0;
}

bool isValidKeyword(const string& kw) {
    static unordered_set<string> validKeywords = {"int", "float", "char", "bool", "if", "else", "while", "for"};
    return validKeywords.count(kw) > 0;
}

bool isKeyword(const string& s) {
    static unordered_set<string> keywords = {"int", "float", "char", "bool", "if", "else", "while", "for"};
    return keywords.count(s) > 0;
}

bool isValidIdentifier(const string& id) {
    if (id.empty()) return false;
    if (isKeyword(id)) return false;  // <-- reject if id is keyword
    if (!isalpha(id[0]) && id[0] != '_') return false;
    for (char c : id) {
        if (!isalnum(c) && c != '_') return false;
    }
    return true;
}


// Parser matchers with validation
bool match(const vector<Token>& tokens, int& i, const string& expectedType) {
    if (i < (int)tokens.size() && tokens[i].type == expectedType) {
        if (expectedType == "Identifier" && !isValidIdentifier(tokens[i].value)) return false;
        if (expectedType == "Keyword" && !isValidKeyword(tokens[i].value)) return false;
        i++;
        return true;
    }
    return false;
}

bool matchValue(const vector<Token>& tokens, int& i, const string& expectedValue) {
    if (i < (int)tokens.size() && tokens[i].value == expectedValue) {
        i++;
        return true;
    }
    return false;
}

bool matchNumber(const vector<Token>& tokens, int& i) {
    if (i < (int)tokens.size() && tokens[i].type == "Number" && isValidNumber(tokens[i].value)) {
        i++;
        return true;
    }
    return false;
}

bool matchIdentifierOrNumber(const vector<Token>& tokens, int& i) {
    if (i < (int)tokens.size()) {
        if (tokens[i].type == "Identifier" && isValidIdentifier(tokens[i].value)) {
            i++;
            return true;
        } else if (tokens[i].type == "Number" && isValidNumber(tokens[i].value)) {
            i++;
            return true;
        }
    }
    return false;
}

// Parsers
bool parseAssignment(const vector<Token>& tokens, int& i) {
    int start = i;
    if (match(tokens, i, "Identifier") && matchValue(tokens, i, "=")) {
        if (!matchIdentifierOrNumber(tokens, i)) {
            i = start;
            return false;
        }

        while (i < (int)tokens.size() && tokens[i].type == "Operator") {
            if (!isValidOperator(tokens[i].value)) {
                i = start;
                return false;
            }
            i++;
            if (!matchIdentifierOrNumber(tokens, i)) {
                i = start;
                return false;
            }
        }

        if (!matchValue(tokens, i, ";")) {
            i = start;
            return false;
        }

        return true;
    }

    i = start;
    return false;
}

bool parseDeclaration(const vector<Token>& tokens, int& i) {
    int start = i;
    if (match(tokens, i, "Keyword") &&
        match(tokens, i, "Identifier") &&
        matchValue(tokens, i, "=") &&
        matchNumber(tokens, i) &&
        matchValue(tokens, i, ";"))
        return true;

    i = start;
    return false;
}

bool parseIfStatement(const vector<Token>& tokens, int& i) {
    int start = i;
    if (matchValue(tokens, i, "if") &&
        matchValue(tokens, i, "(") &&
        match(tokens, i, "Identifier") &&
        match(tokens, i, "Operator")) {

        if (!isValidOperator(tokens[i - 1].value)) {
            i = start;
            return false;
        }

        if (!matchIdentifierOrNumber(tokens, i)) {
            i = start;
            return false;
        }

        if (!matchValue(tokens, i, ")") || !matchValue(tokens, i, "{")) {
            i = start;
            return false;
        }

        // Parse inner statements
        while (i < (int)tokens.size() && tokens[i].value != "}") {
            if (parseDeclaration(tokens, i)) continue;
            if (parseAssignment(tokens, i)) continue;
            if (parseIfStatement(tokens, i)) continue;

            // If none match, invalid statement inside if-block
            i = start;
            return false;
        }

        if (!matchValue(tokens, i, "}")) {
            i = start;
            return false;
        }

        return true;
    }

    i = start;
    return false;
}

bool parse(const vector<Token>& tokens) {
    int i = 0;
    while (i < (int)tokens.size()) {
        if (tokens[i].type.empty()) {
            i++;
            continue;
        }

        if (parseDeclaration(tokens, i)) continue;
        if (parseAssignment(tokens, i)) continue;
        if (parseIfStatement(tokens, i)) continue;

        return false; // Unknown or invalid statement
    }
    return true;
}

vector<Token> readTokensFromFile(const string& filename) {
    vector<Token> tokens;
    ifstream infile(filename);
    if (!infile) {
        cerr << "Cannot open " << filename << "\n";
        return tokens;
    }

    string line;
    while (getline(infile, line)) {
        if (line.empty()) continue;

        size_t openBracket = line.find('[');
        size_t closeBracket = line.find(']');
        size_t arrow = line.find("-->");

        if (openBracket != string::npos && closeBracket != string::npos && arrow != string::npos) {
            string type = line.substr(openBracket + 1, closeBracket - openBracket - 1);
            string value = line.substr(arrow + 3);
            while (!value.empty() && isspace(value[0])) value.erase(0, 1);  // trim leading space
            tokens.push_back({type, value});
        }
    }

    return tokens;
}

int main() {
    vector<Token> tokens = readTokensFromFile("tokens.txt");

    cout << "Tokens read from file:\n";
    for (const auto& t : tokens) {
        cout << "[" << t.type << "] --> " << t.value << "\n";
    }

    cout << "\nParsing result: ";
    if (parse(tokens))
        cout << "Syntax is correct.\n";
    else
        cout << "Syntax is incorrect.\n";

    return 0;
}
