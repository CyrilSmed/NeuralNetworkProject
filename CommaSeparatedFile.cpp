#include "CommaSeparatedFile.h"
#include <iostream>
#include <vector>
#include <fstream> // File Management
#include <string>
#include <sstream> // File Management

using namespace std;

CommaSeparatedDataFileRead::CommaSeparatedDataFileRead(string fileName, char delimChar)
{
	m_delimChar = delimChar;

	//m_file.open(fileName, ios::out | ios::trunc);
	m_file.open(fileName, ios::in);

	if (!m_file.is_open())
	{
		throw runtime_error("Could not open file");
	}

	// Read the column names
	if (m_file.good())
	{
		getline(m_file, m_line);
		stringstream stringStream(m_line);
		getline(stringStream, m_word, m_delimChar);


		if (m_line.find("﻿") != string::npos) m_file.seekg(3, ios::beg);
		else m_file.seekg(0, ios::beg);

		getNextLine();
	}
}
CommaSeparatedDataFileRead::~CommaSeparatedDataFileRead() { m_file.close(); }
bool CommaSeparatedDataFileRead::getNextValue(string* value)
{
	bool endOfFile = false;
	while (!endOfFile)
	{
		if (!m_stringStream.eof())
		{
			getline(m_stringStream, m_word, m_delimChar);
			*value = m_word;
			return true;
		}
		else
		{
			endOfFile = getNextLine();
		}
	}
	return false;
}
bool CommaSeparatedDataFileRead::getNextLine()
{
	if (m_file.eof() == 0)
	{
		getline(m_file, m_line);
		stringstream temp(m_line);
		m_stringStream.swap(temp);

		return false;
	}
	else
	{
		m_file.clear(); // clear end of the file flag
		m_file.seekg(0, ios::beg); // return to the beginning

		getline(m_file, m_line);
		stringstream stringStream(m_line);
		getline(stringStream, m_word, m_delimChar);
		if (m_line.find("﻿") != string::npos) m_file.seekg(3, ios::beg);
		else m_file.seekg(0, ios::beg);

		getNextLine();
		return true;
	}
}

CommaSeparatedDataFileWrite::CommaSeparatedDataFileWrite(string fileName, char delimChar)
{
	m_delimChar = delimChar;

	m_file.open(fileName, ios::out | ios::trunc);

	if (!m_file.is_open())
	{
		throw runtime_error("Could not open file");
	}
}
CommaSeparatedDataFileWrite::~CommaSeparatedDataFileWrite() { m_file.close(); }
void CommaSeparatedDataFileWrite::saveNextValue(const string value)
{
	m_file << value << m_delimChar;
}
void CommaSeparatedDataFileWrite::nextLine()
{
	m_file.seekg(-1, ios::end);
	m_file << '\n';
}