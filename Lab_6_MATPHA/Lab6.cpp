#include "defs.h"
#include "Scanner.h"
#include "Diagram.h"
#include "Semant.h"
#include <iostream>

int main(int argc, char *argv[]) 
{
	system("color F0");  
	string fileName = "input.dat";
	Scanner *scanner = new Scanner(fileName);
	Diagram *diagram = new Diagram(scanner);	
	diagram->program();
	Lexeme lexeme;
	int type = scanner->scan(lexeme);
	if (type == TYPE_END)
	{
		cout << "No syntax errors were found." << endl;
	}
	else
		scanner->printError("Additional text found at the end of the program", "");
	return 0;
}