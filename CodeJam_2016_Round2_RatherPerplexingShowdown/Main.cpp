#include <fstream>
#include <string>
#include <cinttypes>
#include <vector>
#include <algorithm>

using std::ifstream;
using std::ofstream; 
using std::string;
using std::vector;
using std::endl; 

enum class Options : uint8_t
{
	PAPER,
	ROCK,
	SCISSORS
};

char options2Char(Options option)
{
	switch (option)
	{
	case Options::PAPER:
		return 'P';
	case Options::ROCK:
		return 'R';
	case Options::SCISSORS:
		return 'S'; 
	default :
		return 'N';
	}
}

Options getPartner(Options winner)
{
	Options partner; 

	switch (winner)
	{
	case Options::PAPER:
		partner = Options::ROCK;
		break;
	case Options::SCISSORS:
		partner = Options::PAPER; 
		break;
	case Options::ROCK:
		partner = Options::SCISSORS;
		break;
	default:
		break;
	}

	return partner;
}

void solveKnowingWinner(vector<vector<Options> >& resultVector, const Options winner, const int MAX)
{
	resultVector.resize(MAX + 1);
	resultVector[0].reserve(1);
	resultVector[0].emplace_back(winner);

	for (int i = 1; i <= MAX; ++i)
	{
		int size = 1 << i;
		resultVector[i].reserve(size);

		for (int j = 0; j < resultVector[i - 1].size(); ++j)
		{
			Options winner = resultVector[i - 1][j];
			Options partner = getPartner(winner);

			resultVector[i].emplace_back(winner);
			resultVector[i].emplace_back(partner);
		}
	}
}

void solveCases(vector<vector<Options> >& resultRock, vector<vector<Options> >& resultScissors, vector<vector<Options> >& resultPaper)
{
	const int MAX = 12;

	solveKnowingWinner(resultRock, Options::ROCK, MAX);
	solveKnowingWinner(resultScissors, Options::SCISSORS, MAX);
	solveKnowingWinner(resultPaper, Options::PAPER, MAX);	
}

bool checkTopRound(const vector<Options>& topRound, int R, int P, int S)
{
	int nbR = 0;
	int nbP = 0;
	int nbS = 0;
	
	for (int i = 0; i < topRound.size(); ++i)
	{
		switch (topRound[i])
		{
		case Options::PAPER:
			nbP++;
			break;
		case Options::ROCK:
			nbR++;
			break;
		case Options::SCISSORS:
			nbS++;
			break;
		default:
			break;
		}
	}

	if (R == nbR && P == nbP && S == nbS)
	{
		return true;
	}

	return false; 
}

string composeSolution(std::vector<Options>& workingRound)
{
	// find right order
	unsigned int blockSize = 1; 

	// for all blockSize
	while (blockSize <= workingRound.size() / 2)
	{
		// check all blocks 2 by 2
		for (auto it = workingRound.begin(); it != workingRound.end(); it += blockSize * 2)
		{
			auto first = it; 
			auto second = first + blockSize;

			auto compareFirst = first;
			auto compareSecond = second; 

			int count = 0;
			while (*compareFirst == *compareSecond && count < blockSize)
			{
				++compareFirst;
				++compareSecond; 
				++count; 
			}

			Options firstBlock = *compareFirst;
			Options secondBlock = *compareSecond;

			if (firstBlock > secondBlock)
			{
				std::swap_ranges(first, second, second); 
			}
		}

		blockSize *= 2; 
	}

	string solution = "";
	solution.resize(workingRound.size());
	for (int i = 0; i < workingRound.size(); i+=2)
	{
		char o1 = options2Char(workingRound[i]);
		char o2 = options2Char(workingRound[i + 1]);

		solution[i] = o1;
		solution[i + 1] = o2;
	}

	return solution; 
}

string calculate(int N, int R, int P, int S)
{
	vector<vector<Options> > resultRock;
	vector<vector<Options> > resultScissors;
	vector<vector<Options> > resultPaper;
	solveCases(resultRock, resultScissors, resultPaper);

	string result = ""; 
	if (checkTopRound(resultRock[N], R, P, S))
		result = composeSolution(resultRock[N]);
	else if (checkTopRound(resultScissors[N], R, P, S))
		result = composeSolution(resultScissors[N]);
	else if (checkTopRound(resultPaper[N], R, P, S))
		result = composeSolution(resultPaper[N]); 

	return result; 
}

int main(int argc, char* argv[])
{
	ifstream file(argv[1]); 

	if (!file.is_open())
	{
		return 1; 
	}

	int N, R, S, P;
	N = R = S = P = 0; 
	
	string result;

	int T = 0;
	file >> T; 

	ofstream out("Output.txt");
	for (int i = 1; i <= T; ++i)
	{
		file >> N;
		file >> R;
		file >> P; 
		file >> S; 

		result = calculate(N, R, P, S); 

		if (result == "")
			result = "IMPOSSIBLE"; 

		out << "Case #" << i << ": " << result << endl;
	}

	return 0;
}