#pragma once

#include <string>
#include <vector>
using namespace std;

struct Score
{
	int score;
	string name;
};

typedef vector<Score> wyniki;

wyniki ReadScores();

void AddScore(Score);