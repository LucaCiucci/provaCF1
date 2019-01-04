#pragma once

#include <vector>
#include <iostream>
#include <math.h>

typedef double number;

enum class Variable {
	x,
	y,
	t
};

struct Vector3
{
	// Default Constructor
	Vector3()
	{
		X = 0.0f;
		Y = 0.0f;
		Z = 0.0f;
	}
	// Variable Set Constructor
	Vector3(double X_, double Y_, double Z_)
	{
		X = X_;
		Y = Y_;
		Z = Z_;
	}
	// Bool Equals Operator Overload
	bool operator==(const Vector3& other) const
	{
		return (this->X == other.X && this->Y == other.Y && this->Z == other.Z);
	}
	// Bool Not Equals Operator Overload
	bool operator!=(const Vector3& other) const
	{
		return !(this->X == other.X && this->Y == other.Y && this->Z == other.Z);
	}
	// Addition Operator Overload
	Vector3 operator+(const Vector3& right) const
	{
		return Vector3(this->X + right.X, this->Y + right.Y, this->Z + right.Z);
	}
	// Subtraction Operator Overload
	Vector3 operator-(const Vector3& right) const
	{
		return Vector3(this->X - right.X, this->Y - right.Y, this->Z - right.Z);
	}
	// double Multiplication Operator Overload
	Vector3 operator*(const double& other) const
	{
		return Vector3(this->X * other, this->Y * other, this->Z * other);
	}
	// double Division Operator Overload
	Vector3 operator/(const double& other) const
	{
		return Vector3(this->X / other, this->Y / other, this->Z / other);
	}

	// Scalar Product Operator Overload
	double operator*(const Vector3& right) const
	{
		return double(this->X * right.X + this->Y * right.Y + this->Z * right.Z);
	}
	// Vectorial Product Operator Overload
	Vector3 operator%(const Vector3& right) const
	{
		return Vector3(this->Y*right.Z - this->Z*right.Y, this->Z*right.X - this->X*right.Z, this->X*right.Y - this->Y*right.X);
	}
	// Addition Operator Overload
	void operator+=(const Vector3& right)
	{
		this->X += right.X;
		this->Y += right.Y;
		this->Z += right.Z;
	}
	// Oposite
	Vector3 operator-(void) const
	{
		return Vector3(-this->X, -this->Y, -this->Z);
	}

	// Positional Variables
	double X;
	double Y;
	double Z;
};

struct Box {
	Vector3 min, max;
};

enum class NodeType {// TODO
	Sum,
	Product,
	Subtraction,
	Division,
	Equals,
	Constant,
	Number,
	Variable
};

enum class IntervalType {// TODO
	Value,
	Interval
};

class Interval {// TODO
public:
	IntervalType type;
	number lowerBound, upperBound;
	bool lowerIncluded, upperIncluded;
	Interval operator+(const Interval& other) const;
	Interval operator-(const Interval& other) const;
	Interval operator*(const Interval& other) const;
	//void changeSign(void);
	//Interval changeSign(const Interval&);
};

class Space {// TODO
private:
	void simplify(Space &);
public:
	std::vector<Interval> interval;
	Space operator+(const Space& other);
	Space operator-(const Space& other);
	Space operator*(const Space& other);
	Space operator/(const Space& other);
};
Space operator/(const Interval& left, const Interval& right);
Space toSpace(Interval);

struct Node {// TODO
	NodeType nodetype;
	Space value;// will be calculated by the algorithm during calculus, after the definition of the equation
	int sub1, sub2;// index
	Variable variable;
};

struct Equation {// TODO
	//int topIndex = 0;
	std::vector<Node> node;
};

class CodomainFinder {// TODO
private:
	Equation equation;
	int maxIter = 6;
	Space evaluateEquation(int, Box &);
public:
	void sayHello(void) {
		std::cout << "Hello There!" << std::endl;
	};
	bool contained(number, Space);
	bool contained(number, Interval);
	bool evaluate(Box, int);
	bool evaluate(Box);
	void setMaxIter(int);
	bool setEquation(Equation);
};

//Space sum(Space, Space);




