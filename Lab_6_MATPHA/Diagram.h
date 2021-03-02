#ifndef ANALYZER_DIAGRAM_H
#define ANALYZER_DIAGRAM_H

#include "Scanner.h"
#include "Semant.h"

class Diagram 
{
private:
    Scanner *scanner;
	void setAll(int pointer, int line, int column);
	void getAll(int *pointer, int *line, int *column);

	// далее функции, реализующие диаграммы
public:
	void classMain();				// класс Main
	void classContent();			// содержание класса
	void description();				// описание
	void data();					// дата
	void variablesList(DataType dataType);	// список переменных
	void variables(DataType dataType);		// переменная
	Data function(bool NeedToInterpActions);				// функция
	int  varForFunc(Tree * treeFunc);// переменные для описания функции
	Data compositeOperator();		// составной оператор
	Data operatorsAndDescription();	// операторы и описания
	Data operatorD();				// оператор
	void assigment();				// присваивание
	Data functionCall();		// вызов функции
	int  listOfExpression(Tree *func);// список выражений (для передачи в функцию в качестве параметра(ов))
	Data loop();						// цикл do - while

	Data expression();				// выражение
	Data logAnd();					// логическое И
	Data excOr();					// исключающее ИЛИ
	Data equality();				// равенство/неравенство
	Data comparison();				// сравнения (>=, <=, <, >)
	Data addend(bool minusWasHere);	// слагаемое
	Data multiplier();				// множитель
	Data elementaryExpression();	// элементарное выражение

    void program();
	Tree *root;
	explicit Diagram(Scanner *scanner);
    ~Diagram();
};

#endif //ANALYZER_DIAGRAM_H