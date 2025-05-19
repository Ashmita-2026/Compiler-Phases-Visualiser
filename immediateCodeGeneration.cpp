/*  if - else
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <unordered_map>

struct Token {
    std::string type;
    std::string value;
};

class ImmediateCodeGenerator {
    std::vector<Token> tokens;
    int pos = 0;
    int tempCount = 0;
    int labelCount = 0;

    std::unordered_map<std::string, std::string> symbolTable;

public:
    ImmediateCodeGenerator(const std::vector<Token>& tokens) : tokens(tokens) {}

    void generate() {
        while (pos < tokens.size()) {
            if (match("Keyword", "int")) {
                parseDeclaration();
            } else if (match("Identifier")) {
                parseAssignment();
            } else if (match("Keyword", "if")) {
                parseIfStatement();
            } else {
                error("Unexpected token at global scope");
                pos++;
            }
        }
    }

private:
    bool match(const std::string& type, const std::string& value = "") {
        if (pos >= tokens.size()) return false;
        if (tokens[pos].type != type) return false;
        if (!value.empty() && tokens[pos].value != value) return false;
        return true;
    }

    void error(const std::string& msg) {
        std::cerr << "Error at token " << pos << ": " << msg << "\n";
    }

    std::string newTemp() {
        return "t" + std::to_string(tempCount++);
    }

    std::string newLabel() {
        return "L" + std::to_string(labelCount++);
    }

    std::string expectIdentifier() {
        if (!match("Identifier")) {
            error("Expected identifier");
            return "";
        }
        return tokens[pos++].value;
    }

    std::string expectNumber() {
        if (!match("Number")) {
            error("Expected number");
            return "";
        }
        return tokens[pos++].value;
    }

    std::string expectRelationalOperator() {
        if (!match("Operator")) {
            error("Expected relational operator");
            return "";
        }
        std::string op = tokens[pos].value;
        if (op == "==" || op == "!=" || op == "<" || op == "<=" || op == ">" || op == ">=") {
            pos++;
            return op;
        } else {
            error("Invalid relational operator: " + op);
            return "";
        }
    }

    void expectOperator(const std::string& op) {
        if (!match("Operator", op)) {
            error("Expected operator '" + op + "'");
        } else {
            pos++;
        }
    }

    std::string parseExpression() {
        std::string left;
        if (match("Number") || match("Identifier")) {
            left = tokens[pos++].value;
        } else {
            error("Expected operand");
            return "";
        }

        if (match("Operator", "+") || match("Operator", "-") || match("Operator", "*") || match("Operator", "/")) {
            std::string op = tokens[pos++].value;
            std::string right = parseExpression();
            std::string temp = newTemp();
            std::cout << temp << " = " << left << " " << op << " " << right << "\n";
            return temp;
        }

        return left;
    }

    void parseDeclaration() {
        pos++; // skip 'int'
        std::string varName = expectIdentifier();

        if (symbolTable.find(varName) != symbolTable.end()) {
            std::cerr << "Error: Variable '" << varName << "' already declared.\n";
        } else {
            symbolTable[varName] = "int";
        }

        if (match("Operator", "=")) {
            pos++; // skip '='
            std::string rhs = parseExpression();
            std::cout << varName << " = " << rhs << "\n";
        }

        expectOperator(";");
    }

    void parseAssignment() {
        std::string varName = expectIdentifier();

        if (symbolTable.find(varName) == symbolTable.end()) {
            std::cerr << "Error: Variable '" << varName << "' used before declaration.\n";
        }

        expectOperator("=");
        std::string rhs = parseExpression();
        expectOperator(";");
        std::cout << varName << " = " << rhs << "\n";
    }

    void parseIfStatement() {
        pos++; // skip 'if'
        expectOperator("(");

        std::string condVar = expectIdentifier();
        std::string relOp = expectRelationalOperator();
        std::string condVal = expectNumber();

        expectOperator(")");
        expectOperator("{");

        std::string labelTrue = newLabel();
        std::string labelFalse = newLabel();
        std::string labelNext = newLabel();

        std::cout << "if " << condVar << " " << relOp << " " << condVal << " go to " << labelTrue << "\n";
        std::cout << "       go to " << labelFalse << "\n";

        std::cout << labelTrue << ": ";
        while (!match("Operator", "}")) {
            if (match("Keyword", "int")) {
                parseDeclaration();
            } else if (match("Identifier")) {
                parseAssignment();
            } else {
                error("Unexpected token inside if block");
                pos++;
            }
        }
        expectOperator("}");
        std::cout << "       go to " << labelNext << "\n";

        std::cout << labelFalse << ": ";

        // Check for optional else
        if (match("Keyword", "else")) {
            pos++; // skip 'else'
            expectOperator("{");

            while (!match("Operator", "}")) {
                if (match("Keyword", "int")) {
                    parseDeclaration();
                } else if (match("Identifier")) {
                    parseAssignment();
                } else {
                    error("Unexpected token inside else block");
                    pos++;
                }
            }
            expectOperator("}");
        }

        std::cout << labelNext << ":\n";
    }
};

// Token reading from file
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
    std::vector<Token> tokens = readTokensFromFile("tokensImmediateCodeGeneration.txt");
    ImmediateCodeGenerator generator(tokens);
    generator.generate();
    return 0;
}

*/

