#ifndef ANALYZER_SCANNER_H
#define ANALYZER_SCANNER_H
#include "defs.h"

class Scanner 
{
private:
    Text text;
    int pointer;
    int currentLine = 1;
    int currentColumn = 1;
    void countLineAndColumn();
public:
	void setPointer(int index); 
    int getPointer();
    int getCurrentLine() const;
    void setCurrentLine(int line);
    int getCurrentColumn() const;		
    void setCurrentColumn(int column);	
    void printError(string, string);	// Печать ошибки
    void readText(string);				// Считывание текста
    int scan(Lexeme lexeme);			// Выделение лексемы
	Scanner(string);					// 
	~Scanner() = default;				
};
#endif //ANALYZER_SCANNER_H