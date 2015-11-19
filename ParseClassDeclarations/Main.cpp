#include <fstream>
#include <iostream>
#include <vector>
#include <cctype>

using namespace std;

struct ClassStruct
{
	string identifier;
	string block;
};

class ClassParser
{
public:
	string GetFile(string fileName) const
	{
		ifstream file(fileName);
		string output;
		char buffer;
		while (file.get(buffer)) output += buffer;
		return output;
	}

	int PositionOfClassIdentifierMismatchPosition(string input, int offset) const
	{
		auto classIdentifier = "class ";
		auto index = 0;
		while(index < 6)
		{
			if (input[offset + index] != classIdentifier[index]) return index;
			++index;
		}
		return -1;
	}

	vector<ClassStruct> GetClassDeclarations(string input) const
	{
		vector<ClassStruct> output;
		size_t index = 0;
		while (true)
		{
			if (index >= input.length() - 4) break;
			if (input[index] == '"')
			{
				++index;
				while (input[index] != '"') ++index;
				++index;
				continue;
			}
			index += PositionOfClassIdentifierMismatchPosition(input, index) + 1;
			if (PositionOfClassIdentifierMismatchPosition(input, index) > -1) continue;
			index += 6;
			while (isspace(input[index])) ++index;
			ClassStruct newClass;
			while (!isspace(input[index]))
			{
				newClass.identifier += input[index];
				++index;
			}
			while (input[index] != '{') ++index;
			++index;
			while (true)
			{
				if (input[index] == '}' && input[index + 1] == ';') break;
				newClass.block += input[index];
				++index;
			}
			output.push_back(newClass);
			++index;
		}
		return output;
	}
};

int main(int argc, const char ** argv)
{
	if (argc != 2)
	{
		wcout << L"Usage: ParseClassDeclarations [file to parse]\r\n";
		return -1;
	}
	ClassParser parser;
	auto input = parser.GetFile(argv[1]);
	auto classes = parser.GetClassDeclarations(input);
	wcout << L"The number of identified class declarations is: " << classes.size() << L"\r\n";
	return 0;
}