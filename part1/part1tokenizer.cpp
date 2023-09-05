
//#include "Part1Tokenizer.hpp"
#include <vector>
#include <string>
#include <istream>

using namespace std;

vector<string> tokenizeCodeStrip(istream& code) {
    // Initialize variables
    string line;
    vector<string> tokens;
    int switchCount = 1; // Used for detecting certain character sequences

    while (getline(code, line)) {
        int currentIndex = 0;
        string nextCharacterHold;

        if (line.empty() || line[0] == '#') {
            continue; // Skip empty lines and lines starting with #
        }

        string currentToken;
        bool inStringX = false;
        bool inNumber = false;
        bool inCharLiteral = false;
        int brokenCharacterCount = 0; // Used for handling broken character literals

        for (char currentCharacter : line) {
            currentIndex++;
            char nextNextChar = line[currentIndex + 1];
            char nextCharacter = line[currentIndex];
            char previousCharacter = line[currentIndex - 2];

            if (brokenCharacterCount == 1) {
                brokenCharacterCount++;
                continue; // Skip the next character in the broken character literal
            }
            if (brokenCharacterCount == 2) {
                brokenCharacterCount = 0;
                continue; // Skip two characters in the broken character literal
            }

            if (inStringX) {
                // Add characters to the current string literal token
                currentToken += currentCharacter;

                if (currentCharacter == '"') {
                    if (nextCharacter == '"') {
                        continue; // Handle consecutive double quotes within a string literal
                    }

                    inStringX = false;
                    tokens.push_back(currentToken);
                    currentToken.clear(); // Clear the current token
                }
            }
            else if (inCharLiteral) {
                // Add characters to the current character literal token
                currentToken += currentCharacter;

                if (currentCharacter == '\'') {
                    inCharLiteral = false;
                    tokens.push_back(currentToken);
                    currentToken.clear(); // Clear the current token
                }
            }
            else if (inNumber) {
                // Handle number literals
                char NextCharacter = line[currentIndex];

                if (isdigit(currentCharacter) || currentCharacter == '.' ||
                    (currentCharacter == 'e' && isdigit(previousCharacter) && (isdigit(NextCharacter) || (NextCharacter == '-' && isdigit(nextNextChar)))) ||
                    (currentCharacter == 'E' && isdigit(previousCharacter) && (isdigit(NextCharacter) || (NextCharacter == '-' && isdigit(nextNextChar)))) ||
                    ((currentCharacter == '-' || currentCharacter == '+') && (previousCharacter == 'e' || previousCharacter == 'E'))) {

                    currentToken += currentCharacter;

                    if (NextCharacter == '.' && currentCharacter == '.') {
                        inNumber = false;
                        tokens.push_back(currentToken);
                        currentToken.clear(); // Clear the current token
                    }
                }
                else {
                    inNumber = false;
                    tokens.push_back(currentToken);
                    currentToken.clear(); // Clear the current token

                    if (!isspace(currentCharacter)) {
                        currentToken += currentCharacter;

                        if (currentCharacter == '+' || currentCharacter == '-') {
                            tokens.push_back(currentToken);
                            currentToken.clear(); // Clear the current token
                        }
                    }
                }
            }
            else if (isspace(currentCharacter)) {
                // Handle whitespace characters
                if (!currentToken.empty()) {
                    tokens.push_back(currentToken);
                    currentToken.clear(); // Clear the current token
                }
            }
            else if (currentCharacter == '|') {
                tokens.push_back(currentToken);
                currentToken.clear(); // Clear the current token
            }
            else if (currentCharacter == '(' || currentCharacter == ')' || currentCharacter == ':' || currentCharacter == ';' || currentCharacter == '*') {
                // Handle specific characters as individual tokens
                if (!currentToken.empty()) {
                    tokens.push_back(currentToken);
                    currentToken.clear(); // Clear the current token
                }
                tokens.push_back(string(1, currentCharacter));
            }
            else if (currentCharacter == '+' || currentCharacter == '-' || currentCharacter == '/' || currentCharacter == '%' ||
                     currentCharacter == '=' || currentCharacter == '?' || currentCharacter == '<' || currentCharacter == '>' ||
                     currentCharacter == '~') {

                char NNextCharacter = line[currentIndex];

                if (!currentToken.empty()) {
                    tokens.push_back(currentToken);
                    currentToken.clear(); // Clear the current token
                }

                if (switchCount == 2) {
                    switchCount = 1;
                    tokens.push_back(nextCharacterHold);
                    nextCharacterHold.clear();
                    continue; // Skip to the next character in the line
                }
                else if ((currentCharacter == '+' && NNextCharacter == '+') ||
                         (currentCharacter == '-' && NNextCharacter == '-') ||
                         (currentCharacter == '=' && NNextCharacter == '=') ||
                         (currentCharacter == '<' && NNextCharacter == '<') ||
                         (currentCharacter == '>' && NNextCharacter == '>') ||
                         (currentCharacter == '=' && NNextCharacter == '~')) {

                    nextCharacterHold += currentCharacter;
                    nextCharacterHold += NNextCharacter;
                    switchCount++;
                    continue; // Skip to the next character in the line
                }

                tokens.push_back(string(1, currentCharacter));
                nextCharacterHold.clear();
            }
            else if (currentCharacter == '"') {
                // Handle string literals
                if (!currentToken.empty()) {
                    tokens.push_back(currentToken);
                    currentToken.clear(); // Clear the current token
                }
                currentToken += currentCharacter;
                inStringX = true;
                continue; // Skip to the next character in the line
            }
            else if (currentCharacter == '\'') {
                // Handle character literals
                if (nextNextChar != '\'' && (nextCharacter != '\\' && line[currentIndex + 2] != '\'')) {
                    brokenCharacterCount = 1;
                    tokens.push_back(currentToken);
                    continue; // Skip the next character in the line
                }

                if (!currentToken.empty()) {
                    tokens.push_back(currentToken);
                    currentToken.clear(); // Clear the current token
                }

                currentToken += currentCharacter;
                inCharLiteral = true;
            }
            else if (isdigit(currentCharacter) || currentCharacter == '.') {
                // Handle number literals
                if (nextCharacter == '.' && !isdigit(currentCharacter)) {
                    tokens.push_back(currentToken);
                    continue; // Skip to the next character in the line
                }

                currentToken += currentCharacter;
                inNumber = true;
            }
            else {
                currentToken += currentCharacter;
            }
        }

        if (!currentToken.empty()) {
            tokens.push_back(currentToken);
        }

        if (!tokens.empty() && inStringX) {
            tokens.back().clear(); // Clear the last token if it is an incomplete string literal
        }
    }

    return tokens;
}