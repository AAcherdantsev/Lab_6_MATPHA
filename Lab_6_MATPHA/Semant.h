#ifndef ANALYZER_SEMANT_H
#define ANALYZER_SEMANT_H
#include "defs.h"
#include "Scanner.h"
#include <afxres.h>
#include <algorithm>

// Тип данных
enum  DataType
{
	TYPE_SHORT_INT = 0,
	TYPE_LONG_INT = 1,
	TYPE_BOOLEAN = 2,
	TYPE_UNKNOWN = 3
};

// Значения поля данных
union DataValue
{
	short int dataForShort;
	long long int dataForLong;
};

// Семантические типы
enum ObjectType
{
	TYPE_FUNCTION,
	TYPE_VARIABLE
};

struct Data
{
	DataType typeOfData;
	DataValue valueOfData;
};

// Узел дерева
struct Node
{
	Lexeme ID;
	int objectType;
	Data data;
	int amountVarForFunc;
	int pointerForFunc;
};

class Tree 
{
	Node *node;
	Tree *parent; 
	Tree *left;
	Tree *right;
	static Scanner *scanner;
public:
	static bool flagInter;
	static Tree *current;
	// Функции обработки дерева
	Tree   *findUp(Tree *from, Lexeme ID);
	Tree   *findUpInsideOneArea(Tree *from, Lexeme ID);
	Tree   *findUp(Lexeme ID);
	Tree   *getFirstParamOfFunc(Tree *func);
	Tree   *getLeft(Tree *elem);
	Tree   *getLeft();
	Tree   *getParent();
	Tree   *getRight();

	void   setLeft(Node *data);
	void   setRight(Node *data);
	void   deleteVariables(Tree* finish);
	void   destroyLocalTree(Tree* tree);

	// Семантические подпрограммы	
	Tree   *getCurrent();
	Tree   *includeVar(Lexeme ID, DataType type);
	Tree   *includeFunc(Lexeme ID, DataType type);
	Tree   *makeScope();
	DataType getConstType(Lexeme constLexeme);
	Data   getExpressionType(Data firstType, Data secondType);
	void showVarInfo(DataType datatype, Lexeme lexeme);
	void assignValueToVariable(Data expressionData, Lexeme variableID);
	void showFuncInfo(Lexeme lexeme);
	Node* getNode();
	Data   getTypeByCastTable(Data firstType, Data  secondType);
	Tree   *SemGetFunct(Lexeme a); // найти в таблице функцию с именем a и вернуть ссылку на соответствующий элемент дерева
	bool   IDcontrol(Tree *elem, Lexeme ID);
	void   setCurrent(Tree *elem);
	Node*  getObjectFromTree(Lexeme ID);
	void   setVarValueAndType(Data newData, Lexeme ID);
	void   setVarValue(Data newData, Lexeme ID);
	void   SemSetParam(Tree* Addr, int num);
	void   SemControlParam(Tree *Addr, int num);
	bool   assigmentControl(DataType IDType, DataType expressionType);
	static void setScanner(Scanner *scanner);
	Tree(Tree *left, Tree *right, Tree *parent, Node *data);
	Tree();
	~Tree();
};
#endif //ANALYZER_SEMANT_H