#include "prova.h"

Interval Interval::operator+(const Interval& other) const
{
	Interval out;
	if (this->type == IntervalType::Value && other.type == IntervalType::Value)
	{
		out.type = IntervalType::Value;
		out.upperBound = out.lowerBound = this->lowerBound + other.lowerBound;
		out.lowerIncluded = this->lowerIncluded && other.lowerIncluded;
		out.upperIncluded = this->upperIncluded && other.upperIncluded;
	}
	else
	{
		out.type = IntervalType::Interval;
		out.lowerBound = this->lowerBound + other.lowerBound;
		out.upperBound = this->upperBound + other.upperBound;
		out.lowerIncluded = this->lowerIncluded && other.lowerIncluded;
		out.upperIncluded = this->upperIncluded && other.upperIncluded;
	}
	return out;
}

Interval Interval::operator-(const Interval& other) const
{
	Interval tmp;
	tmp.type = other.type;
	tmp.lowerBound = -other.upperBound;
	tmp.upperBound = -other.lowerBound;
	tmp.lowerIncluded = other.upperIncluded;
	tmp.upperIncluded = other.lowerIncluded;
	return *this + tmp;
}
Interval Interval::operator*(const Interval & other) const
{
	Interval out;
	out.type = (this->type == IntervalType::Value && other.type == IntervalType::Value) ? IntervalType::Value : IntervalType::Interval;
	double p[4], min, max;
	p[0] = this->lowerBound * other.lowerBound;
	p[1] = this->lowerBound * other.upperBound;
	p[2] = this->upperBound * other.lowerBound;
	p[3] = this->upperBound * other.upperBound;
	min = max = p[0];
	bool b[4], low_included, upp_included;
	b[0] = this->lowerIncluded && other.lowerIncluded;
	b[0] = this->lowerIncluded && other.upperIncluded;
	b[0] = this->upperIncluded && other.lowerIncluded;
	b[0] = this->upperIncluded && other.upperIncluded;
	low_included = upp_included = b[0];
	for (int i = 1; i < 4; i++)
	{
		if (p[i] < min)
		{
			min = p[i];
			low_included = b[i];
		}
		if (p[i] > max)
		{
			max = p[i];
			upp_included = b[i];
		}
	}
	out.lowerBound = min;
	out.upperBound = max;
	out.lowerIncluded = low_included;
	out.upperIncluded = upp_included;
	return out;
}
/*
void Interval::changeSign(void)
{
	*this = changeSign(*this);
}
Interval Interval::changeSign(const Interval &interval)
{
	Interval out;
	out.type = interval.type;
	out.lowerBound = -interval.upperBound;
	out.upperBound = -interval.lowerBound;
	out.lowerIncluded = interval.upperIncluded;
	out.upperIncluded = interval.lowerIncluded;
	return out;
}*/

Space Space::operator+(const Space & other)
{
	Space out;
	for (int i = 0; i < this->interval.size(); i++)
	{
		for (int j = 0; j < other.interval.size(); j++)
		{
			out.interval.push_back(this->interval[i] + other.interval[j]);
		}
	}
	simplify(out);
	return out;
}

Space Space::operator-(const Space & other)
{
	Space out;
	for (int i = 0; i < this->interval.size(); i++)
	{
		for (int j = 0; j < other.interval.size(); j++)
		{
			out.interval.push_back(this->interval[i] - other.interval[j]);
		}
	}
	simplify(out);
	return out;
}

Space Space::operator*(const Space & other)
{
	Space out;
	for (int i = 0; i < this->interval.size(); i++)
	{
		for (int j = 0; j < other.interval.size(); j++)
		{
			out.interval.push_back(this->interval[i] * other.interval[j]);
		}
	}
	simplify(out);
	return out;
}

Space Space::operator/(const Space & other)
{
	Space out;
	Space result;
	for (int i = 0; i < this->interval.size(); i++)
	{
		for (int j = 0; j < other.interval.size(); j++)
		{
			result = this->interval[i] / other.interval[j];
			for (int k = 0; k < result.interval.size(); k++)
			out.interval.push_back(result.interval[k]);
		}
	}
	simplify(out);
	return out;
}

void Space::simplify(Space &space)// TODO
{
}

