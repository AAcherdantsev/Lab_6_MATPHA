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

	// ����� �������, ����������� ���������
public:
	void classMain();				// ����� Main
	void classContent();			// ���������� ������
	void description();				// ��������
	void data();					// ����
	void variablesList(DataType dataType);	// ������ ����������
	void variables(DataType dataType);		// ����������
	Data function(bool NeedToInterpActions);				// �������
	int  varForFunc(Tree * treeFunc);// ���������� ��� �������� �������
	Data compositeOperator();		// ��������� ��������
	Data operatorsAndDescription();	// ��������� � ��������
	Data operatorD();				// ��������
	void assigment();				// ������������
	Data functionCall();		// ����� �������
	int  listOfExpression(Tree *func);// ������ ��������� (��� �������� � ������� � �������� ���������(��))
	Data loop();						// ���� do - while

	Data expression();				// ���������
	Data logAnd();					// ���������� �
	Data excOr();					// ����������� ���
	Data equality();				// ���������/�����������
	Data comparison();				// ��������� (>=, <=, <, >)
	Data addend(bool minusWasHere);	// ���������
	Data multiplier();				// ���������
	Data elementaryExpression();	// ������������ ���������

    void program();
	Tree *root;
	explicit Diagram(Scanner *scanner);
    ~Diagram();
};

#endif //ANALYZER_DIAGRAM_H