/* for loop
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <unordered_map>

struct Token {
    std::string type;
    std::string value;
};

class ImmediateCodeGenerator {
    std::vector<Token> tokens;
    int pos = 0;
    int tempCount = 0;
    int labelCount = 0;

public:
    std::unordered_map<std::string, std::string> symbolTable;

    ImmediateCodeGenerator(const std::vector<Token>& tokens) : tokens(tokens) {}

    void generate() {
        while (pos < tokens.size()) {
            if (match("Keyword", "int")) {
                parseDeclaration();
            } else if (match("Identifier")) {
                parseAssignment();
            } else if (match("Keyword", "if")) {
                parseIfStatement();
            } else if (match("Keyword", "for")) {
                parseForLoop();
            } else {
                error("Unexpected token at global scope");
                pos++;
            }
        }
    }

private:
    bool match(const std::string& type, const std::string& value = "") {
        if (pos >= tokens.size()) return false;
        if (tokens[pos].type != type) return false;
        if (!value.empty() && tokens[pos].value != value) return false;
        return true;
    }

    void error(const std::string& msg) {
        std::cerr << "Error at token " << pos << ": " << msg << "\n";
    }

    std::string newTemp() {
        return "t" + std::to_string(tempCount++);
    }

    std::string newLabel() {
        return "L" + std::to_string(labelCount++);
    }

    std::string expectIdentifier() {
        if (!match("Identifier")) {
            error("Expected identifier");
            return "";
        }
        return tokens[pos++].value;
    }

    std::string expectNumber() {
        if (!match("Number")) {
            error("Expected number");
            return "";
        }
        return tokens[pos++].value;
    }

    std::string expectRelationalOperator() {
        if (!match("Operator")) {
            error("Expected relational operator");
            return "";
        }
        std::string op = tokens[pos].value;
        if (op == "==" || op == "!=" || op == "<" || op == "<=" || op == ">" || op == ">=") {
            pos++;
            return op;
        } else {
            error("Invalid relational operator: " + op);
            return "";
        }
    }

    void expectOperator(const std::string& op) {
        if (!match("Operator", op)) {
            error("Expected operator '" + op + "'");
        } else {
            pos++;
        }
    }

    std::string parseExpression() {
        std::string left;
        if (match("Number") || match("Identifier")) {
            left = tokens[pos++].value;
        } else {
            error("Expected operand");
            return "";
        }

        if (match("Operator", "+") || match("Operator", "-") || match("Operator", "*") || match("Operator", "/")) {
            std::string op = tokens[pos++].value;
            std::string right = parseExpression();
            std::string temp = newTemp();
            std::cout << "       " << temp << " = " << left << " " << op << " " << right << "\n";
            return temp;
        }

        return left;
    }

    void parseDeclaration() {
        pos++; // skip 'int'
        std::string varName = expectIdentifier();

        if (symbolTable.find(varName) != symbolTable.end()) {
            std::cerr << "Error: Variable '" << varName << "' already declared.\n";
        } else {
            symbolTable[varName] = "int";
        }

        if (match("Operator", "=")) {
            pos++; // skip '='
            std::string rhs = parseExpression();
            std::cout << "       " << varName << " = " << rhs << "\n";
        }

        expectOperator(";");
    }

    void parseAssignment() {
        std::string varName = expectIdentifier();

        if (symbolTable.find(varName) == symbolTable.end()) {
            std::cerr << "Error: Variable '" << varName << "' used before declaration.\n";
        }

        expectOperator("=");
        std::string rhs = parseExpression();
        expectOperator(";");
        std::cout << "       " << varName << " = " << rhs << "\n";
    }

    void parseIfStatement() {
        pos++; // skip 'if'
        expectOperator("(");

        std::string condVar = expectIdentifier();
        std::string relOp = expectRelationalOperator();
        std::string condVal = expectNumber();

        expectOperator(")");
        expectOperator("{");

        std::string labelTrue = newLabel();
        std::string labelFalse = newLabel();
        std::string labelNext = newLabel();

        // Print condition jump:
        std::cout << "       if " << condVar << " " << relOp << " " << condVal << " go to " << labelTrue << "\n";
        std::cout << "       go to " << labelFalse << "\n";

        std::cout << labelTrue << ":\n";

        while (!match("Operator", "}")) {
            if (match("Keyword", "int")) {
                parseDeclaration();
            } else if (match("Identifier")) {
                parseAssignment();
            } else {
                error("Unexpected token inside if block");
                pos++;
            }
        }
        expectOperator("}");

        std::cout << "       go to " << labelNext << "\n";

        std::cout << labelFalse << ":\n";

        // Check for optional else
        if (match("Keyword", "else")) {
            pos++; // skip 'else'
            expectOperator("{");

            while (!match("Operator", "}")) {
                if (match("Keyword", "int")) {
                    parseDeclaration();
                } else if (match("Identifier")) {
                    parseAssignment();
                } else {
                    error("Unexpected token inside else block");
                    pos++;
                }
            }
            expectOperator("}");
        }

        std::cout << labelNext << ":\n";
    }

    void parseForLoop() {
    pos++; // skip 'for'
    expectOperator("(");

    // Parse init statement: could be int declaration or assignment
    if (match("Keyword", "int")) {
        parseDeclaration();
    } else if (match("Identifier")) {
        parseAssignment();
    } else {
        error("Expected init statement in for loop");
    }

    // Now parse condition
    std::string condVar = expectIdentifier();
    std::string relOp = expectRelationalOperator();
    std::string condVal = expectNumber();
    expectOperator(";");

    // Parse update expression (e.g., i = i + 1)
    std::string updateVar = expectIdentifier();
    expectOperator("=");
    std::string updateExpr = parseExpression();
    expectOperator(")");

    // Labels for loop control
    std::string labelStart = newLabel();
    std::string labelBody = newLabel();
    std::string labelEnd = newLabel();

    // Print init statement already done inside parseDeclaration/parseAssignment

    std::cout << labelStart << ":\n";

    std::cout << "       if " << condVar << " " << relOp << " " << condVal << " go to " << labelBody << "\n";
    std::cout << "       go to " << labelEnd << "\n";

    std::cout << labelBody << ":\n";

    // Parse loop body statements
    expectOperator("{");
    while (!match("Operator", "}")) {
        if (match("Keyword", "int")) {
            parseDeclaration();
        } else if (match("Identifier")) {
            parseAssignment();
        } else if (match("Keyword", "if")) {
            parseIfStatement();
        } else {
            error("Unexpected token inside for loop body");
            pos++;
        }
    }
    expectOperator("}");

    // Print update expression
    std::cout << "       " << updateVar << " = " << updateExpr << "\n";

    // Jump back to start to check condition again
    std::cout << "       go to " << labelStart << "\n";

    std::cout << labelEnd << ":\n";
}

};

// Token reading from file
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
    std::vector<Token> tokens = readTokensFromFile("tokensImmediateCodeGeneration.txt");
    ImmediateCodeGenerator generator(tokens);
    generator.generate();
    return 0;
}
*/




