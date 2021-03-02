#include "Semant.h"
#include <string>
#include <stdio.h>
#include <iostream>
#include <iomanip>

#define TYPE_EMPTY -1

Tree::Tree(Tree *left, Tree *right, Tree *parent, Node *data) 
{
	node = new Node();
	this->parent = parent;
	this->left = left;
	this->right = right;
	memcpy(node, data, sizeof(Node));
	node->data.valueOfData.dataForLong = 0;
}

Tree::Tree() 
{
	node = new Node();
	this->parent = nullptr;
	this->left = nullptr;
	this->right = nullptr;
	memcpy(node, &("-root-"), sizeof(Node));
	node->data.valueOfData.dataForLong = 0;
}

void Tree::setScanner(Scanner *scan) 
{
	scanner = scan;
}

Tree* Tree::getFirstParamOfFunc(Tree *func)
{
	return func->right->left;
}

Tree *Tree::getLeft(Tree *elem)
{
	return elem->left;
}

Tree* Tree::getLeft()
{
	return this->left;
}

Tree* Tree::getParent()
{
	return this->parent;
}

Tree* Tree::getRight()
{
	return this->right;
}

void Tree::setLeft(Node *data) 
{
	Tree *newLeft = new Tree(nullptr, nullptr, this, data);
	this->left = newLeft;
}

void Tree::setRight(Node *data) 
{
	Tree *newRight = new Tree(nullptr, nullptr, this, data);
	this->right = newRight;
}

void Tree::deleteVariables(Tree* tree)
{
	if (!flagInter) return;
	if (tree == NULL) return;
	if (strcmp(tree->node->ID, "-root-") == 0)
	{
		destroyLocalTree(tree->right);
		destroyLocalTree(tree->left);
	}
	else
	{
		int amountVarForFunc = tree->node->amountVarForFunc;
		if (amountVarForFunc == 0)
		{
			destroyLocalTree(tree->right);
			tree->right = nullptr;
		}
		else
		{
			tree = tree->right;
			for (int i = 0; i < amountVarForFunc; i++)
				tree = tree->left;
			destroyLocalTree(tree->left);
		}
	}
}

void Tree::destroyLocalTree(Tree* tree)
{
	if (tree != NULL)
	{
		if (tree->left != NULL)
			destroyLocalTree(tree->left);
		if (tree->right != NULL)
			destroyLocalTree(tree->right);
		if (tree->parent != NULL)
		{
			tree->parent->left = NULL;
			tree->parent->right = NULL;
		}
		free(tree);
	}
}

// Поиск вершины вверх по связям от fromNode
Tree *Tree::findUp(Tree *fromNode, Lexeme ID) 
{
	Tree *result = fromNode;
	while ((result != nullptr) &&
		(strcmp(ID, result->node->ID) != 0))
		result = result->parent;
	return result;
}

// Поиск вершины в одной области видимости
Tree *Tree::findUpInsideOneArea(Tree *from, Lexeme ID) 
{
	Tree *result = from;
	while ((result != nullptr) && (result->parent != nullptr) && (result->parent->right != result)) 
	{
		if (memcmp(ID, result->node->ID,
			max(strlen(ID), strlen(result->node->ID))) == 0)
			return result;
		result = result->parent;
	}
	return nullptr;
}

Tree *Tree::findUp(Lexeme ID) 
{
	return findUp(this, ID);
}

void Tree::setCurrent(Tree *elem) 
{
	current = elem;
}

Tree*Tree::getCurrent() 
{
	return current;
}

// Добавление переменной в таблицу
Tree *Tree::includeVar(Lexeme ID, DataType type) 
{
	if (IDcontrol(current, ID))
		scanner->printError("A function / variable with this name already exists", ID);
	Node *tempElement = new Node();
	memcpy(tempElement->ID, ID, strlen(ID) + 1);
	tempElement->data.typeOfData  = type;
	tempElement->objectType = TYPE_VARIABLE;
	if (type == TYPE_SHORT_INT)
		tempElement->data.valueOfData.dataForShort = 0;
	if (type == TYPE_LONG_INT)
		tempElement->data.valueOfData.dataForLong = 0;
	current->setLeft(tempElement);
	current = current->left;
	return current;
}

// Добавление функции в таблицу
Tree *Tree::includeFunc(Lexeme ID, DataType type)
{
	if (IDcontrol(current, ID))
		scanner->printError("A function / variable with this name already exists", ID);
	Node *tempElement = new Node();
	memcpy(tempElement->ID, ID, strlen(ID) + 1);
	tempElement->data.typeOfData = type;
	tempElement->objectType = TYPE_FUNCTION;

	if (type == TYPE_SHORT_INT)
		tempElement->data.valueOfData.dataForShort = 0;
	if (type == TYPE_LONG_INT)
		tempElement->data.valueOfData.dataForLong = 0;

	tempElement->amountVarForFunc = 0;
	current->setLeft(tempElement);
	current = current->left;
	Tree *result = current;
	Node *empty = new Node();
	memcpy(empty->ID, &"-empty-", strlen("-empty-") + 1);
	empty->data.typeOfData = TYPE_UNKNOWN;
	empty->objectType = TYPE_EMPTY;
	empty->data.valueOfData.dataForLong = NULL;/////////////////Long
	current->setRight(empty);
	current = current->right;
	return result;
}

