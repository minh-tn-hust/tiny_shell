#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define BUFFER_SIZE 1024
#define SPLIT_CHAR " \t\n\r\a"
#define SPLIT_BUFFER_SIZE 64

char* shellReadline() {
	int bufsize = BUFFER_SIZE;
	int cPosition = 0;
	char* buffer = (char*) malloc(bufsize * sizeof(char));
	if (!buffer) {
		printf("Minh-Shell: Allocation Error");
		exit(EXIT_FAILURE);
	}

	int c;

	while (1) {
		c = getchar();
		if (c == EOF || c == '\n') {
			buffer[cPosition] = '\0';
			return buffer;
		}
		if (cPosition >= BUFFER_SIZE) {
			bufsize += BUFFER_SIZE;
			buffer = (char*) realloc(buffer, bufsize * sizeof(char));
			if (!buffer) {
				printf("Minh-Shell: Allocation Error");
				exit(EXIT_FAILURE);
			}
		}

		buffer[cPosition] = c;
		++cPosition;
	}
	
}

char** shellSplitLine(char *line) {
	int bufsize = SPLIT_BUFFER_SIZE;
	char** tokens = (char**)malloc(bufsize * sizeof(char*));
	int position = 0;
	if (!tokens) {
		printf("Minh-Shell: Allocation Error");
		exit(EXIT_FAILURE);
	}

	char* temp = strtok_s(line, SPLIT_CHAR,&line);
	while (temp != NULL) {
		tokens[position] = temp;
		++position;
		temp = strtok_s(line, SPLIT_CHAR,&line);
	}
	tokens[position] = (char*)"";
	return tokens;
}

