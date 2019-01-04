
#include <iostream>
#include "prova.h"
#include <Windows.h>

int main(void)
{
	CodomainFinder CF;

	CF.sayHello();
	// y-x*y
	Equation equation1;
	{
		equation1.node.push_back({ NodeType::Subtraction,	Space(), 1,	 2,	Variable() });	// 0 -
		equation1.node.push_back({ NodeType::Variable,		Space(), -1, -1,Variable::y });	// 1 y
		equation1.node.push_back({ NodeType::Product,		Space(), 3,	 4,	Variable() });	// 2 *
		equation1.node.push_back({ NodeType::Variable,	Space(), -1, -1,Variable::x });	// 3 x
		equation1.node.push_back({ NodeType::Variable,	Space(), -1, -1,Variable::y });	// 4 y
	}
	// (x*x+y*y-1)*(y-x*x)
	Equation equation2;
	{
		equation2.node.push_back({ NodeType::Product, Space(),1, 2,Variable() });	// 0 *
		equation2.node.push_back({ NodeType::Sum, Space(),3, 4,Variable() });	// 1 +
		equation2.node.push_back({ NodeType::Subtraction, Space(),8,5,Variable() });	// 2 -
		equation2.node.push_back({ NodeType::Product, Space(),7, 7,Variable() });	// 3 *
		equation2.node.push_back({ NodeType::Subtraction, Space(),6, 9,Variable() });	// 4 -
		equation2.node.push_back({ NodeType::Product, Space(),7, 7,Variable() });	// 5 *
		equation2.node.push_back({ NodeType::Product, Space(),8,8,Variable() });	// 6 *
		equation2.node.push_back({ NodeType::Variable, Space(),-1,-1,Variable::x });	// 7 x
		equation2.node.push_back({ NodeType::Variable, Space(),-1,-1,Variable::y });	// 8 y
		equation2.node.push_back({ NodeType::Number, toSpace({ IntervalType::Value, 10, 10, true, true }),-1,-1,Variable() });	// 9 1
	}
	CF.setEquation(equation2);
	std::cout << "(x*x+y*y-1)*(y-x*x)" << std::endl;
	system("pause");
	int n = 10;
	double l = 5;
	Interval x_, y_, v_;
	v_ = x_ = y_ = { IntervalType::Interval, 0, 0, true, true };
	v_.lowerBound = v_.upperBound = 1.1; v_.type = IntervalType::Value;
	//Space x = toSpace(x_); Space y = toSpace(y_); Space v = toSpace(v_);
	Interval x = x_; Interval y = y_; Interval v = v_;


	Interval a = { IntervalType::Interval, 1.1, 1.1, true, true };
	Interval b = { IntervalType::Interval, 0, 5.0/10, true, true };
	/*std::cout << (toSpace(a) / toSpace(b)).interval[0].lowerBound << std::endl;
	std::cout << (toSpace(a) / toSpace(b)).interval[0].upperBound << std::endl;
	std::cout << CF.contained(10, a / b) << std::endl;*/
	for (int i = n; i > -n; i--) {
		for (int j = -n*2; j < n*2; j++) {
			/*y.interval[0].lowerBound = l / n * (i-1);
			y.interval[0].upperBound = l / n * i;
			x.interval[0].lowerBound = l / n * j / 2.25;
			x.interval[0].upperBound = l / n * (j + 1) / 2.25;*/
			y.lowerBound = l / n * (i - 1);// se si scambiano non funziona più niente!
			y.upperBound = l / n * i;
			x.lowerBound = l / n * j / 2.25;
			x.upperBound = l / n * (j + 1) / 2.25;
			//std::cout << CF.contained(0, x*x*x - y * y + v);
			std::cout << ( CF.evaluate({ {x.lowerBound, y.lowerBound, x.lowerBound}, { x.upperBound, y.upperBound, x.upperBound} })
				? ' '
				: (char)219);
		}
		std::cout << std::endl;
	}
	std::cout << "\nBye!" << std::endl;
	system("pause");
	return 0;
}