// Добавление области видимости в таблицу
Tree *Tree::makeScope() 
{
	Node *emptyNode = new Node();
	memcpy(emptyNode->ID, &"-empty-", strlen("-empty-") + 1);
	emptyNode->data.typeOfData = TYPE_UNKNOWN;
	emptyNode->objectType = TYPE_EMPTY;
	emptyNode->data.valueOfData.dataForLong = NULL;/////////////////Long
	current->setLeft(emptyNode);
	current = current->left;
	Tree *pointer = current;
	current->setRight(emptyNode);
	current = current->right;
	return pointer;
}

// Возвращает true, если есть дубликат ID
bool Tree::IDcontrol(Tree *elem, Lexeme ID) {
	if (findUpInsideOneArea(elem, ID) == nullptr)
		return false;
	return true;
}


// Возвращает тип константы
DataType Tree::getConstType(Lexeme constLexeme) 
{
	if (strcmp(constLexeme, "true") == 0 || strcmp(constLexeme, "false") == 0)
		return TYPE_BOOLEAN;
	long long number = stoll(constLexeme, nullptr, 10);
	if (number > -32768 && number < 32767)
		return TYPE_SHORT_INT;
	if (number > -2147483647 && number < 2147483647)
		return TYPE_LONG_INT;
}

Node* Tree::getObjectFromTree(Lexeme ID)
{
	Tree* result = this->findUp(this->getCurrent(), ID);
	return result->node;
}

void Tree::setVarValueAndType(Data newData, Lexeme ID)
{
	Tree* result = this->findUp(this->getCurrent(), ID);

	if (result == nullptr)
		scanner->printError("Identifier was not declared", ID);

	if (result->node->objectType == TYPE_FUNCTION)
		scanner->printError("Identifier is declared as a function", ID);

	result->node->data.typeOfData = newData.typeOfData;
	if (newData.typeOfData == TYPE_SHORT_INT)
		result->node->data.valueOfData.dataForShort = newData.valueOfData.dataForShort;

	if (newData.typeOfData == TYPE_LONG_INT)
		result->node->data.valueOfData.dataForLong = newData.valueOfData.dataForLong;
}

void Tree::setVarValue(Data newData, Lexeme ID)
{
	Tree* result = this->findUp(this->getCurrent(), ID);

	if (result == nullptr)
		scanner->printError("Identifier was not declared", ID);

	if (result->node->objectType == TYPE_FUNCTION)
		scanner->printError("Identifier is declared as a function", ID);

	if (result->node->data.typeOfData == TYPE_SHORT_INT)
	{
		result->node->data.valueOfData.dataForShort = newData.valueOfData.dataForShort;
		result->node->data.valueOfData.dataForShort = newData.valueOfData.dataForLong;
	}

	if (result->node->data.typeOfData == TYPE_LONG_INT)
	{
		result->node->data.valueOfData.dataForLong = newData.valueOfData.dataForLong;
		result->node->data.valueOfData.dataForLong = newData.valueOfData.dataForShort;

	}
}

// Получение типа выражения
Data Tree::getExpressionType(Data firstType, Data secondType) 
{

	if (firstType.typeOfData == TYPE_BOOLEAN && secondType.typeOfData == TYPE_BOOLEAN)
	{
		firstType.typeOfData = TYPE_BOOLEAN;
		return firstType;
	}
	if (secondType.typeOfData == TYPE_UNKNOWN)
	{
		return firstType;
	}
	else
	{
		if (firstType.typeOfData == secondType.typeOfData)
		{
			return firstType;
		}
		if (firstType.typeOfData != TYPE_LONG_INT)
		{
			firstType.typeOfData = TYPE_LONG_INT;
			firstType.valueOfData.dataForLong = firstType.valueOfData.dataForShort;
		}
		return firstType;
	}
}

void Tree::showVarInfo(DataType dataType, Lexeme lexeme)
{
	if (!flagInter) return;
	cout << "Initialization variable: ";
	if (dataType == TYPE_SHORT_INT)
		cout << " (short) ";
	if (dataType == TYPE_LONG_INT)
		cout << " (long)  ";
	if (dataType == TYPE_BOOLEAN)
		cout << " (boolean) ";
	if (dataType == TYPE_UNKNOWN)
		cout << " (unknow)  ";
	cout << lexeme << endl;
}