Space CodomainFinder::evaluateEquation(int topIndex, Box &box)// TODO
{
	switch (equation.node[topIndex].nodetype)
	{
	case NodeType::Constant:
		std::cout << "Constant TODO" << std::endl;
		exit(0);
	case NodeType::Division:
		return evaluateEquation(equation.node[topIndex].sub1, box) / evaluateEquation(equation.node[topIndex].sub2, box);
	case NodeType::Equals:
		std::cout << "Equals TODO" << std::endl;
		exit(0);
	case NodeType::Number:
		return equation.node[topIndex].value;
	case NodeType::Product:
		return evaluateEquation(equation.node[topIndex].sub1, box) * evaluateEquation(equation.node[topIndex].sub2, box);
	case NodeType::Subtraction:
		return evaluateEquation(equation.node[topIndex].sub1, box) - evaluateEquation(equation.node[topIndex].sub2, box);
	case NodeType::Sum:
		return evaluateEquation(equation.node[topIndex].sub1, box) + evaluateEquation(equation.node[topIndex].sub2, box);
	case NodeType::Variable:
		switch (equation.node[topIndex].variable)
		{
		case Variable::x:
			return toSpace({ IntervalType::Interval, box.min.X, box.max.X, true, true });
		case Variable::y:
			return toSpace({ IntervalType::Interval, box.min.Y, box.max.Y, true, true });
		case Variable::t:
			return toSpace({ IntervalType::Interval, box.min.Z, box.max.Z, true, true });
		default:
			break;
		}
	default:
		break;
	}
	return Space();
}

bool CodomainFinder::contained(number x, Space space)
{
	for (int i = 0; i < space.interval.size(); i++)
		if (contained(x,space.interval[i]))
			return true;
	return false;
}

bool CodomainFinder::contained(number x, Interval interval)
{
	return (x >= interval.lowerBound && x <= interval.upperBound);
}

bool CodomainFinder::evaluate(Box box, int iter)// TODO
{
	if (iter <= 0)
		return contained(0, evaluateEquation(0, box));
	if (!evaluate(box, 0))
		return false;
	iter = iter - 1;
	bool b1 = evaluate({ {box.min.X, box.min.Y, box.min.Z}, {(box.min.X + box.max.X) / 2, (box.min.Y + box.max.Y) / 2 , (box.min.Z + box.max.Z) / 2 } }, iter);
	bool b2 = evaluate({ {(box.min.X + box.max.X) / 2, box.min.Y, box.min.Z}, {box.max.X, (box.min.Y + box.max.Y) / 2 , (box.min.Z + box.max.Z) / 2 } }, iter);
	bool b3 = evaluate({ {(box.min.X + box.max.X) / 2, (box.min.Y + box.max.Y) / 2, box.min.Z}, {box.max.X, box.max.Y , (box.min.Z + box.max.Z) / 2 } }, iter);
	bool b4 = evaluate({ {box.min.X, (box.min.Y + box.max.Y) / 2, box.min.Z}, {(box.min.X + box.max.X) / 2, box.max.Y , (box.min.Z + box.max.Z) / 2 } }, iter);
	bool b5 = evaluate({ {box.min.X, box.min.Y, (box.min.Z + box.max.Z) / 2}, {(box.min.X + box.max.X) / 2, (box.min.Y + box.max.Y) / 2 , box.max.Z } }, iter);
	bool b6 = evaluate({ {(box.min.X + box.max.X) / 2, box.min.Y, (box.min.Z + box.max.Z) / 2}, {box.max.X, (box.min.Y + box.max.Y) / 2 , box.max.Z } }, iter);
	bool b7 = evaluate({ {(box.min.X + box.max.X) / 2, (box.min.Y + box.max.Y) / 2, (box.min.Z + box.max.Z) / 2}, {box.max.X, box.max.Y , box.max.Z } }, iter);
	bool b8 = evaluate({ {box.min.X, (box.min.Y + box.max.Y) / 2, (box.min.Z + box.max.Z) / 2}, {(box.min.X + box.max.X) / 2, box.max.Y , box.max.Z } }, iter);
	
	return (b1 || b2 || b3 || b4 || b5 || b6 || b7 || b8);
}

bool CodomainFinder::evaluate(Box box)
{
	return evaluate(box, maxIter);
}