#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <unordered_map>

struct Token {
    std::string type;
    std::string value;
};

class ImmediateCodeGenerator {
    std::vector<Token> tokens;
    int pos = 0;
    int tempCount = 0;
    int labelCount = 0;

public:
    std::unordered_map<std::string, std::string> symbolTable;

    ImmediateCodeGenerator(const std::vector<Token>& tokens) : tokens(tokens) {}

    void generate() {
        while (pos < tokens.size()) {
            if (match("Keyword", "int")) {
                parseDeclaration();
            } else if (match("Identifier")) {
                parseAssignment();
            } else if (match("Keyword", "if")) {
                parseIfStatement();
            } else if (match("Keyword", "for")) {
                parseForLoop();
            } else if (match("Keyword", "while")) {
                parseWhileLoop();
            } else {
                error("Unexpected token at global scope");
                pos++;
            }
        }
    }

private:
    bool match(const std::string& type, const std::string& value = "") {
        if (pos >= tokens.size()) return false;
        if (tokens[pos].type != type) return false;
        if (!value.empty() && tokens[pos].value != value) return false;
        return true;
    }

    void error(const std::string& msg) {
        std::cerr << "Error at token " << pos << ": " << msg << "\n";
    }

    std::string newTemp() {
        return "t" + std::to_string(tempCount++);
    }

