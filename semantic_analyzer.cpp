#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <string>

struct Token {
    std::string type;
    std::string value;
};

class SemanticAnalyzer {
    std::unordered_map<std::string, std::string> symbolTable; // varName -> type
    std::vector<Token> tokens;
    int pos = 0;

public:
    SemanticAnalyzer(const std::vector<Token>& tokens) : tokens(tokens) {}

    void analyze() {
        while (pos < tokens.size()) {
            if (tokens[pos].type == "Keyword" && tokens[pos].value == "int") {
                parseDeclaration();
            } else if (tokens[pos].type == "Identifier") {
                parseAssignmentOrExpression();
            } else if (tokens[pos].type == "Keyword" && tokens[pos].value == "if") {
                parseIfStatement();
            } else {
                // Skip or error unknown statement start
                std::cerr << "Error: Unexpected token " << tokens[pos].value << " at position " << pos << "\n";
                pos++;
            }
        }
        std::cout << "Semantic analysis completed.\n";
    }

private:
    void parseDeclaration() {
        // Format: int <Identifier> = <Number> ; 
        pos++; // skip 'int'
        if (!expect("Identifier")) {
            error("Expected identifier after int");
            return;
        }
        std::string varName = tokens[pos].value;
        pos++;
        if (!expect("Operator") || tokens[pos].value != "=") {
            error("Expected '=' after variable name");
            return;
        }
        pos++; // skip '='
        if (!expect("Number")) {
            error("Expected number after '='");
            return;
        }
        pos++; // skip number
        if (!expect("Operator") || tokens[pos].value != ";") {
            error("Expected ';' at end of declaration");
            return;
        }
        pos++; // skip ';'

        // Insert variable in symbol table
        if (symbolTable.find(varName) != symbolTable.end()) {
            std::cerr << "Semantic error: Variable '" << varName << "' already declared.\n";
        } else {
            symbolTable[varName] = "int";
            std::cout << "Declared variable '" << varName << "' of type int.\n";
        }
    }

    void parseAssignmentOrExpression() {
        // Format: <Identifier> = <Number or Expression> ;
        std::string varName = tokens[pos].value;
        if (symbolTable.find(varName) == symbolTable.end()) {
            std::cerr << "Semantic error: Variable '" << varName << "' used before declaration.\n";
            // Skip to next ';' to continue analysis
            skipToSemicolon();
            return;
        }
        pos++; // skip Identifier

        if (!expect("Operator") || tokens[pos].value != "=") {
            error("Expected '=' in assignment");
            return;
        }
        pos++; // skip '='

        parseExpression();

        if (!expect("Operator") || tokens[pos].value != ";") {
            error("Expected ';' at end of assignment");
            return;
        }
        pos++; // skip ';'
        std::cout << "Assignment to variable '" << varName << "' is semantically valid.\n";
    }

    void parseIfStatement() {
        // Format: if ( <condition> ) { <statements> }
        pos++; // skip 'if'
        if (!expect("Operator") || tokens[pos].value != "(") {
            error("Expected '(' after if");
            return;
        }
        pos++; // skip '('

        parseCondition();

        if (!expect("Operator") || tokens[pos].value != ")") {
            error("Expected ')' after condition");
            return;
        }
        pos++; // skip ')'

        if (!expect("Operator") || tokens[pos].value != "{") {
            error("Expected '{' to start if block");
            return;
        }
        pos++; // skip '{'

        // parse statements inside if block (simple: just assignments or declarations)
        while (pos < tokens.size() && !(tokens[pos].type == "Operator" && tokens[pos].value == "}")) {
            if (tokens[pos].type == "Keyword" && tokens[pos].value == "int") {
                parseDeclaration();
            } else if (tokens[pos].type == "Identifier") {
                parseAssignmentOrExpression();
            } else {
                error("Unexpected token inside if block");
                pos++;
            }
        }
        if (!expect("Operator") || tokens[pos].value != "}") {
            error("Expected '}' at end of if block");
            return;
        }
        pos++; // skip '}'
        std::cout << "If statement semantic check passed.\n";
    }

    void parseCondition() {
        // Simple condition: <Identifier> <relational_op> <Number>
        if (!expect("Identifier")) {
            error("Expected identifier in condition");
            return;
        }
        std::string varName = tokens[pos].value;
        if (symbolTable.find(varName) == symbolTable.end()) {
            std::cerr << "Semantic error: Variable '" << varName << "' used in condition before declaration.\n";
        }
        pos++; // skip Identifier

        if (!expect("Operator")) {
            error("Expected operator in condition");
            return;
        }
        std::string op = tokens[pos].value;
        if (op != ">" && op != "<" && op != ">=" && op != "<=" && op != "==" && op != "!=") {
            error("Invalid relational operator in condition");
            return;
        }
        pos++; // skip operator

        if (!expect("Number")) {
            error("Expected number in condition");
            return;
        }
        pos++; // skip number
    }

    void parseExpression() {
        // For simplicity, accept Number or <Number> + <Number>
        if (!expect("Number")) {
            error("Expected number in expression");
            return;
        }
        pos++; // skip number

        if (pos < tokens.size() && tokens[pos].type == "Operator" && tokens[pos].value == "+") {
            pos++; // skip '+'
            if (!expect("Number")) {
                error("Expected number after '+' in expression");
                return;
            }
            pos++; // skip number
        }
    }

    bool expect(const std::string& expectedType) {
        if (pos >= tokens.size()) return false;
        return tokens[pos].type == expectedType;
    }

    void error(const std::string& msg) {
        std::cerr << "Semantic error at token index " << pos << ": " << msg << "\n";
    }

    void skipToSemicolon() {
        while (pos < tokens.size() && !(tokens[pos].type == "Operator" && tokens[pos].value == ";")) {
            pos++;
        }
        if (pos < tokens.size()) pos++; // skip ';'
    }
};

std::vector<Token> readTokensFromFile(const std::string& filename) {
    std::vector<Token> tokens;
    std::ifstream infile(filename);
    std::string line;

    while (std::getline(infile, line)) {
        std::istringstream iss(line);
        Token token;
        if (iss >> token.type >> token.value) {
            tokens.push_back(token);
        }
    }
    return tokens;
}

int main() {
    std::vector<Token> tokens = readTokensFromFile("tokensSemantic.txt");

    SemanticAnalyzer analyzer(tokens);
    analyzer.analyze();

    return 0;
}