void CodomainFinder::setMaxIter(int newMaxIter)
{
	maxIter = newMaxIter;
}

bool CodomainFinder::setEquation(Equation newEquation)// TODO add a control
{
	equation = newEquation;
	std::cout << "dimensione equazione" << equation.node.size() << std::endl;
	return true;
}

Space operator/(const Interval& left, const Interval& right)
{
	Space out;
	Interval tmp, right_;
	if (right.lowerBound <= 0 && right.upperBound >= 0)// TODO tratta il caso in cui uno o più estremi sono zero
	{
		right_ = right;
		right_.lowerBound = 0;
		right_.lowerIncluded = false;
		tmp.type = (left.type == IntervalType::Value && right_.type == IntervalType::Value) ? IntervalType::Value : IntervalType::Interval;
		double p[4], min, max;
		p[0] = left.lowerBound * INFINITY;
		p[1] = left.lowerBound / right_.upperBound;
		p[2] = left.upperBound * INFINITY;
		p[3] = left.upperBound / right_.upperBound;
		min = max = p[0];
		bool b[4], low_included, upp_included;
		b[0] = false;
		b[0] = left.lowerIncluded && right_.upperIncluded;
		b[0] = false;
		b[0] = left.upperIncluded && right_.upperIncluded;
		low_included = upp_included = b[0];
		for (int i = 1; i < 4; i++)
		{
			if (p[i] < min)
			{
				min = p[i];
				low_included = b[i];
			}
			if (p[i] > max)
			{
				max = p[i];
				upp_included = b[i];
			}
		}
		tmp.lowerBound = min;
		tmp.upperBound = max;
		tmp.lowerIncluded = low_included;
		tmp.upperIncluded = upp_included;

		out.interval.push_back(tmp);

		
		right_ = right;
		right_.upperBound = 0;
		right_.upperIncluded = false;
		tmp.type = (left.type == IntervalType::Value && right.type == IntervalType::Value) ? IntervalType::Value : IntervalType::Interval;
		p[0] = left.lowerBound / right_.lowerBound;
		p[1] = left.lowerBound *(-INFINITY);
		p[2] = left.upperBound / right_.lowerBound;
		p[3] = left.upperBound *(-INFINITY);
		min = max = p[0];
		b[0] = left.lowerIncluded && right_.lowerIncluded;
		b[0] = false;
		b[0] = left.upperIncluded && right_.lowerIncluded;
		b[0] = false;
		low_included = upp_included = b[0];
		for (int i = 1; i < 4; i++)
		{
			if (p[i] < min)
			{
				min = p[i];
				low_included = b[i];
			}
			if (p[i] > max)
			{
				max = p[i];
				upp_included = b[i];
			}
		}
		tmp.lowerBound = min;
		tmp.upperBound = max;
		tmp.lowerIncluded = low_included;
		tmp.upperIncluded = upp_included;

		out.interval.push_back(tmp);
	}
	else
	{
		tmp.type = (left.type == IntervalType::Value && right.type == IntervalType::Value) ? IntervalType::Value : IntervalType::Interval;
		double p[4], min, max;
		p[0] = left.lowerBound / right.lowerBound;
		p[1] = left.lowerBound / right.upperBound;
		p[2] = left.upperBound / right.lowerBound;
		p[3] = left.upperBound / right.upperBound;
		min = max = p[0];
		bool b[4], low_included, upp_included;
		b[0] = left.lowerIncluded && right.lowerIncluded;
		b[0] = left.lowerIncluded && right.upperIncluded;
		b[0] = left.upperIncluded && right.lowerIncluded;
		b[0] = left.upperIncluded && right.upperIncluded;
		low_included = upp_included = b[0];
		for (int i = 1; i < 4; i++)
		{
			if (p[i] < min)
			{
				min = p[i];
				low_included = b[i];
			}
			if (p[i] > max)
			{
				max = p[i];
				upp_included = b[i];
			}
		}
		tmp.lowerBound = min;
		tmp.upperBound = max;
		tmp.lowerIncluded = low_included;
		tmp.upperIncluded = upp_included;

		out.interval.push_back(tmp);
	}
	return out;
}

Space toSpace(Interval interval)
{
	Space out;
	out.interval.push_back(interval);
	return out;
}
