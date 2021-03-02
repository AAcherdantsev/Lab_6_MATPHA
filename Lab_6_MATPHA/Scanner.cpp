#include "Scanner.h"
#include <iostream>
#include <string.h> 

using namespace std;

Lexeme keywords[MAX_KEYWORDS] = 
{
		"do", "while", "short", "long",
		 "Main", "return",
		 "class", "boolean", "true", "false", "main"
};

int Key_Index[MAX_KEYWORDS] = 
{
		TYPE_DO, TYPE_WHILE, TYPE_SHORT, TYPE_LONG,
		TYPE_MAIN, TYPE_RETURN,
		TYPE_CLASS, TYPE_BOOL, TYPE_TRUE, TYPE_FALSE, TYPE_MAIN_FUNC
};

int Scanner::getPointer()
{
	return pointer;
}

void Scanner::printError(string error, string a)
{
	if (a[0] == '\0')
	{
		cout << "Error: " << error << endl;
		exit(0);
	}
	else
	{
		cout << "Error: " << error << ". Line: " << currentLine << ". Column: "
			<< currentColumn << ". Invalid character: " << a << endl;
		exit(1);
	}
	
}

void Scanner::readText(string fileName) {
	fstream inputFile(fileName);

	if (inputFile.fail()) {
		printError("File not found", "");
		exit(0);
	}

	char symbol;
	while (inputFile.get(symbol))
	{
		text[pointer++] = symbol;
		if (pointer > MAX_TEXT_SIZE - 1)
		{
			printError("The module is too large", "");
			break;
		}
	}

	text[pointer] = '\0';
	inputFile.close();
}

Scanner::Scanner(string fileName) 
{
	setPointer(0);
	readText(fileName);
	setPointer(0);
}

