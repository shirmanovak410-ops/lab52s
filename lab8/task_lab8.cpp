#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <sstream>
#include <stack>
#include <cctype>
bool Operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '^';
}

bool Function(const std::string& str) {
    return str == "sin" || str == "cos" || str == "tg" || str == "ctg" || str == "exp";
}

std::vector<std::string> infixtopostfix(const std::string& infix) {
    std::vector<std::string> result;
    std::stack<std::string> operators;
    std::unordered_map<std::string, int> priority = {
        {"+", 1}, {"-", 1},
        {"*", 2}, {"/", 2},
        {"^", 3},
        {"sin", 4}, {"cos", 4}, {"tg", 4}, {"ctg", 4}, {"exp", 4}
    };
    std::string cleaned;
    for (char c : infix) { //удаление пробелов
        if (!std::isspace(c)) cleaned += c;
    }
    for (size_t i = 0; i < cleaned.length(); ++i) {
        char c = cleaned[i];

        if (std::isdigit(c) || (c == '-' && (i == 0 || cleaned[i-1] == '(' || Operator(cleaned[i-1])))) {
            std::string number;

            if (c == '-') {
                number = "-";
                i++;
                if (i < cleaned.length()) c = cleaned[i];
            }

            while (i < cleaned.length() && (std::isdigit(cleaned[i]) || cleaned[i] == '.')) {
                number += cleaned[i];
                i++;
            }
            i--;
            result.push_back(number);
        }

        else if (std::isalpha(c)) {
            std::string func;
            while (i < cleaned.length() && std::isalpha(cleaned[i])) {
                func += cleaned[i];
                i++;
            }
            i--;
            operators.push(func);
        }

        else if (c == '(') {
            operators.push("(");
        }

        else if (c == ')') {
            while (!operators.empty() && operators.top() != "(") {
                result.push_back(operators.top());
                operators.pop();
            }
            if (!operators.empty() && operators.top() == "(") {
                operators.pop();
            }
            // Если после скобки была функция - добавляем её
            if (!operators.empty() && Function(operators.top())) {
                result.push_back(operators.top());
                operators.pop();
            }
        }

        else if (Operator(c)) {
            std::string op(1, c);


            if (c == '-' && !(i == 0 || cleaned[i-1] == '(' || Operator(cleaned[i-1]))) {
                // Выталкиваем операторы с высшим или равным приоритетом
                while (!operators.empty() && operators.top() != "(" &&
                       priority[operators.top()] >= priority[op]) {
                    result.push_back(operators.top());
                    operators.pop();
                }
                operators.push(op);
            }
            else if (c != '-') { // Обработка других операторов
                while (!operators.empty() && operators.top() != "(" &&
                       priority[operators.top()] >= priority[op]) {
                    result.push_back(operators.top());
                    operators.pop();
                }
                operators.push(op);
            }
        }
    }

    while (!operators.empty()) {
        result.push_back(operators.top());
        operators.pop();
    }
    return result;
}
int main() {
    std::string line;
    std::getline(std::cin, line);

    std::vector<std::string> postfix = infixtopostfix(line);
    for (const auto& token : postfix) {
        std::cout << token << " ";
    }
    std::cout << std::endl;

    return 0;
}