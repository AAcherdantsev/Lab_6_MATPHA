#include "pch.h"
#include <fstream>

#include "../Lab_6_MATPHA/Diagram.cpp"
#include "../Lab_6_MATPHA/Diagram.h"

#include "../Lab_6_MATPHA/Semant.cpp"
#include "../Lab_6_MATPHA/Semant.h"

class TreeTests : public ::testing::Test
{
protected:
	Scanner* scanner;
	Diagram* diagram;
	void SetUp()
	{
		string fileName = "testSimpleProgram.txt";

		string sampleOfTextProgram = R"(  
	class Main 
	{
		long add(long a, long b)
		{
			return a + b;
		}

		short pow2(short d)
		{
			return d * d;
		}

		long func()
		{
			long var = 10;
			var = var - 10;
			var = var - var + var + 1;
			return add(var, 5);
		
		}

		long funcWithLoop()
		{
			long varForLoop = 0;
			do
			{
				varForLoop = varForLoop + 2;
			} while (varForLoop <= 10);
			return varForLoop;
		}

		long main()
		{
			long varInMain = 10;
			varInMain = add(101, 4);
			varInMain = pow2(10);
			varInMain = func();
			varInMain = funcWithLoop();
		}
	};)";

		ofstream fout;
		fout.open(fileName);
		fout << sampleOfTextProgram << endl;
		fout.close();

		scanner = new Scanner(fileName);
		diagram = new Diagram(scanner);
		diagram->program();
		remove(fileName.c_str());
	}
	void TearDown()
	{

	}
};


void prepareData(Data *firstData, Data *secondData, DataType type, int firsValue, int secondValue)
{
	firstData->typeOfData = secondData->typeOfData = type;
	if (type == TYPE_LONG_INT)
	{
		firstData->valueOfData.dataForLong = firsValue;
		secondData->valueOfData.dataForLong = secondValue;
	}
	if (type == TYPE_SHORT_INT)
	{
		firstData->valueOfData.dataForShort = firsValue;
		secondData->valueOfData.dataForShort = secondValue;
	}
}

TEST(TestDiagram, TestOperatorPlus)
{
	Data firstOperand, secondOperand;
	int first = 5, second = 10;

	prepareData(&firstOperand, &secondOperand, TYPE_SHORT_INT, first, second);
	int expected = first + second;
	int actual = (firstOperand + secondOperand).valueOfData.dataForShort;

	EXPECT_TRUE(actual == expected) << "Short data";

	prepareData(&firstOperand, &secondOperand, TYPE_LONG_INT, first, second);
	actual = (firstOperand + secondOperand).valueOfData.dataForLong;
	
	EXPECT_TRUE(actual == expected) << "Long data";
}

TEST(TestDiagram, TestOperatorMinus)
{
	Data firstOperand, secondOperand;
	int first = 5, second = 10;

	prepareData(&firstOperand, &secondOperand, TYPE_SHORT_INT, first, second);
	int expected = first - second;
	int actual = (firstOperand - secondOperand).valueOfData.dataForShort;

	EXPECT_TRUE(actual == expected) << "Short data";

	prepareData(&firstOperand, &secondOperand, TYPE_LONG_INT, first, second);
	actual = (firstOperand - secondOperand).valueOfData.dataForLong;

	EXPECT_TRUE(actual == expected) << "Long data";
}

TEST(TestDiagram, TestOperatorMul)
{
	Data firstOperand, secondOperand;
	int first = 5, second = 10;

	prepareData(&firstOperand, &secondOperand, TYPE_SHORT_INT, first, second);
	int expected = first * second;
	int actual = (firstOperand * secondOperand).valueOfData.dataForShort;

	EXPECT_TRUE(actual == expected) << "Short data";

	prepareData(&firstOperand, &secondOperand, TYPE_LONG_INT, first, second);
	actual = (firstOperand * secondOperand).valueOfData.dataForLong;

	EXPECT_TRUE(actual == expected) << "Long data";
}

TEST(TestDiagram, TestOperatorDiv)
{
	Data firstOperand, secondOperand;
	int first = 10, second = 5;

	prepareData(&firstOperand, &secondOperand, TYPE_SHORT_INT, first, second);
	int expected = first / second;
	int actual = (firstOperand / secondOperand).valueOfData.dataForShort;

	EXPECT_TRUE(actual == expected) << "Short data";

	prepareData(&firstOperand, &secondOperand, TYPE_LONG_INT, first, second);
	actual = (firstOperand / secondOperand).valueOfData.dataForLong;

	EXPECT_TRUE(actual == expected) << "Long data";
}

