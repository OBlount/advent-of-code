#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define PUZZLE_PATH "puzzle.txt"
#define BUFFER_SIZE 255
#define MAX_SYMBOLS 10
#define MAX_GEAR_SIZE 10
#define GEAR_PART '*'

typedef char* Engine;
typedef char  EnginePart;

void printEngine(Engine engine, size_t engineLen, size_t engineWidth)
{
	size_t engineIndex = 0;
	for (size_t i = 0; i < engineLen; ++i) {
		for (size_t j = 0; j < engineWidth; ++j) printf("%c", engine[engineIndex++]);
		printf("\n");
	}

}

void makeEngine(FILE *f, char *buffer, Engine engine, size_t lineCount, size_t lineLen)
{
	for (size_t i = 0; i < lineCount; ++i) {
		if (i == 0 || i == lineCount-1) {
			for (size_t j = 0; j < lineLen; ++j) {
				if (j == lineLen-1) engine[(i*lineLen)+j] = '\0';
				else 		    engine[(i*lineLen)+j] = '.';
			}
		}
		else {
			for (size_t j = 0; j < lineLen; ++j) {
				if 	(j == lineLen-1) engine[(i*lineLen)+j] = '\0';
				else if (j == lineLen-2) engine[(i*lineLen)+j] = '.';
				else if (j == 0)    	 engine[(i*lineLen)+j] = '.';
				else {
					char *part = fgets(buffer, 2, f);
					if (part[0] == '\n') part = fgets(buffer, 2, f);
					engine[(i*lineLen)+j] = part[0];
				}
			}
		}
	}
	fseek(f, 0, SEEK_SET);
}

size_t getLengthOfNumber(Engine engine, size_t index)
{
	if (!isdigit(engine[index+1])) return 1;
	else 			       return 1 + getLengthOfNumber(engine, index+1);
}

int isPartSymbol(EnginePart part)
{
	const char SYMS[MAX_SYMBOLS] = { '#', '$', '%', '&', '*', '+', '-', '/', '=', '@' };
	for (size_t j = 0; j < MAX_SYMBOLS; ++j) {
		if (part == SYMS[j]) return 1;
	}
	return 0;
}

int part1(Engine engine, size_t engineSize, size_t lineLen)
{
	int partSum = 0;
	for (size_t i = 0; i < engineSize; ++i) {
		if (isdigit(engine[i])) {
			size_t numberLen = getLengthOfNumber(engine, i);
			char *numberSpan = (char *) calloc(numberLen+1, sizeof(char));
			size_t numberSpanIndex = 0;
			for (size_t j = 0; j < numberLen; ++j)
				numberSpan[numberSpanIndex++] = engine[i+j];

			// Check if the number is ajacent to any symbols:
			// Check left and right:
			EnginePart leftPart  = engine[i-1];
			EnginePart rightPart = engine[i+numberLen];
			if (isPartSymbol(leftPart) || isPartSymbol(rightPart)) {
				partSum += atoi(numberSpan);
				free(numberSpan);
				i += numberLen;
				continue;
			}
			
			// Check above and below:
			size_t aboveStartIndex = (i-lineLen)-1;
			size_t belowStartIndex = (i+lineLen)-1;
			short detectedSymbol   = 0;
			for (size_t j = 0; j < numberLen+2; ++j) {
				EnginePart selectedPart = engine[aboveStartIndex+j];
				if (isPartSymbol(selectedPart)) {
					partSum += atoi(numberSpan);
					detectedSymbol = 1;
					break;
				}
			}
			if (detectedSymbol) {
				free(numberSpan);
				i += numberLen;
				continue;
			}
			for (size_t j = 0; j < numberLen+2; ++j) {
				EnginePart selectedPart = engine[belowStartIndex+j];
				if (isPartSymbol(selectedPart)) {
					partSum += atoi(numberSpan);
					break;
				}
			}

			free(numberSpan);
			i += numberLen;
		}
	}
	return partSum;
}

int part2(Engine engine, size_t engineSize, size_t lineLen, int **gears)
{
	int gearRatioSum = 0;
	for (size_t i = 0; i < engineSize; ++i) {
		if (isdigit(engine[i])) {
			size_t numberLen = getLengthOfNumber(engine, i);
			char *numberSpan = (char *) calloc(numberLen+1, sizeof(char));
			size_t numberSpanIndex = 0;
			for (size_t j = 0; j < numberLen; ++j)
				numberSpan[numberSpanIndex++] = engine[i+j];

			// Check if number is adjacent to a gear part. Append to gears if so:
			// Check left and right:
			EnginePart leftPart  = engine[i-1];
			EnginePart rightPart = engine[i+numberLen];
			if (leftPart == GEAR_PART)  {
				gears[i-1][++(gears[i-1][0])] = atoi(numberSpan);
				free(numberSpan);
				i += numberLen;
				continue;
			}
			if (rightPart == GEAR_PART)  {
				gears[i+numberLen][++(gears[i+numberLen][0])] = atoi(numberSpan);
				free(numberSpan);
				i += numberLen;
				continue;
			}

			// Check above and below:
			size_t aboveStartIndex = (i-lineLen)-1;
			size_t belowStartIndex = (i+lineLen)-1;
			for (size_t j = 0; j < numberLen+2; ++j) {
				size_t offset = aboveStartIndex+j;
				EnginePart selectedPart = engine[offset];
				if (selectedPart == GEAR_PART)
					gears[offset][++gears[offset][0]] = atoi(numberSpan);
			}
			for (size_t j = 0; j < numberLen+2; ++j) {
				size_t offset = belowStartIndex+j;
				EnginePart selectedPart = engine[offset];
				if (selectedPart == GEAR_PART)
					gears[offset][++gears[offset][0]] = atoi(numberSpan);
			}

			free(numberSpan);
			i += numberLen;
		}
	}

	// Tot up the gear ratios with all the gears with exactly two numbers attached:
	for (size_t i = 0; i < engineSize; ++i)
		if (gears[i][0] == 2) gearRatioSum += gears[i][1] * gears[i][2];
	return gearRatioSum;
}

int main()
{
	FILE *f = NULL;
	char buffer[BUFFER_SIZE] = { '\0' };

	f = fopen(PUZZLE_PATH, "r");
	if (f == NULL) exit(1);

	// Get engine length and width:
	fgets(buffer, sizeof(buffer), f);
	size_t lineLen = strlen(buffer) + 2;
	fseek(f, 0, SEEK_SET);
	size_t lineCount = 2;
	while (fgets(buffer, sizeof(buffer), f)) lineCount++;
	fseek(f, 0, SEEK_SET);

	// Create, make and pad engine:
	size_t engineSize = (lineLen * lineCount);
	Engine engine     = (Engine) calloc(engineSize, sizeof(EnginePart));
	makeEngine(f, buffer, engine, lineCount, lineLen);

	// Gears is a matrix. The first number in a row represents the length of array:
	int **gears      = (int **) calloc(engineSize, sizeof(int *));
	for (size_t i = 0; i < engineSize; ++i) gears[i] = (int *) calloc(MAX_GEAR_SIZE, sizeof(int));

	int partSum      = part1(engine, engineSize, lineLen);
	int gearRatioSum = part2(engine, engineSize, lineLen, gears);
	printf("Part 1 sum: %d\n", partSum);
	printf("Part 2 sum: %d\n", gearRatioSum);

	// Clean Up:
	for (size_t i = 0; i < engineSize; ++i) free(gears[i]);
	free(gears);
	free(engine);
	fclose(f);
	return 0;
}