void Tree::assignValueToVariable(Data expressionData, Lexeme variableID)
{
	if (!flagInter) return;
	DataType variableType = getObjectFromTree(variableID)->data.typeOfData;

	assigmentControl(variableType, expressionData.typeOfData);
	setVarValueAndType(getTypeByCastTable(getObjectFromTree(variableID)->data, expressionData), variableID);
	variableType = getObjectFromTree(variableID)->data.typeOfData;
	setVarValue(expressionData, variableID);
	cout << "Assignment variable: ";
	if (variableType == TYPE_SHORT_INT)
		cout << " (short) ";
	if (variableType == TYPE_LONG_INT)
		cout << " (long)  ";
	if (variableType == TYPE_BOOLEAN)
		cout << " (boolean) ";
	if (variableType == TYPE_UNKNOWN)
		cout << " (unknow)  ";
	cout << variableID << " = ";
	if (variableType == TYPE_SHORT_INT)
		cout << getObjectFromTree(variableID)->data.valueOfData.dataForShort << endl;
	if (variableType == TYPE_LONG_INT)
		cout << getObjectFromTree(variableID)->data.valueOfData.dataForLong << endl;
}

void Tree::showFuncInfo(Lexeme lexeme)
{
	DataType dataType = getObjectFromTree(lexeme)->data.typeOfData;
	cout << "Added function: ";
	if (dataType == TYPE_SHORT_INT)
		cout << " (short) " << lexeme << " = " << getObjectFromTree(lexeme)->data.valueOfData.dataForShort << endl;
	if (dataType == TYPE_LONG_INT)
		cout << " (long)  " << lexeme << " = " << getObjectFromTree(lexeme)->data.valueOfData.dataForLong << endl;
	if (dataType == TYPE_BOOLEAN)
		cout << " (boolean) " << endl;
	if (dataType == TYPE_UNKNOWN)
		cout << " (unknow)  " << endl;
}

Node* Tree::getNode()
{
	return this->node;
}

// Возвращает тип в соотвествии с таблицей приведения
Data Tree::getTypeByCastTable(Data firstType, Data secondType)
{
	if (firstType.typeOfData == TYPE_BOOLEAN && secondType.typeOfData == TYPE_BOOLEAN)
	{
		firstType.typeOfData = TYPE_BOOLEAN;
		return firstType;
	}
	else if
		(
			(firstType.typeOfData == TYPE_SHORT_INT && secondType.typeOfData == TYPE_SHORT_INT) ||
			(firstType.typeOfData == TYPE_SHORT_INT && secondType.typeOfData == TYPE_LONG_INT) ||
			(firstType.typeOfData == TYPE_LONG_INT && secondType.typeOfData == TYPE_SHORT_INT) ||
			(firstType.typeOfData == TYPE_LONG_INT && secondType.typeOfData == TYPE_LONG_INT)
			)
	{
		if (firstType.typeOfData == secondType.typeOfData)
		{
			return firstType;
		}
		if (firstType.typeOfData != TYPE_LONG_INT)
		{
			firstType.typeOfData = TYPE_LONG_INT;
			firstType.valueOfData.dataForLong = firstType.valueOfData.dataForShort;
		}
		return firstType;
	}
	Tree::scanner->printError("Boolean cast error", "");
}

// Контроль приведения типов
bool Tree::assigmentControl(DataType IDType, DataType expressionType) 
{
	if (IDType == TYPE_BOOLEAN && expressionType == TYPE_BOOLEAN)
		return true;
	else if (
		(IDType == TYPE_SHORT_INT && expressionType == TYPE_SHORT_INT) ||
		(IDType == TYPE_SHORT_INT && expressionType == TYPE_LONG_INT) ||
		(IDType == TYPE_LONG_INT && expressionType == TYPE_SHORT_INT) ||
		(IDType == TYPE_LONG_INT && expressionType == TYPE_LONG_INT)
		)
		return true;
	Tree::scanner->printError("Typecast error", "");
	return false;
}

// установить число формальных параметров n для функции по адресу Addr
void Tree::SemSetParam(Tree* Addr, int num)
{
	Addr->node->amountVarForFunc = num;
}

// проверить равенство числа формальных параметров значению
// num для функции по адресу Addr
void Tree::SemControlParam(Tree *Addr, int num)
{
	if (!flagInter) return;
	if (num != Addr->node->amountVarForFunc)
		Tree::scanner->printError("Invalid number of parameters for the function ", Addr->node->ID);
}

Tree * Tree::SemGetFunct(Lexeme a)
// найти в таблице функцию с именем a
// и вернуть ссылку на соответствующий элемент дерева.
{
	Tree * v = findUp(current, a);
	if (v == NULL)
		Tree::scanner->printError("Missing function description ", a);
	if (v->node->objectType != TYPE_FUNCTION)
		Tree::scanner->printError("ID is not a function ", a);
	return v;
}
Tree::~Tree() = default;
Tree* Tree::current = nullptr;
Scanner* Tree::scanner = nullptr;
bool Tree::flagInter = true;