TEST(TestDiagram, TestOperatorCompare)
{
	Data firstOperand, secondOperand;
	int first = 10;

	prepareData(&firstOperand, &secondOperand, TYPE_SHORT_INT, first, first);
	EXPECT_TRUE(firstOperand == secondOperand) << " 10 == 10 (short) (short)";
	EXPECT_FALSE(firstOperand > secondOperand) << " 10 > 10 (short) (short)";
	EXPECT_FALSE(firstOperand < secondOperand) << " 10 < 10 (short) (short)";



	prepareData(&firstOperand, &secondOperand, TYPE_SHORT_INT, first, first + 10);
	EXPECT_FALSE(firstOperand == secondOperand) << " 10 == 20 (short) (short)";
	EXPECT_FALSE(firstOperand > secondOperand)  << " 10 > 20 (short) (short)";
	EXPECT_TRUE (firstOperand < secondOperand)  << " 10 < 20 (short) (short)";


	prepareData(&firstOperand, &secondOperand, TYPE_LONG_INT, first, first);
	EXPECT_TRUE(firstOperand == secondOperand) << " 10 == 10 (long) (long)";
	EXPECT_FALSE(firstOperand > secondOperand) << " 10 > 10 (long) (long)";
	EXPECT_FALSE(firstOperand < secondOperand) << " 10 < 10 (long) (long)";


	prepareData(&firstOperand, &secondOperand, TYPE_LONG_INT, first, first + 10);
	EXPECT_FALSE(firstOperand == secondOperand) << " 10 == 20 (long) (long)";
	EXPECT_FALSE(firstOperand > secondOperand) << " 10 > 20 (long) (long)";
	EXPECT_TRUE(firstOperand < secondOperand) << " 10 < 20 (long) (long)";


	secondOperand.valueOfData.dataForLong = first;
	firstOperand.typeOfData = TYPE_SHORT_INT;
	firstOperand.valueOfData.dataForShort = first;

	EXPECT_TRUE(firstOperand == secondOperand) << " 10 == 10 (short) (long)";
	EXPECT_TRUE(secondOperand == firstOperand ) << " 10 == 10 (long) (short)";

	EXPECT_FALSE(firstOperand < secondOperand) << " 10 < 10 (short) (long)";
	EXPECT_FALSE(secondOperand > firstOperand) << " 10 > 10 (long) (short)";


	firstOperand.valueOfData.dataForShort = first + 5;

	EXPECT_FALSE(firstOperand == secondOperand) << " 15 == 10 (short) (long)";
	EXPECT_FALSE(secondOperand == firstOperand) << " 10 == 15 (long) (short)";

	EXPECT_TRUE(firstOperand > secondOperand) << " 15 > 10 (short) (long)";
	EXPECT_TRUE(secondOperand < firstOperand) << " 10 < 15 (long) (short)";
}

TEST(TestExpression, TestExpressionEvaluation)
{
	string fileName = "testExpression.txt";

	ofstream fout;
	fout.open(fileName);
	fout << "long variable = 1;" << endl << 
		R"( 
			-10 + 1 - 1  + (20 / 10) - variable + 
			(1 > 1) - (1 >= 1) + (1 <= 1) + (1 < 1) - 
			(5 ^ 3) + (1 || 0) * (0 && 0) + (0 != 1) 
		;)";
	fout.close();

	Scanner* scanner = new Scanner(fileName);
	Diagram* diagram = new Diagram(scanner);
	diagram->data();
	Data actual = diagram->expression();
	Data expected;
	expected.typeOfData = TYPE_SHORT_INT;
	expected.valueOfData.dataForShort = -8;

	remove(fileName.c_str());
	EXPECT_TRUE(actual.typeOfData == expected.typeOfData);
	EXPECT_TRUE(actual.valueOfData.dataForShort == expected.valueOfData.dataForShort);
}

TEST_F(TreeTests, TestCurrentNode)
{
	EXPECT_TRUE(strcmp(diagram->root->current->getNode()->ID, "main") == 0) << "Check current node";
}

TEST_F(TreeTests, TestsAssigmentControl)
{
	EXPECT_TRUE(diagram->root->assigmentControl(TYPE_LONG_INT, TYPE_SHORT_INT)) << "Check Assigment Control (L S)";
	EXPECT_EXIT(diagram->root->assigmentControl(TYPE_BOOLEAN, TYPE_SHORT_INT),
				::testing::ExitedWithCode(0), "") << "Check Assigment Control (B S)";
}

TEST_F(TreeTests, TestCastingTypes)
{
	Data data1, data2, data3;
	data1.typeOfData = TYPE_SHORT_INT;
	data2.typeOfData = TYPE_LONG_INT;
	data3.typeOfData = TYPE_BOOLEAN;

	EXPECT_TRUE(diagram->root->getTypeByCastTable(data1, data1).typeOfData ==
		TYPE_SHORT_INT) << "Check casting types (S S)";

	EXPECT_TRUE(diagram->root->getTypeByCastTable(data2, data2).typeOfData ==
		TYPE_LONG_INT) << "Check casting types (L L)";

	EXPECT_TRUE(diagram->root->getTypeByCastTable(data3, data3).typeOfData ==
		TYPE_BOOLEAN) << "Check casting types (B B)";

	EXPECT_EXIT(diagram->root->getTypeByCastTable(data3, data1), 
				::testing::ExitedWithCode(0), "") << "Check casting types (B S)";

}