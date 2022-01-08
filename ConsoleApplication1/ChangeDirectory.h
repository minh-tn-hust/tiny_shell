#pragma once
#include <Windows.h>
#include <stdio.h>
#define BUFFER_SIZE 300 
#include <tchar.h>
#include <atlstr.h>
#include "Converter.h"


void cd(char** args) {
	TCHAR buffer[BUFFER_SIZE];
	
	if (strcmp(args[0], "") == 0) {
		GetCurrentDirectory(BUFFER_SIZE, buffer);
		printf("%ls\n", buffer);
	}
	else if (strcmp(args[0], "..") == 0) {
		// Thực hiện di chuyển về thư mục cha
		int i = SetCurrentDirectory(TEXT(".."));
		GetCurrentDirectory(BUFFER_SIZE, buffer);
		printf("%ls\n", buffer);
	}
	else {
		// Thực hiện conver tử char* sang wchar_t * rồi bỏ vào LPWSTR để có thể sử dụng được với các hàm
		// của Win32 (các hàm yêu cầu các xâu kí tự đầu vào phải là kiểu wide character, char nó không
		// có nhận
		char *arg = args[0];
		wchar_t *wtext = new wchar_t[strlen(arg)];
		size_t rReturn;
		mbstowcs_s(&rReturn, wtext, strlen(arg) + 1, arg, strlen(arg) + 1);
		LPWSTR ptr = wtext;


		// Thực hiện di chuyển thư mục tới đường dẫn được cung cấp
		int i = SetCurrentDirectory(ptr);
		if (i == 0) {
			printf("Invalid file path\n");
		}
		GetCurrentDirectory(BUFFER_SIZE, buffer);
		printf("%ls\n", buffer);
	}
}


