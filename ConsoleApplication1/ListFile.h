#pragma once
#include <Windows.h>
#include <stdio.h>
#define BUFFER_SIZE 300 
#include <tchar.h>
#include <atlstr.h>
#include "Converter.h"

void listFile(LPCWSTR filename) {
	// biến chứa thông tin của file được tìm thấy
	WIN32_FIND_DATA ffd;
	HANDLE hfind;
	hfind = FindFirstFile(filename, &ffd);

	// nếu như không tìm thấy file
	if (hfind == INVALID_HANDLE_VALUE) {
		_tprintf(TEXT("Can't find this file\n"));
	}
	else {
		int count = 0;
		do {
			_tprintf(TEXT("%-50s "), ffd.cFileName);
			count++;
			if (count != 0 && count % 3 == 0) {
				_tprintf(TEXT("\n"));
			}
		} while (FindNextFile(hfind, &ffd) != 0);
		_tprintf(TEXT("\n"));
	}
}


void ls(char** args) {
	TCHAR buffer[BUFFER_SIZE];
	
	// Thực hiện liệt kê các file trong thư mục hiện tại
	if (strcmp(args[0], "") == 0) {
		GetCurrentDirectory(BUFFER_SIZE, buffer);
		wcscat_s(buffer, BUFFER_SIZE, TEXT("\\*"));
		listFile(buffer);
	}
	// Thực hiện hiển thị các tham số mà ls hỗ trợ
	else if (strcmp(args[0], "-help") == 0) {
		printf("cd directory_name : if the current directory \n\t\t    exist directory_name file, \n\t\t    you  can move to this file\n");
		printf("cd ..             : move to parent directory\n");
		printf("cd                : go to current file\n");
		printf("cd file_path      : move to file_path\n");
	} else {
		// Thực hiện hiển thị các file chứa trong đường dẫn
		char *arg = args[0];
		strcat_s(arg,strlen(arg) + 3, "\\*");
		wchar_t *wtext = new wchar_t[strlen(arg)];
		size_t rReturn;
		mbstowcs_s(&rReturn, wtext, strlen(arg) + 1, arg, strlen(arg) + 1);
		LPWSTR ptr = wtext;
		listFile(ptr);

	}
}

