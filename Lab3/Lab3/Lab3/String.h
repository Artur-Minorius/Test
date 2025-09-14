#pragma once
#include <iostream>
#include "List.h"
class String
{
private:
	List<char> string;

public:
	String(const char* str);
	String(double str);
	String(String&);
	String();
	~String();

	void Append(const char*);
	void Append(double);
	void Append(String&);

	char* ToCharArray();
	wchar_t* ToWCharArray();

	char operator[](int i);
	bool operator==(String&);
	bool operator==(const char*);

	String& operator+(String&);
	String& operator+(const char*);

	friend std::ostream& operator<<(std::ostream&, String&);
	friend std::istream& operator>>(std::istream&, String&);
};

