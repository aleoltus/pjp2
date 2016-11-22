#include <algorithm>
#include "Scores.h"
using namespace std;
bool operator< (const Score& a, const Score& b)
{
	return a.score > b.score;
}

const char FILE_NAME[] = "wyniki.csv";

wyniki ReadScores()
{
	wyniki w;
	Score s;
	char name[25];
	FILE * file;
	if (fopen_s(&file, FILE_NAME, "r") != 0)
		return w;

	while (fscanf(file, "%s %d\n", name, &s.score) != EOF)
	{
		s.name = name;
		w.push_back(s);
	}

	fclose(file);
	return w;
}

static void WriteScores(wyniki& w)
{
	FILE * file;
	if (fopen_s(&file, FILE_NAME, "w") != 0)
		return;

	for (auto it = w.begin(); it != w.end(); ++it)
	{
		if (it->name == "")
			it->name="puste";
		fprintf(file, "%s %d\n", it->name.c_str(), it->score);
	}

	fclose(file);
}

static void AddScore(wyniki& w, Score s)
{
	w.push_back(s);
	sort(w.begin(), w.end());

	if (w.size() > 10)
		w.pop_back();
}

void AddScore(Score s)
{
	wyniki w = ReadScores();

	AddScore(w, s);

	WriteScores(w);
}