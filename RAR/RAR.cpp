// RAR.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "windows.h"
#include <iostream>

using namespace std;

char fileName[260];
char fileSizeInChar[10];
long fileSize;

long getfileSize(FILE *file)
{
	long length;
	fseek(file, 0, SEEK_END);
	length = ftell(file);
	fseek(file, 0, SEEK_SET);
	return length;
}

void convertLongToChar()
{
	_itoa_s(fileSize, fileSizeInChar, 10, 10);
	strcat_s(fileSizeInChar, 10, "\r\n");
}

void addEndlineToChar(FILE* currentFile, char* parameters)
{
	fileSize = getfileSize(currentFile);
	strcpy_s(fileName, 260, parameters);
	strcat_s(fileName, 260, "\r\n");
}

void archive(FILE* inFile, FILE* outFile)
{
	fwrite(fileName, sizeof(char), strlen(fileName), outFile);
	fwrite(fileSizeInChar, sizeof(char), strlen(fileSizeInChar), outFile);
	const int size = 256;
	char mas[size];
	size_t read;
	while ((read = fread(mas, sizeof(char), size, inFile)) != 0)
	{
		fwrite(mas, sizeof(char), read, outFile);
	}
}

char* getFileName(FILE* outFile, char* parameters)
{
	char fName[260];
	fgets(fName, strlen(fName), outFile);
	return fName;
}

long getFileSize(FILE* outFile, char* parameters)
{
	char fSizeChar[10];
	fgets(fSizeChar, strlen(fSizeChar), outFile);
	long fSize = atoi(fSizeChar);
	return fSize;
}

char* replaceUnnesserySymbols(char* dest, char* source)
{
	char* symbol;
	do
	{
		symbol = strstr(dest, source);
		if (symbol != NULL)
		{
			char* symbol_ = symbol + strlen(source);
			strcpy_s(symbol, strlen(symbol), symbol_);
		}
		else break;
	} while (true);
	return dest;
}

void dearchive(FILE* inFile, FILE* outFile)
{
	char *masR = new char[fileSize];
	size_t readR;
	readR = fread(masR, sizeof(char), fileSize, outFile);
	fwrite(masR, sizeof(char), readR, inFile);
}

int main(int argc, char* argv[])
{
	FILE *inFile;
	FILE *outFile;

	if (argc > 2)
	{
		fopen_s(&outFile, "archive.krtv", "wb");

		for (int i = 1; i < argc; i++)
		{
			fopen_s(&inFile, argv[i], "rb");

			addEndlineToChar(inFile, argv[i]);
			convertLongToChar();

			archive(inFile, outFile);

			fclose(inFile);
		}

		fclose(outFile);
	}

	if (argc == 2)
	{
		fopen_s(&outFile, argv[1], "rb");

		while (true)
		{
			strcpy_s(fileName, 260, getFileName(outFile, argv[1]));
			fileSize = getFileSize(outFile, argv[1]);
			if (fileSize == 0)
				break;
			
			char *symbols = "\r\n";
			replaceUnnesserySymbols(fileName, symbols);
			fopen_s(&inFile, fileName, "wb");
			dearchive(inFile, outFile);
			fclose(inFile);
		}

		fclose(outFile);
	}

	if (argc == 1)
	{
		cout << "To archive: RAR README.txt debug.txt release.txt" << endl;
		cout << "To dearchive: RAR archive.krtv" << endl;
	}

	return 0;
}

