// penny.cpp

#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <algorithm>
#include <chrono>
#include <stack>

std::map<int, char> alpha_map
	{
		{0, 'A'},
		{1, 'B'},
		{2, 'C'},
		{3, 'D'},
		{4, 'E'},
		{5, 'F'},
		{6, 'G'},
		{7, 'H'},
		{8, 'I'},
		{9, 'K'},
		{10, 'L'},
		{11, 'M'},
		{12, 'N'},
		{13, 'O'},
		{14, 'P'},
		{15, 'Q'},
		{16, 'R'},
		{17, 'S'},
		{18, 'T'},
		{19, 'U'},
		{20, 'V'},
		{21, 'W'},
		{22, 'X'},
		{23, 'Y'},
		{24, 'Z'}
	};

std::string beta_map("ABCDEFGHIKLMNOPQRSTUVWXYZ");

struct SSTK
{
	int state;
	int tx;
	double cost;
	SSTK(int s, int t, double c) : state(s), tx(t), cost(c) {}
};

std::stack<SSTK> sstk;
std::stack<SSTK> rev_sstk;

class Segments
{
	std::set<std::pair<int, int>> segments;
public:
	Segments() : segments({std::make_pair(0,1)}) {}
	void Store(int x, int y)
	{
		if (x < y)
			segments.insert(std::make_pair(x,y));
		else
			segments.insert(std::make_pair(y,x));
	}
	bool Find(int x, int y)
	{
		std::pair<int, int> segment;
		if (x < y)
			segment = std::make_pair(x,y);
		else
			segment = std::make_pair(y,x);
			
		return
			find(segments.begin(), segments.end(),
				segment) != segments.end();
	}
	void Delete(int x, int y)
	{
		std::pair<int, int> segment;
		if (x < y)
			segment = std::make_pair(x,y);
		else
			segment = std::make_pair(y,x);
		
		segments.erase(segment);
	}
};

double calc_cost(int state, int tx, double i_cost)
{
	double cost = i_cost;
	if (state - 5 == tx)
		cost /= 2;
	else if (state - 1 == tx)
		cost -= 2;
	else if (state + 1 == tx)
		cost += 2;
	else if (state + 5 == tx)
		cost *= 2;
	return cost;
}

class StateMachine
{
	std::vector<int> states{};
	int state;
	double cost;
	int count;
	Segments duplicate_edge;
public:
	std::vector<std::vector<int>> transitions;
	
	StateMachine() : transitions(25, states), state(1), cost(2.0), count(0)
	{
		transitions[0] = {};
		transitions[1] = {2};
		transitions[2] = {3, 7};
		transitions[3] = {4, 8}; // 2 could not arrive at end state 24
		transitions[4] = {3, 9};

		transitions[5] = {6, 10};
		transitions[6] = {5, 7, 11};
		transitions[7] = {6, 8, 12}; // 2 could not arrive at end state 24
		transitions[8] = {3, 7, 9, 13};
		transitions[9] = {4, 8, 14};

		transitions[10]= {5, 11, 15};
		transitions[11]= {6, 10, 12, 16};
		transitions[12]= {7, 11, 13, 17};
		transitions[13]= {8, 12, 14, 18};
		transitions[14]= {9, 13, 19};

		transitions[15]= {10, 16, 20};
		transitions[16]= {11, 15, 17, 21};
		transitions[17]= {12, 16, 18, 22};
		transitions[18]= {13, 17, 19, 23};
		transitions[19]= {14, 18, 24};

		transitions[20]= {15, 21};
		transitions[21]= {16, 20, 22};
		transitions[22]= {17, 21, 23};
		transitions[23]= {18, 22, 24};
		transitions[24]= {};
	}
	
	void print_stack()
	{
		while (!sstk.empty())
		{
			SSTK s = sstk.top();
			rev_sstk.push(s);
			sstk.pop();
		}
		
		while (!rev_sstk.empty())
		{
			SSTK s = rev_sstk.top();
			sstk.push(s);
			rev_sstk.pop();
			int diff = s.tx - s.state;
			switch (diff)
			{
				case -5:
					std::cout << 'n';
					break;
				case -1:
					std::cout << 'w';
					break;
				case 1:
					std::cout << 'e';
					break;
				case 5:
					std::cout << 's';
					break;
				default:
					std::cout << 'X';
					break;
			}
		}
	}
	
	void print_stack_nodes()
	{
		while (!sstk.empty())
		{
			SSTK s = sstk.top();
			rev_sstk.push(s);
			sstk.pop();
		}
		
		while (!rev_sstk.empty())
		{
			SSTK s = rev_sstk.top();
			sstk.push(s);
			rev_sstk.pop();
			std::cout << beta_map[s.tx] << ' ';
		}
		std::cout << '\n';
	}
	
	void transit_states()
	{		
		
		if(state == 24)
		{
			//++count;
			if (cost == 0.0)
			{
				++count;
				std::cout << count << ". reached 24 at cost of " << cost << '\n';
				print_stack();  // or print_stack_nodes() for alternate representation;
				std::cout << '\n';
			}
			return;
		}
		for (auto tx : transitions[state])
		{
			if (duplicate_edge.Find(state, tx)) continue;

			Push(state, tx, cost); // before tx cost

			cost = calc_cost(state, tx, cost); // after tx cost

			state = tx;

			transit_states();

			SSTK s = Pop();
			state = s.state;
			cost = s.cost;
		}
	}
	
	void Push(int s, int t, double c)
	{
		SSTK z(s, t, c);
		sstk.push(z);
		duplicate_edge.Store(z.state, z.tx);
	}
	
	SSTK Pop()
	{
		SSTK s = sstk.top();
		sstk.pop();
		duplicate_edge.Delete(s.state, s.tx);
		return s;
	}
	
	friend std::ostream& operator << (std::ostream& o, const StateMachine& t);
};

std::ostream& operator << (std::ostream& o, const StateMachine& t)
{
	o << t.transitions.size() << '\n';
	for (auto ix : t.transitions)
		o << ix.size() << " ";
	return o;
}

int main(int argc, char** argv)
{
	using namespace std;
	
	StateMachine sm;
	
	sm.transit_states();
}