int Scanner::scan(Lexeme lexeme)
{
	int index = 0;
	// Очищаем лексему
	for (int i = 0; i < MAX_LEXEME_SIZE; i++)
		lexeme[i] = 0;
start:
	// Пропуск незначащих символов
	while ((text[pointer] == ' ') || (text[pointer] == '\n') || (text[pointer] == '\t')) {
		countLineAndColumn();
		pointer++;
	}
	// Пропуск однострочных комментариев
	if ((text[pointer] == '/') && (text[pointer + 1] == '/')) {
		pointer += 2;
		currentColumn += 2;
		while (text[pointer] != '\n') {
			// Если дошли до конца модуля
			if (text[pointer] == '\0')
				goto continuation;
			countLineAndColumn();
			pointer++;
		}
		goto start;
	}
	// Пропуск многострочных комментариев
	if ((text[pointer] == '/') && (text[pointer + 1] == '*')) {
		pointer += 2;
		currentColumn += 2;
		while (!((text[pointer] == '*') && (text[pointer + 1] == '/'))) {
			// Если дошли до конца модуля
			if (text[pointer] == '\0')
				goto continuation;
			countLineAndColumn();
			pointer++;
		}
		pointer += 2;
		currentColumn += 2;
		goto start;
	}
continuation:
	if (text[pointer] == '\0')
		return TYPE_END;
	// Шестнадцатиричные константы
	if (text[pointer] == '0' && (text[pointer + 1] == 'x' || text[pointer + 1] == 'X')) {
		lexeme[index++] = text[pointer++];
		lexeme[index++] = text[pointer++];
		currentColumn += 2;
		while (((text[pointer] >= '0') && (text[pointer] <= '9')) ||
			((text[pointer] >= 'a') && (text[pointer] <= 'f')) ||
			((text[pointer] >= 'A') && (text[pointer] <= 'F')))
		{
			if (index < MAX_LEXEME_SIZE - 1)
				lexeme[index++] = text[pointer++];
			else
				pointer++;
			currentColumn++;
		}
		if (index == MAX_LEXEME_SIZE - 1)
			printError("The lexem is too long. It was shortened", "");
		return TYPE_CONST_HEX;
	}

	// Десятичные константы
	else if (text[pointer] <= '9' && text[pointer] >= '0') {
		lexeme[index++] = text[pointer++];
		currentColumn++;
		while (text[pointer] <= '9' && text[pointer] >= '0') {
			if (index < MAX_LEXEME_SIZE - 1)
				lexeme[index++] = text[pointer++];
			else
				pointer++;
			currentColumn++;
		}
		if (index == MAX_LEXEME_SIZE - 1)
			printError("The lexem is too long. It was shortened", "");

		return TYPE_CONST_DECIMAL;
	}
	// Идентификаторы
	else if ((text[pointer] >= 'a') && (text[pointer] <= 'z') ||
		(text[pointer] >= 'A') && (text[pointer] <= 'Z') ||
		text[pointer] == '_') {
		lexeme[index++] = text[pointer++];
		currentColumn++;
		while ((text[pointer] >= 'a') && (text[pointer] <= 'z') ||
			(text[pointer] >= 'A') && (text[pointer] <= 'Z') ||
			(text[pointer] <= '9' && text[pointer] >= '0') ||
			text[pointer] == '_') {
			if (index < MAX_LEXEME_SIZE - 1)
				lexeme[index++] = text[pointer++];
			else
				pointer++;
			currentColumn++;
		}

		if (index == MAX_LEXEME_SIZE - 1)
			printError("The lexem is too long. It was shortened", "");
		// Ключевые слова 
		for (int i = 0; i < MAX_KEYWORDS; i++) {
			if (strcmp(lexeme, keywords[i]) == 0)
				return Key_Index[i];
		}

		return TYPE_IDENTIFIER;
	}
	// Знаки операций
	else if (text[pointer] == '<')
	{
		lexeme[index++] = text[pointer++];
		currentColumn++;
		if (text[pointer] == '=')
		{
			lexeme[index++] = text[pointer++];
			currentColumn++;
			return TYPE_LESS_EQUAL;
		}
		return TYPE_LESS;
	}

	else if (text[pointer] == '>') {
		lexeme[index++] = text[pointer++];
		currentColumn++;
		if (text[pointer] == '=') {
			lexeme[index++] = text[pointer++];
			currentColumn++;
			return TYPE_MORE_EQUAL;
		}
		return TYPE_MORE;
	}

	else if (text[pointer] == '=') {
		lexeme[index++] = text[pointer++];
		currentColumn++;
		if (text[pointer] == '=') {
			lexeme[index++] = text[pointer++];
			currentColumn++;
			return TYPE_EQUAL;
		}
		return TYPE_SAVE;
	}

	else if ((text[pointer] == '!') && (text[pointer + 1] == '=')) {
		lexeme[index++] = text[pointer++];
		lexeme[index++] = text[pointer++];
		currentColumn += 2;
		return TYPE_NOT_EQUAL;
	}

	else if (text[pointer] == '+') {
		lexeme[index++] = text[pointer++];
		currentColumn++;
		return TYPE_PLUS;
	}

	else if (text[pointer] == '-') {
		lexeme[index++] = text[pointer++];
		currentColumn++;
		return TYPE_MINUS;
	}

	else if (text[pointer] == '*') {
		lexeme[index++] = text[pointer++];
		currentColumn++;
		return TYPE_MULTIPLICATION;
	}

	else if (text[pointer] == '/') {
		lexeme[index++] = text[pointer++];
		currentColumn++;
		return TYPE_DIVISION;
	}

	// Спец. символы
	else if (text[pointer] == ',') {
		lexeme[index++] = text[pointer++];
		currentColumn++;
		return TYPE_COMMA;
	}

	else if (text[pointer] == ';') {
		lexeme[index++] = text[pointer++];
		currentColumn++;
		return TYPE_SEMICOLON;
	}

	else if (text[pointer] == '(') {
		lexeme[index++] = text[pointer++];
		currentColumn++;
		return TYPE_OPEN_PARENTHESES;
	}

	else if (text[pointer] == ')') {
		lexeme[index++] = text[pointer++];
		currentColumn++;
		return TYPE_CLOSED_PARENTHESES;
	}

	else if (text[pointer] == '{') {
		lexeme[index++] = text[pointer++];
		currentColumn++;
		return TYPE_OPEN_BRACES;
	}

	else if (text[pointer] == '}') {
		lexeme[index++] = text[pointer++];
		currentColumn++;
		return TYPE_CLOSED_BRACES;
	}

	else if (text[pointer] == '|' && text[pointer + 1] == '|')
	{
		lexeme[index++] = text[pointer++];
		lexeme[index++] = text[pointer++];
		currentColumn += 2;
		return TYPE_LOG_OR;
	}

	else if (text[pointer] == '&' && text[pointer + 1] == '&')
	{
		lexeme[index++] = text[pointer++];
		lexeme[index++] = text[pointer++];
		currentColumn += 2;
		return TYPE_LOG_AND;
	}

	else if (text[pointer] == '^')
	{
		lexeme[index++] = text[pointer++];
		currentColumn += 1;
		return TYPE_EXC_OR;
	}

	else if (text[pointer] == '%')
	{
		lexeme[index++] = text[pointer++];
		currentColumn += 1;
		return TYPE_DIV_REM;
	}

	else if (text[pointer] == '!' && text[pointer + 1] != '=')
	{
		lexeme[index++] = text[pointer++];
		currentColumn += 1;
		return TYPE_LOG_NOT;
	}

	// Неизвестный символ
	else
	{
		lexeme[index++] = text[pointer++];
		currentColumn++;
		printError("Найден неверный символ", lexeme);
		return TYPE_ERROR;
	}
}

void Scanner::countLineAndColumn()
{
	if (text[pointer] == ' ')
		currentColumn++;
	else if (text[pointer] == '\t')
		currentColumn += 4;
	else if (text[pointer] == '\n')
	{
		currentColumn = 0;
		currentLine++;
	}
}


int Scanner::getCurrentLine() const 
{
    return currentLine;
}

void Scanner::setCurrentLine(int line) 
{
    Scanner::currentLine = line;
}

int Scanner::getCurrentColumn() const 
{
    return currentColumn;
}

void Scanner::setCurrentColumn(int column) 
{
    Scanner::currentColumn = column;
}

void Scanner::setPointer(int index)
{
	pointer = index;
}