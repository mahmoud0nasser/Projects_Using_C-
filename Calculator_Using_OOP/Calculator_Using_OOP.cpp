#include <iostream>

using namespace std;

class clsCalculator
{
private:

	int _Result = 0;
	string _LastOperation = "Clear";
	int _LastNumber = 0;
	int _PreviousResult = 0;

	bool _isZero(int Number)
	{
		return (Number == 0);
	}

public:

	void Add(int Number)
	{
		_LastNumber = Number;
		_PreviousResult = _Result;
		_LastOperation = "Adding";
		_Result += Number;
	}

	void Subtract(int Number)
	{
		_LastNumber = Number;
		_PreviousResult = _Result;
		_LastOperation = "Subtracting";
		_Result -= Number;
	}

	void Multiply(int Number)
	{
		_LastNumber = Number;
		_PreviousResult = _Result;
		_LastOperation = "Multipling";
		_Result *= Number;
	}

	void Divide(int Number)
	{
		_LastNumber = Number;

		if (_isZero(Number))
		{
			Number = 1;
		}
		_PreviousResult = _Result;
		_LastOperation = "Dividing";
		_Result /= Number;
	}

	int GetFinalResult()
	{
		return _Result;
	}

	void CancelLastOperation()
	{
		_LastNumber = 0;
		_LastOperation = "Canceling Last Operation";
		_Result = _PreviousResult;
	}

	void Clear()
	{
		_LastNumber = 0;
		_PreviousResult = 0;
		_LastOperation = "Clear";
		_Result = 0;
	}

	void PrintResult()
	{
		cout << "Result ";
		cout << "after " << _LastOperation << " " << _LastNumber << " is: " << _Result << endl;
	}
};

int main()
{
	clsCalculator Calculator;

	Calculator.Clear();

	Calculator.Add(10);
	Calculator.PrintResult();

	Calculator.Add(100);
	Calculator.PrintResult();

	Calculator.Subtract(20);
	Calculator.PrintResult();

	Calculator.Multiply(10);
	Calculator.PrintResult();

	Calculator.Divide(0);
	Calculator.PrintResult();

	Calculator.Divide(10);
	Calculator.PrintResult();

	Calculator.CancelLastOperation();
	Calculator.PrintResult();

	Calculator.Clear();
	Calculator.PrintResult();

	return 0;
}