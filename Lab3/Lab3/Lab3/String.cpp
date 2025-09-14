#include "String.h"
#include <sstream>
//Constructors

String::String(const char* str) {
	for (int i = 0; str[i] != '\0'; i++) {
		string.push_back(str[i]);
	}
}
String::String(String& str) {
	for (int i = 0; i < str.string.size(); i++) {
		string.push_back(str[i]);
	}
}

String::String() {
}
String::~String() {
	//string.~List();
}
String::String(double str) {
	Append(str);
}
//Input/Output

std::ostream& operator<<(std::ostream& os, String& str) {
	str.string.ForEach([&](char symbol) {os << symbol; });
	return os;
}

std::istream& operator>>(std::istream& is, String& str) {
	str.~String();
	str = String();
	while (is.peek() != '\0' && is.peek() != EOF && is.peek() != '\n')
		str.string.push_back((char)is.get());
	return is;
}

//Operators
char String::operator[](int i) {
	if (i > string.size())
		return '\0';
	return this->string[i];
}
bool String::operator==(String& str) {
	int i = 0;
	if (this->string.size() != str.string.size())
		return 0;
	while (i < this->string.size() && i < str.string.size()) {
		if ((*this)[i] != str[i])
			return 0;
		i++;
	}
	return 1;
}
bool String::operator==(const char* str) {
	String s = String(str);
	return *this == s;
}
String& String::operator+(String& str) {
	String* s = new String(*this);
	s->Append(str);
	return *s;
}
String& String::operator+(const char* str) {
	String* s = new String(*this);
	s->Append(str);
	return *s;
}
//Utility

void String::Append(const char* str) {
	int str_size = 0;
	while (str[str_size++] != '\0');
	for (int i = 0; i < str_size; i++) {
		string.push_back(str[i]);
	}
}

void String::Append(String& str)
{
	Append(str.ToCharArray());
}

char* String::ToCharArray() {
	char* charArray = new char[string.size() + 1];
	charArray[string.size()] = '\0';
	for (int i = 0; i < string.size(); i++) {
		charArray[i] = string[i];
	}
	return charArray;
}

wchar_t* String::ToWCharArray()
{
	wchar_t* charArray = new wchar_t[string.size() + 1];
	charArray[string.size()] = '\0';
	for (int i = 0; i < string.size(); i++) {
		charArray[i] = (wchar_t)string[i];
	}
	return charArray;
}

void String::Append(double number) {
	std::ostringstream oss;
	oss << number;
	Append(oss.str().c_str());
}