    std::string newLabel() {
        return "L" + std::to_string(labelCount++);
    }

    std::string expectIdentifier() {
        if (!match("Identifier")) {
            error("Expected identifier");
            return "";
        }
        return tokens[pos++].value;
    }

    std::string expectNumber() {
        if (!match("Number")) {
            error("Expected number");
            return "";
        }
        return tokens[pos++].value;
    }

    std::string expectRelationalOperator() {
        if (!match("Operator")) {
            error("Expected relational operator");
            return "";
        }
        std::string op = tokens[pos].value;
        if (op == "==" || op == "!=" || op == "<" || op == "<=" || op == ">" || op == ">=") {
            pos++;
            return op;
        } else {
            error("Invalid relational operator: " + op);
            return "";
        }
    }

    void expectOperator(const std::string& op) {
        if (!match("Operator", op)) {
            error("Expected operator '" + op + "'");
        } else {
            pos++;
        }
    }

    std::string parseExpression() {
        std::string left;
        if (match("Number") || match("Identifier")) {
            left = tokens[pos++].value;
        } else {
            error("Expected operand");
            return "";
        }

        if (match("Operator", "+") || match("Operator", "-") || match("Operator", "*") || match("Operator", "/")) {
            std::string op = tokens[pos++].value;
            std::string right = parseExpression();
            std::string temp = newTemp();
            std::cout << "       " << temp << " = " << left << " " << op << " " << right << "\n";
            return temp;
        }

        return left;
    }

    void parseDeclaration() {
        pos++; // skip 'int'
        std::string varName = expectIdentifier();

        if (symbolTable.find(varName) != symbolTable.end()) {
            std::cerr << "Error: Variable '" << varName << "' already declared.\n";
        } else {
            symbolTable[varName] = "int";
        }

        if (match("Operator", "=")) {
            pos++; // skip '='
            std::string rhs = parseExpression();
            std::cout << "       " << varName << " = " << rhs << "\n";
        }

        expectOperator(";");
    }

    void parseAssignment() {
        std::string varName = expectIdentifier();

        if (symbolTable.find(varName) == symbolTable.end()) {
            std::cerr << "Error: Variable '" << varName << "' used before declaration.\n";
        }

        expectOperator("=");
        std::string rhs = parseExpression();
        expectOperator(";");
        std::cout << "       " << varName << " = " << rhs << "\n";
    }

