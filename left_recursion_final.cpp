#include<iostream>
#include<fstream>
#include<windows.h>
#include<string>
#include<sstream>
#include<vector>
using namespace std;

class NonTerminal{
    string name;
    vector<string> productionRules;
    public:
        NonTerminal(string name)
        {
            this->name=name;
        }
        string getName()
        {
            return name;
        }
        void setRules(vector<string> &rules)
        {
            productionRules=rules;
        }
        void addRule(string &rule)
        {
            productionRules.push_back(rule);
        }
        vector<string> getRules()
        {
            return productionRules;
        }
        void printRule()
        {
            cout << name << " -> ";
            for (int i = 0; i < productionRules.size(); i++) {
                if (productionRules[i] == "ε") {
                    cout << "ε";
                } else {
                    cout << productionRules[i];
                }
    
                if (i != productionRules.size() - 1) {
                    cout << " | ";
                }
            }
            cout << endl;
        }
        
};

class Grammar{
    vector<NonTerminal> nonTerminals;
    private:
        int findNonTerminalIndex(string &name)
        {
            for(int i=0;i<nonTerminals.size();i++)
            {
                if(nonTerminals[i].getName()==name)
                {
                    return i;
                }
            }
            return -1;
        }
    public:
        void readGrammarFromFile(const string& filename)
        {
            ifstream file(filename);
            string line;
            while(getline(file,line)){
                if(line.empty())
                continue;
                stringstream ss(line);
                string lhs,arrow,rhs;
                ss>>lhs>>arrow;
                getline(ss,rhs);
                if(lhs.empty() || arrow != "->" || rhs.empty())
                {
                    continue;
                }
                vector<string> productions;
                stringstream rhsStream(rhs);
                string prod;
                while(getline(rhsStream,prod,'|'))
                {
                    prod.erase(0, prod.find_first_not_of(" \t"));
                    prod.erase(prod.find_last_not_of(" \t") + 1);
                    productions.push_back(prod);
                }
                int index=findNonTerminalIndex(lhs);
                if(index==-1)
                {
                    NonTerminal nt(lhs);
                    nt.setRules(productions);
                    nonTerminals.push_back(nt);
                }
                else{
                    for(auto &prod:productions)
                    {
                        nonTerminals[index].addRule(prod);
                    }
                }
            }
        }

        void solveNonImmediateLR(NonTerminal &A, NonTerminal &B)
        {
            string nameA=A.getName();
            string nameB=B.getName();
            vector<string> rulesA=A.getRules();
            vector<string> rulesB=B.getRules();
            vector<string> newRulesA;
            for(auto rule:rulesA){
                if (rule.substr(0, nameB.size()) == nameB){
                    for(auto ruleB:rulesB)
                    {
                        newRulesA.push_back(ruleB+rule.substr(nameB.size()));
                    }
                }
                else{
                    newRulesA.push_back(rule);
                }
            }
            A.setRules(newRulesA);
        }
        void solveImmediateLR(NonTerminal &A)
        {
            string name=A.getName();
            string newName=name+"'";

            vector<string> alphas,betas;
            for(auto rule:A.getRules())
            {
                if (rule.substr(0, name.size()) == name) 
                {
                    alphas.push_back(rule.substr(name.size()));
                }
                else{
                    betas.push_back(rule);
                }
            }
            if(alphas.empty())
            return;
            vector<string> newRulesA,newRulesA1;
            for(auto &beta:betas){
                newRulesA.push_back(beta + newName);
            }
            for (auto& alpha : alphas) {
                newRulesA1.push_back(alpha + newName);
            }
            newRulesA1.push_back("ε");

            A.setRules(newRulesA);
            NonTerminal newNonTerminal(newName);
            newNonTerminal.setRules(newRulesA1);
            nonTerminals.push_back(newNonTerminal);
        }

        void applyAlgo(){
            for(int i=0;i<nonTerminals.size();i++)
            {
                for(int j=0;j<i;j++)
                {
                    solveNonImmediateLR(nonTerminals[i],nonTerminals[j]);
                }
                solveImmediateLR(nonTerminals[i]);
            }
        }
        void printRules()
        {
            for (auto& nonTerminal : nonTerminals) {
                nonTerminal.printRule();
            }
        }
        
};

int main()
{
    SetConsoleOutputCP(CP_UTF8);
    Grammar grammar;
    grammar.readGrammarFromFile("grammar.txt");
    grammar.applyAlgo();
    grammar.printRules();
    return 0;
}