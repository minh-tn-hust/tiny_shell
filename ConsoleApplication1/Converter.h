#pragma once
#include <tchar.h>
#include <stdlib.h>
#include <atlstr.h>
#include <string.h>


char* go_back(char *source) {
	int length = strlen(source);
	if (source[3] == '\0')
		return source;
	for (int i = length - 1; i > 0; i--) {
		if (source[i] == '\\') {
			source[i] = '\0';
			break;
		}
	}
	return source;
}