    void parseIfStatement() {
        pos++; // skip 'if'
        expectOperator("(");

        std::string condVar = expectIdentifier();
        std::string relOp = expectRelationalOperator();
        std::string condVal = expectNumber();

        expectOperator(")");
        expectOperator("{");

        std::string labelTrue = newLabel();
        std::string labelFalse = newLabel();
        std::string labelNext = newLabel();

        std::cout << "       if " << condVar << " " << relOp << " " << condVal << " go to " << labelTrue << "\n";
        std::cout << "       go to " << labelFalse << "\n";

        std::cout << labelTrue << ":\n";

        while (!match("Operator", "}")) {
            if (match("Keyword", "int")) {
                parseDeclaration();
            } else if (match("Identifier")) {
                parseAssignment();
            } else {
                error("Unexpected token inside if block");
                pos++;
            }
        }
        expectOperator("}");

        std::cout << "       go to " << labelNext << "\n";
        std::cout << labelFalse << ":\n";

        if (match("Keyword", "else")) {
            pos++; // skip 'else'
            expectOperator("{");
            while (!match("Operator", "}")) {
                if (match("Keyword", "int")) {
                    parseDeclaration();
                } else if (match("Identifier")) {
                    parseAssignment();
                } else {
                    error("Unexpected token inside else block");
                    pos++;
                }
            }
            expectOperator("}");
        }

        std::cout << labelNext << ":\n";
    }

    void parseForLoop() {
        pos++; // skip 'for'
        expectOperator("(");

        if (match("Keyword", "int")) {
            parseDeclaration();
        } else if (match("Identifier")) {
            parseAssignment();
        } else {
            error("Expected init statement in for loop");
        }

        std::string condVar = expectIdentifier();
        std::string relOp = expectRelationalOperator();
        std::string condVal = expectNumber();
        expectOperator(";");

        std::string updateVar = expectIdentifier();
        expectOperator("=");
        std::string updateExpr = parseExpression();
        expectOperator(")");

        std::string labelStart = newLabel();
        std::string labelBody = newLabel();
        std::string labelEnd = newLabel();

        std::cout << labelStart << ":\n";
        std::cout << "       if " << condVar << " " << relOp << " " << condVal << " go to " << labelBody << "\n";
        std::cout << "       go to " << labelEnd << "\n";

        std::cout << labelBody << ":\n";
        expectOperator("{");
        while (!match("Operator", "}")) {
            if (match("Keyword", "int")) {
                parseDeclaration();
            } else if (match("Identifier")) {
                parseAssignment();
            } else if (match("Keyword", "if")) {
                parseIfStatement();
            } else {
                error("Unexpected token inside for loop body");
                pos++;
            }
        }
        expectOperator("}");

        std::cout << "       " << updateVar << " = " << updateExpr << "\n";
        std::cout << "       go to " << labelStart << "\n";
        std::cout << labelEnd << ":\n";
    }

    void parseWhileLoop() {
        pos++; // skip 'while'
        expectOperator("(");

        std::string condVar = expectIdentifier();
        std::string relOp = expectRelationalOperator();
        std::string condVal = expectNumber();

        expectOperator(")");
        expectOperator("{");

        std::string labelStart = newLabel();
        std::string labelBody = newLabel();
        std::string labelEnd = newLabel();

        std::cout << labelStart << ":\n";
        std::cout << "       if " << condVar << " " << relOp << " " << condVal << " go to " << labelBody << "\n";
        std::cout << "       go to " << labelEnd << "\n";

        std::cout << labelBody << ":\n";

        while (!match("Operator", "}")) {
            if (match("Keyword", "int")) {
                parseDeclaration();
            } else if (match("Identifier")) {
                parseAssignment();
            } else if (match("Keyword", "if")) {
                parseIfStatement();
            } else {
                error("Unexpected token inside while loop");
                pos++;
            }
        }

        expectOperator("}");
        std::cout << "       go to " << labelStart << "\n";
        std::cout << labelEnd << ":\n";
    }
};

// Token reader
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
    std::vector<Token> tokens = readTokensFromFile("tokensImmediateCodeGeneration.txt");
    ImmediateCodeGenerator generator(tokens);
    generator.generate();
    return 0;
}
