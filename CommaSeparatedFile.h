#pragma once
#include <iostream>
#include <fstream> // File Management
#include <string>
#include <sstream> // File Management

using namespace std;

class CommaSeparatedDataFileRead
{
private:
	ifstream m_file;
	stringstream m_stringStream;
	string m_line;
	string m_word;
	char m_delimChar;

	bool getNextLine();
public:
	CommaSeparatedDataFileRead(string fileName, char delimChar = ',');
	~CommaSeparatedDataFileRead();
	bool getNextValue(string* value);
};
class CommaSeparatedDataFileWrite
{
private:
	fstream m_file;
	stringstream m_stringStream;
	string m_line;
	string m_word;
	char m_delimChar;
public:
	CommaSeparatedDataFileWrite(string fileName, char delimChar = ',');
	~CommaSeparatedDataFileWrite();
	void saveNextValue(const string value);
	void nextLine();
};

