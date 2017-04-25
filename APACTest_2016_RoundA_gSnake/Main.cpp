#include <fstream>
#include <vector>
#include <set>
#include <deque>
#include <algorithm>

using std::ifstream; 
using std::ofstream; 
using std::ios_base; 
using std::vector; 
using std::pair; 
using std::set; 
using std::deque; 
using std::make_pair;

int evaluateTest(const vector<pair<int, char> >& test, int rows, int cols)
{
	set<pair<int,int> > usedCandies; 
	deque<pair<int, int> > snake; 
	set<pair<int, int> > snakeFind; 

	int currentCmd = 0;
	int len = 1;
	int timeMax = 1000000 + std::max(rows, cols); 
	
	pair<int, int> headPos = { 1, 1 };
	snake.push_front(headPos); 
	snakeFind.insert(headPos);

	unsigned int orient = 0; // Right : 0, Down : 1, Left : 2, Up : 3

	// for all operations op in test
	for (int time = 1; time < timeMax; ++time)
	{
		// Move snake
		switch (orient)
		{
		case 0 : // right
			++headPos.first;
			break;
		case 1: // Down
			++headPos.second;
			break;
		case 2: // Left
			--headPos.first; 
			break;
		case 3: // Up
			--headPos.second;
			break;
		default:
			break;
		}

		// Compute cycle position
		headPos.first = (((headPos.first-1) + cols) % cols) +1;
		headPos.second = (((headPos.second-1) + rows) % rows) +1;
		
		int sum = headPos.first + headPos.second;

		if (((sum) % 2 == 1) && (usedCandies.find(make_pair(headPos.first, headPos.second)) == usedCandies.end()))
		{
			usedCandies.emplace(headPos.first, headPos.second);
			++len;
		}

		// If length is bigger than the queue, remove last element
		if (snake.size() == len)
		{
			pair<int, int> snakeLast = snake.back(); 
			snake.pop_back(); 
			snakeFind.erase(snakeLast); 
		}

		// Check if snake's position is valid
		if (snakeFind.find(headPos) != snakeFind.end())
		{
			return len;
		}

		// Add new head to snake. 
		snake.push_front(headPos);
		snakeFind.insert(headPos); 

		// if op's time equals current time, change orientation accordingly
		if (currentCmd < test.size() && test[currentCmd].first == time)
		{
			if (test[currentCmd].second == 'R')
			{
				++orient;
			}
			else if (test[currentCmd].second == 'L')
			{
				--orient;
			}

			orient = (orient + 4) % 4;

			++currentCmd; 
		}
	}

	return len;
}


int main(int argc, char** argv)
{
	ifstream file(argv[1]); 
	ofstream oFile("Output.txt", ios_base::trunc);
	
	if (!file.is_open())
		return 1; 

	if (!oFile.is_open())
		return 2;

	
	int nbTestCase = 0;
	file >> nbTestCase; 

	for (int i = 0; i < nbTestCase; ++i)
	{
		int actions = 0;
		int rows = 0;
		int columns = 0;

		file >> actions;
		file >> rows;
		file >> columns;

		vector<pair<int, char> > commands;

		int time = 0;
		char command = 0;

		for (int i = 0; i < actions; ++i)
		{
			file >> time;
			file >> command;

			commands.emplace_back(time, command);
		}

		int res = evaluateTest(commands, rows, columns); 

		oFile << "Case #" << i + 1 << ": " << res << "\n"; 
	}

	return 0; 
}