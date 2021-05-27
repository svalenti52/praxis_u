// rle.cpp
// run length encoding
// 9-Feb-2021
// Created by svalenti on 5/26/2021.
// outputs a run length encoding format given an input string
// second, more consolidated attempt

#include <iostream>
#include <string>

int main()
{
	using namespace std;

	string s = {"aaaabbbccadddeeeeeeeeee"};
	cout << "[";
	
	int ix = 0;
	int rle = 1;

	while (ix < s.size())
	{
		if (rle == 1)
			cout << "(\"" << s[ix] << "\", ";

		if (ix+1 < s.size())
		{
			if (s[ix] == s[ix+1])
				++rle;
			else 
			{
				cout << rle << "), ";
				rle = 1;
			}
		}
		else 
			cout << rle << ")";
		
		++ix;
	}
	
	cout << "]";
}
