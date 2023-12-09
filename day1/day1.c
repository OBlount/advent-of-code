#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define PUZZLE_PATH "puzzle.txt"
#define MAX_READ 64

void part1()
{
	FILE *f;
	char buffer[MAX_READ];
	size_t total = 0;

	f = fopen(PUZZLE_PATH, "r");
	if (f == NULL) {
		fprintf(stderr, "[ERROR] - Can't open file");
		exit(1);
	}

	while (fgets(buffer, sizeof(buffer), f)) {
		char first = '\0';
		char last  = '\0';
		char num[3];

		for (size_t i = 0; i < sizeof(buffer); ++i) {
			if (buffer[i] == '\0') break;
			if (isdigit(buffer[i])) {
				if (first == '\0') first = buffer[i];
				else 		   last  = buffer[i];
			}
		}

		if (last == '\0') last = first;
		sprintf(num, "%c%c", first, last);
		num[2] = '\0';
		total += atoi(num);
	}

	printf("[PART 1] Sum of calibration values: %ld\n", total);
	fclose(f);
}

// Reads from a point (index) in the line and returns a number if it encounters a valid number
int readAhead(char *line, int index)
{
	char *validNumbers[] = { "one", "two", "three", "four",
		    "five", "six", "seven", "eight", "nine" };
	char workspace[8] = { '\0' };
	size_t workspaceIndex = 0;
	char *cursor = line + index;

	while (*cursor != '\0' && workspaceIndex < 8) {
		strncat(workspace, cursor, 1);
		for (size_t i = 0; i < 9; ++i) {
			size_t n = sizeof(validNumbers[i]);
			if (strncmp(workspace, validNumbers[i], n) == 0) return i+1;
		}
		workspaceIndex++;
		cursor++;
	}

	return 0;
}

void part2()
{
	FILE *f;
	char buffer[MAX_READ];
	size_t total = 0;

	f = fopen(PUZZLE_PATH, "r");
	if (f == NULL) {
		fprintf(stderr, "[ERROR] - Can't open file");
		exit(1);
	}

	while (fgets(buffer, sizeof(buffer), f)) {
		char first = '\0';
		char last  = '\0';
		char num[3];

		for (size_t i = 0; i < sizeof(buffer); ++i) {
			if (buffer[i] == '\0') break;
			if (isdigit(buffer[i])) {
				if (first == '\0') first = buffer[i];
				else 		   last  = buffer[i];
			}
			else {
				int isNumberText = readAhead(buffer, i);
				if (isNumberText > 0) {
					if (first == '\0') first = isNumberText + '0';
					else 		   last  = isNumberText + '0';
				}
			}
		}

		if (last == '\0') last = first;
		sprintf(num, "%c%c", first, last);
		num[2] = '\0';
		total += atoi(num);
	}

	printf("[PART 2] Sum of calibration values: %ld\n", total);
	fclose(f);
}

int main(int argc, char **argv)
{
	part1();
	part2();
	return 0;
}
