#include <fstream>
#include <iostream>
#include <cctype>
#include <vector>

using namespace std;

struct FunctionDeclaration
{
	string returnType;
	string identifier;
	string arguments;
	string body;
};

struct FunctionParser
{
	string GetFile(string input) const
	{
		ifstream file(input);
		string output;
		char buffer;
		while (file.get(buffer)) output += buffer;
		return output;
	}

	void swap(char* first, char* second) const
	{
		auto temp = move(*second);
		*second = move(*first);
		*first = move(temp);
	}

	vector<FunctionDeclaration> ParseFunctions(string input) const
	{
		vector<FunctionDeclaration> output;
		unsigned int index = 0;
		unsigned int scope = 0;
		while (true)
		{
			if (index >= input.length()) break;
			if (input[index] != '(')
			{
				++index;
				continue;
			}
			if (isspace(input[index])) break;
			FunctionDeclaration declaration;
			auto identifierIndex = index - 1;
			string identifier;
			while(!isspace(input[identifierIndex]))
			{
				identifier += input[identifierIndex];
				--identifierIndex;
			}
			auto head = 0;
			auto tail = identifier.length() - 1;
			while (static_cast<unsigned int>(head) < tail)
			{
				swap(&identifier[head], &identifier[tail]);
				++head;
				--tail;
			}
			declaration.identifier = move(identifier);
			--identifierIndex;
			string returnType;
			while(!isspace(input[identifierIndex]))
			{
				returnType += input[identifierIndex];
				--identifierIndex;
			}
			head = 0;
			tail = returnType.length() - 1;
			while (static_cast<unsigned int>(head) < tail)
			{
				swap(&returnType[head], &returnType[tail]);
				++head;
				--tail;
			}
			declaration.returnType = move(returnType);
			string arguments;
			++index;
			while (input[index] != ')')
			{
				declaration.arguments += input[index];
				++index;
			}
			while (input[index] != '{') ++index;
			++scope;
			declaration.body += '{';
			++index;
			while (scope)
			{
				if (input[index] == '{') ++scope;
				if (input[index] == '}') --scope;
				declaration.body += input[index];
				++index;
			}
			output.push_back(declaration);
		}
		return output;
	}
};

int main()
{
	FunctionParser parser;
	auto input = parser.GetFile("D:\\Main.cpp");
	auto declarations = parser.ParseFunctions(input);
	wcout << L"The total number of function definitions is: " << declarations.size() << L"\r\n";
	return 0;
}