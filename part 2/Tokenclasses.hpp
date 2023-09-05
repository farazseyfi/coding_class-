
#ifndef TOKENCLASSES_HPP_

#define TOKENCLASSES_HPP_

#include <string>

#include <iostream>

#include <typeinfo>

namespace ensc251 {

char *demangle(const char *typeName);

class Token { // The Base Token Class
private:

int X_lineIndex;

int X_posIndex;
public:

int getLineIndex() const { return X_lineIndex; }

int getPosIndex() const { return X_posIndex; }

Token(){} // make sure this does not get invoked.


Token(int lineIndex, int posIndex):X_lineIndex(lineIndex),X_posIndex(posIndex) {

}

// we will fix this up when we learn about virtual functions.
friend std::ostream& operator <<(std::ostream& outs, const Token& token){
token.print(outs);
return outs;
}
void print(std::ostream& out) const { out << demangle(typeid(*this).name()) << " (line: " << X_lineIndex <<", index: " << X_posIndex <<")" << ": "; }
virtual ~Token() {}
};

class StringBasedToken:public Token {
private:
std::string m_stringValue;
public:
StringBasedToken(){};
StringBasedToken(const std::string &aStringValue, int lineIndex, int posIndex): Token(lineIndex, posIndex),m_stringValue(aStringValue){}
const std::string& getStringValue( ) const { return m_stringValue; }
void print(std::ostream& str) const { Token::print(str); str << ": " << m_stringValue; }
};

class punctuator:public StringBasedToken {
// e.g., ';', '*', '(', ')', ':'
public:
punctuator(const std::string &aStringValue, int lineIndex, int posIndex):StringBasedToken(aStringValue, lineIndex, posIndex){};
};

class type_spec:public StringBasedToken {
// e.g., 'char' | 'int' | 'float'
public:
type_spec(const std::string &aStringValue, int lineIndex, int posIndex):StringBasedToken(aStringValue, lineIndex, posIndex){};
};

// do not instantiate this class.
class id:public StringBasedToken {
// identifiers -- example: sum
public:
id(const std::string &aStringValue, int lineIndex, int posIndex):StringBasedToken(aStringValue, lineIndex, posIndex){};
};

class pointer_id:public id {
// e.g., char* identifier
public:
pointer_id(const std::string &aStringValue, int lineIndex, int posIndex):id{aStringValue, lineIndex, posIndex}{};
};

class numeric_id:public id {
// char, int, and float identifiers
public:
numeric_id(const std::string &aStringValue, int lineIndex, int posIndex):id{aStringValue, lineIndex, posIndex}{};
};

class int_id:public numeric_id {
// char and int identifiers
public:
int_id(const std::string &aStringValue, int lineIndex, int posIndex):numeric_id{aStringValue, lineIndex, posIndex}{};
};

class gen_assignment_operator:public StringBasedToken{
// '=' | '/=' | '+=' | '-='
public:
gen_assignment_operator(const std::string &aStringValue, int lineIndex, int posIndex):StringBasedToken(aStringValue, lineIndex, posIndex){};
};

class int_assignment_operator:public StringBasedToken{
// '%=' | '<<=' | '>>='
public:
int_assignment_operator(const std::string &aStringValue, int lineIndex, int posIndex):StringBasedToken(aStringValue, lineIndex, posIndex){};
};

class conditional_operator:public StringBasedToken{
// ?
public:
conditional_operator(const std::string &aStringValue, int lineIndex, int posIndex):StringBasedToken(aStringValue, lineIndex, posIndex){};
};

class shift_operator:public StringBasedToken{
// '<<' | '>>'
public:
shift_operator(const std::string &aStringValue, int lineIndex, int posIndex):StringBasedToken(aStringValue, lineIndex, posIndex){};
};

class additive_operator:public StringBasedToken{
// '+' | '-'
public:
additive_operator(const std::string &aStringValue, int lineIndex, int posIndex):StringBasedToken(aStringValue, lineIndex, posIndex){};
};

class div_operator:public StringBasedToken {
// '/'const T& getsValue() const { return value; }
public:
div_operator(const std::string &aStringValue, int lineIndex, int posIndex):StringBasedToken(aStringValue, lineIndex, posIndex){};
};

class mod_operator:public StringBasedToken {
// '%'
public:
mod_operator(const std::string &aStringValue, int lineIndex, int posIndex):StringBasedToken(aStringValue, lineIndex, posIndex){};
};

class comp_operator:public StringBasedToken{
// '~'
public:
comp_operator(const std::string &aStringValue, int lineIndex, int posIndex):StringBasedToken(aStringValue, lineIndex, posIndex){};
};

class postfix_operator:public StringBasedToken{
// '++' | '--'
public:
postfix_operator(const std::string &aStringValue, int lineIndex, int posIndex):StringBasedToken(aStringValue, lineIndex, posIndex){};
};

class string:public StringBasedToken
{
// e.g., "sample string"
public:
string(const std::string &aStringValue, int lineIndex, int posIndex):StringBasedToken(aStringValue, lineIndex, posIndex){};
};

class incorrect:public StringBasedToken
{
// e.g., "sample string --> This is incorrect ...
// (quotation mark " is missing at the end)
public:
incorrect(const std::string &aStringValue, int lineIndex, int posIndex):StringBasedToken(aStringValue, lineIndex, posIndex){};
};


class numeric_int_const;

class constant:public Token
{
public:
constant() { std::cout << "Warning: Do not invoke this constructor" << std::endl; }; // make sure this does not get invoked.
constant(int lineIndex, int posIndex): Token{lineIndex, posIndex} {};
};

template<typename T>
class numeric_const:public constant
{
// e.g. 48.5
private:
T m_value;
std::string m_stringValue;
public:
numeric_const(const T& constValue, int lineIndex, int posIndex):constant(lineIndex, posIndex), m_value(constValue){};
const T& getValue() const { return m_value; }
const std::string& getstring() const{return m_stringValue;}

void print(std::ostream& outs) const {Token::print(outs); outs << ": " << m_value; }
};

template<typename T>
class int_const:public numeric_const<T>{
public:
int_const(const T& constValue, int lineIndex, int posIndex): numeric_const<T>(constValue, lineIndex, posIndex) {}
};

}

#endif /* TOKENCLASSES_HPP_ */