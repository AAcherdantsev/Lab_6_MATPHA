#include "pch.h"
#include <fstream>

#include "../Lab_6_MATPHA/Scanner.cpp"


void prepareFile(string  fileName)
{
	string words[] =
	{
		"class",
		"Main",
		"do",
		"while",
		"short",
		"long" ,
		"boolean",
		"true",
		"false",
		"return",
		"main",

		"testIdentifier",

		"1234" ,
		"0xFFFE",

		",",
		";",
		"(",
		")",
		"{",
		"}",

		"<" ,
		"<=",
		">" ,
		">=",
		"==",
		"!=",
		"+" ,
		"-" ,
		"*" ,
		"/" ,
		"=" ,

		"||" ,
		"&&" ,
		"^" ,
		"%" ,
		"!" ,
	};
	ofstream fileWritter;
	fileWritter.open(fileName);
	for (int i = 0; i < size(words); i++)
	{
		fileWritter << words[i] << " ";
	}
	fileWritter.close();
}

bool CopmareArrays(int first[], int second[], int len)
{
	for (int i = 0; i < len; i++)
		if (first[i] != second[i])
			return false;
	return true;
}

TEST(TestScanner, TestDetectSymbols)
{
	string fileName = "testInput.txt";
	const int amountWords = 36;
	Lexeme lexeme;

	int type;
	prepareFile(fileName);
	int expected[amountWords];
	for (int i = 0; i < amountWords; i++)
		expected[i] = i + 1;

	int actual[amountWords];
	Scanner* scanner = new Scanner(fileName);
	for (int i = 0; i < amountWords; i++)
		actual[i] = scanner->scan(lexeme);

	remove(fileName.c_str());
	EXPECT_TRUE(CopmareArrays(expected, actual, amountWords));
}

TEST(TestScanner, TestIgnoreComments)
{
	string fileName = "testIgnoreComments.txt";
	ofstream fileWritter;
	fileWritter.open(fileName);
	fileWritter << "someVarName // some one string comment" << endl;
	fileWritter << "\t Main /* some " << endl;
	fileWritter << "three strings " << endl;
	fileWritter << "comment */" << endl;
	fileWritter << "return";
	fileWritter.close();
	Scanner* scanner = new Scanner(fileName);
	Lexeme lexeme;
	int expected[] = { 12, 2, 10 };
	int actual[3];
	for (int i = 0; i < size(actual); i++)
		actual[i] = scanner->scan(lexeme);

	EXPECT_TRUE(CopmareArrays(expected, actual, 3));
}

TEST(TestScanner, TestPointer)
{
	string fileName = "testPointer.txt";
	int expected = 15;

	prepareFile(fileName);
	Scanner* scanner = new Scanner(fileName);

	scanner->setPointer(expected);
	int actual = scanner->getPointer();

	remove(fileName.c_str());
	EXPECT_EQ(actual, expected);
}

TEST(TestScanner, TestLine)
{
	string fileName = "testLine.txt";
	int expected = 10;

	prepareFile(fileName);
	Scanner* scanner = new Scanner(fileName);

	scanner->setCurrentLine(expected);
	int actual = scanner->getCurrentLine();

	remove(fileName.c_str());
	EXPECT_EQ(actual, expected);
}

TEST(TestScanner, TestColumn)
{
	string fileName = "testLine.txt";
	int expected = 10;
	prepareFile(fileName);
	Scanner* scanner = new Scanner(fileName);

	scanner->setCurrentColumn(expected);
	int actual = scanner->getCurrentColumn();

	remove(fileName.c_str());
	EXPECT_EQ(actual, expected);
}

TEST(TestScanner, TestError)
{
	string fileName = "testPrintError.txt";
	prepareFile(fileName);
	Scanner* scanner = new Scanner(fileName);

	remove(fileName.c_str());
	EXPECT_EXIT(scanner->printError("Some Error", ""), ::testing::ExitedWithCode(0), "");
}

TEST(TestScanner, TestErrorWithSymbol)
{
	string fileName = "testPrintError.txt";
	prepareFile(fileName);
	Scanner* scanner = new Scanner(fileName);
	remove(fileName.c_str());
	EXPECT_EXIT(scanner->printError("Some Error", "}"), ::testing::ExitedWithCode(1), "");
}
