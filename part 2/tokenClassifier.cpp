
#include <vector>
#include <sstream>
#include <algorithm>
#include <memory>
#include "tokenClasses.hpp"
#include <cctype>
using namespace ensc251;
using namespace std;
#define z "\r\t\v\f "

// Define strings
std::string delx[] = { ">>=", "<<=" };
std::string dely[] = { "+=", "-=", "/=", "%=", "<<", ">>", "++", "--" };
std::string delz{ "/()*%:;=+-~?" };
std::string Z{ "\r\t\v\f " };

// Function to process a sequence of digits in a line
bool DigPros(std::string line, std::string::size_type& index)
{
   bool DigiFound = false;
   while (index < line.length() && isdigit(line.at(index)))
    {
     index++;
     DigiFound = true;
    }
   return DigiFound;
}

// Token classification function
std::vector<shared_ptr<Token>> tokenClassifier(istream& code)
{
    std::vector<shared_ptr<Token>> tokensobj;
    std::string line;
    std::string token;
    int lineIndex = 0;
    bool intCheck = true;
    bool floatCheck = false;
    bool pointerCheck = false;

    // Read lines of code from the input stream
    while (getline(code, line))
    {
        // Skip lines starting with '#'
        if (line.at(0) == '#')
            continue;

        std::string::size_type index2 = 0;
        auto length = line.length();

        // Process each character in the line
        while (index2 < length)
        {
            // Skip line break and increment line index
            if (line[index2] == '\n')
            {
                lineIndex++;
            }

            // Find the first non-white character in the line
            auto index1 = line.find_first_not_of(Z, index2);
            if (std::string::npos == index1)
                break;

            // Process integer and float constants
            if (isdigit(line.at(index1)) || line.at(index1) == '.')
            {
                index2 = index1;

                // Integer constants
                bool preDigitFound{ DigPros(line, index2) };

                // Floats
                if (index2 < length && line.at(index2) == '.')
                {
                    index2++;
                    bool postDigitFound{ DigPros(line, index2) };

                    if (!preDigitFound && !postDigitFound)
                    {
                        token = line.substr(index1, index2 - index1);
                        cout << "Tokenizer detected: invalid token in line: " << line.substr(index1, index2 - index1) << endl;
                        tokensobj.push_back(make_shared<incorrect>(token, lineIndex, index1));
                        continue;
                    }
                }

                // Exponentials
                if (index2 < length && ('e' == line.at(index2) || 'E' == line.at(index2)))
                {
                    // Check if exponential is followed by '+' or '-'
                    if ('+' == line.at(index2 + 1) || '-' == line.at(index2 + 1))
                        index2 += 2;
                    else
                        index2++;
                    if (!DigPros(line, index2))
                    {
                        // Push back the changes
                        if ('e' == line.at(index2 - 1) || 'E' == line.at(index2 - 1))
                            index2--;
                        else
                            index2 -= 2;
                    }
                }

                auto subs = line.substr(index1, index2 - index1);
                if(subs.find_first_of(".eE") != std::string::npos)
                                tokensobj.push_back(make_shared<ensc251::numeric_const<float>>(stof(subs), lineIndex, index1));
                                else
                                tokensobj.push_back(make_shared<ensc251::int_const<int>>(stoi(subs), lineIndex, index1));
                                continue;
                        }
                        for (auto delim : delx) {
                                if (line.substr(index1, 3) == delim) {//this cannot compare
                                        tokensobj.push_back(make_shared<ensc251::int_assignment_operator>(delim, lineIndex, index1));
                                        index2 = index1 + 3;
                                        break;
                                }
                        }
                        if (index2 > index1) continue;


                        for (auto delim : dely) {
                                if (line.substr(index1, 2) == delim) {
                                        if(delim == "%=")
                                tokensobj.push_back(make_shared<ensc251::int_assignment_operator>(delim, lineIndex, index1));
                                            if(delim == ">>" || delim == "<<" )
                                tokensobj.push_back(make_shared<shift_operator>(delim, lineIndex, index1));

                                    if(delim == "/=" || delim == "+=" || delim == "-=")
                                tokensobj.push_back(make_shared<gen_assignment_operator>(delim, lineIndex, index1));

                                if(delim == "++" || delim == "--")
                                tokensobj.push_back(make_shared<postfix_operator>(delim, lineIndex, index1));

                                index2 = index1 + 2;
                                break;
                                }
                                }
                        // if index2 is greater than index1 move to next iteration
                        if (index2 > index1) continue;

                        if ('"' == line[index1]) {
                                index2 = index1 + 1;
                                while (index2 < length) {
                                        if ('\\' == line[index2]) {
                                                index2++;
                                        }
                                        else
                                                if ('"' == line.at(index2)) {
                                                ;
                                                        break;
                                                }
                                        index2++;
                                }
                                if (index2 >= length) { // std::string is not terminated!
                                        cout << "Tokenizer detected:  std::string is not terminated!" << endl;
                                        token = line.substr(index1, index2 - index1);
                                        tokensobj.push_back(make_shared<incorrect>(token, lineIndex, index1));
                                        break;
                                }
                                index2 ++;
                                token = line.substr(index1, index2 - index1);
                                tokensobj.push_back(make_shared<ensc251::string>(token, lineIndex, index1));
                                continue;
                        }

                        if ('\'' == line.at(index1)) {
                                index2 = index1 + 1;
                                if (index2 < length  &&  '\\' == line.at(index2)) {
                                        index2 ++;
                                }
                                index2 ++;
                                if (index2 >= length  ||  line.at(index2) != '\'') // Character constant is not terminated!
                                {
                                        cout << "Tokenizer detected:  character constant is not terminated!" << endl;
                                        token = line.substr(index1, index2 - index1);
                                        tokensobj.push_back(make_shared<incorrect>(token, lineIndex, index1));
                                }
                                else {
                                        index2 ++;
                                        token = line.substr(index1, index2 - index1);
                                        tokensobj.push_back(make_shared<int_const<char>>(token[1], lineIndex, index1));
                                }
                                continue;
                        }
                        // check if the line contains "-" or an alphabetic letter
                        if('_' == line[index1]  ||  isalpha(line[index1])) {
                                index2 = index1 + 1;
                                while (index2 < length  &&  ('_' == line[index2]  ||  isalnum(line[index2])) )
                                        index2 ++;
                                token = line.substr(index1, index2 - index1);

                                if(token == "char" || token == "int"){
                                intCheck = true;
                                floatCheck = false;
                                pointerCheck = false;
                                tokensobj.push_back(make_shared<ensc251::type_spec>(token, lineIndex, index1));
                                }
                                else if(token == "float"){
                                intCheck = false;
                                floatCheck = true;
                                pointerCheck = false;
                                tokensobj.push_back(make_shared<ensc251::type_spec>(token, lineIndex, index1));
                                }
                                else if(intCheck)
                                tokensobj.push_back(make_shared<ensc251::int_id>(token, lineIndex, index1));

                                else if(floatCheck)

                                tokensobj.push_back(make_shared<ensc251::numeric_id>(token, lineIndex, index1));
                                else if(pointerCheck)
                                tokensobj.push_back(make_shared<ensc251::pointer_id>(token, lineIndex, index1));\
                                     continue;
                        }

        index2 = line.find_first_of(delz, index1);

        if (index2 == index1) {
        index2++;
        token = line.substr(index1,1);
        char tokenChar = line[index1];
        // token objects
        switch(tokenChar){
        case '/': tokensobj.push_back(make_shared<div_operator>(token, lineIndex, index1)); break;
        case '+': tokensobj.push_back(make_shared<additive_operator>(token, lineIndex, index1)); break;
        case '-': tokensobj.push_back(make_shared<additive_operator>(token, lineIndex, index1)); break;
        case '~': tokensobj.push_back(make_shared<comp_operator>(token, lineIndex, index1)); break;
        case '%': tokensobj.push_back(make_shared<mod_operator>(token, lineIndex, index1)); break;
        case '?': tokensobj.push_back(make_shared<conditional_operator>(token, lineIndex, index1)); break;
        case '=': tokensobj.push_back(make_shared<gen_assignment_operator>(token, lineIndex, index1)); break;
        case ';': tokensobj.push_back(make_shared<punctuator>(token, lineIndex, index1)); break;
        case ':': tokensobj.push_back(make_shared<punctuator>(token, lineIndex, index1)); break;
        case '(': tokensobj.push_back(make_shared<punctuator>(token, lineIndex, index1)); break;
        case ')': tokensobj.push_back(make_shared<punctuator>(token, lineIndex, index1)); break;
        case '*':
        {
        pointerCheck = true;
        intCheck = false;
        floatCheck = false;
        tokensobj.push_back(make_shared<punctuator>(token, lineIndex, index1));
        break;
        }
        }


        continue;
        }
        
        token = line.substr(index1, index2 - index1);
        tokensobj.push_back(make_shared<incorrect>(token, lineIndex, index1));


        }
        lineIndex++;
        }
        // return 
        return tokensobj;
}