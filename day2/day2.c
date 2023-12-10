#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define PUZZLE_PATH "puzzle.txt"
#define MAX_GAMES 100
#define MAX_READ 256
#define MAX_SETS 6

typedef struct Set {
	size_t r;
	size_t g;
	size_t b;
} Set;

typedef struct Game {
	int gID;
	Set sets[MAX_SETS];
} Game;

void printGames(Game *games)
{
	for (size_t i = 0; i < MAX_GAMES; ++i) {
		printf("Game ID: %d\n", games[i].gID);
		for (size_t j = 0; j < MAX_SETS; ++j) {
			printf("Sets:\n\tr: %ld\n\tg: %ld\n\tb: %ld\n",
					games[i].sets[j].r,
					games[i].sets[j].g,
					games[i].sets[j].b);
		}
	}
}

void initGames(Game *arr)
{
	for (size_t i = 0; i < MAX_GAMES; ++i) {
		arr[i].gID = 0;
		for (size_t j = 0; j < MAX_SETS; ++j) {
			arr[i].sets[j].r = 0;
			arr[i].sets[j].g = 0;
			arr[i].sets[j].b = 0;
		}
	}
}

void parseGames(Game *games)
{
	FILE *f = NULL;
	char buffer[MAX_READ] = { '\0' };

	f = fopen(PUZZLE_PATH, "r");
	if (f == NULL) exit(1);

	size_t gamesIndex = 0;
	while (fgets(buffer, sizeof(buffer), f)) {
		char *gameIDLine = strtok(buffer, ":");
		char *setsLine   = strtok(NULL, ":");

		// Get game ID:
		games[gamesIndex].gID = atoi(&gameIDLine[5]);

		// Parse the sets:
		char *set = strtok(setsLine, ";");
		size_t setsIndex = 0;
		while (set != NULL) {
			char tempSet[MAX_READ] = { '\0' };
			strcpy(tempSet, set);
			char *cursor = tempSet;
			while (*cursor != '\0') {
				char currentDigit[3] = { '\0', '\0', '\0' };
				if (isdigit(*cursor)) {
					currentDigit[0] = *cursor++;
					if (isdigit(*cursor)) currentDigit[1] = *cursor++;
					switch (*++cursor) {
						case 'r':
							games[gamesIndex].sets[setsIndex].r
								= atoi(currentDigit);
							break;
						case 'g':
							games[gamesIndex].sets[setsIndex].g
								= atoi(currentDigit);
							break;
						case 'b':
							games[gamesIndex].sets[setsIndex].b
								= atoi(currentDigit);
							break;
					}
				}
				cursor++;
			}
			set = strtok(NULL, ";");
			setsIndex++;
		}
		gamesIndex++;
	}
	fclose(f);
}

#define MAX_RED 12
#define MAX_GRN 13
#define MAX_BLU 14
void part1(Game *games)
{
	int gIDSum = 0;
	for (size_t i = 0; i < MAX_GAMES ; ++i) {
		int toAdd = games[i].gID;
		for (size_t j = 0; j < MAX_SETS; ++j) {
			size_t reds   = games[i].sets[j].r;
			size_t greens = games[i].sets[j].g;
			size_t blues  = games[i].sets[j].b;
			if (reds > MAX_RED || greens > MAX_GRN || blues > MAX_BLU) {
				toAdd = 0;
				break;
			}
		}
		gIDSum += toAdd;
	}
	printf("Game ID sum: %d\n", gIDSum);
}

void part2(Game *games)
{
	long powers = 0;
	for (size_t i = 0; i < MAX_GAMES ; ++i) {
		size_t minReds   = 1;
		size_t minGreens = 1;
		size_t minBlues  = 1;
		for (size_t j = 0; j < MAX_SETS; ++j) {
			minReds   = minReds   < games[i].sets[j].r ? games[i].sets[j].r : minReds;
			minGreens = minGreens < games[i].sets[j].g ? games[i].sets[j].g : minGreens;
			minBlues  = minBlues  < games[i].sets[j].b ? games[i].sets[j].b : minBlues;
		}
		powers += (minReds*minGreens*minBlues);
	}
	printf("Game ID powers: %ld\n", powers);
}

int main(int argc, char **argv)
{
	Game games[MAX_GAMES];
	initGames(games);
	parseGames(games);
	part1(games);
	part2(games);
	return 0;
}
