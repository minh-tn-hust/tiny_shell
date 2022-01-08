#pragma once
#include <processenv.h>
#include <WinBase.h>
#include <stdio.h>
#include <tchar.h>
#include <stdlib.h>

using namespace std;


void listVariable() {
    LPTSTR lpszVariable; 
    LPTCH lpvEnv; 
    lpvEnv = GetEnvironmentStrings();
    if (lpvEnv == NULL)
    {
        printf("GetEnvironmentStrings failed (%d)\n", GetLastError()); 
        return ;
    }
    lpszVariable = (LPTSTR) lpvEnv;
    while (*lpszVariable)
    {
        _tprintf(TEXT("%s\n"), lpszVariable);
        lpszVariable += lstrlen(lpszVariable) + 1;
    }
    FreeEnvironmentStrings(lpvEnv);
}


void setEnvironmentVariable(char* name, char* value) {
    if (strcmp(name, "") == 0) {
        printf("You need Variable name and value\n");
        return;
    }
	// conveert tên từ char -> wchar
	wchar_t *wname = new wchar_t[strlen(name)];
	size_t rReturn;
	mbstowcs_s(&rReturn, wname, strlen(name) + 1, name, strlen(name) + 1);
	LPWSTR Wname = wname;
	// convert giá trị từ chả -> wchar
	wchar_t *wvalue = new wchar_t[strlen(value)];
	mbstowcs_s(&rReturn, wvalue, strlen(value) + 1, value, strlen(value) + 1);
	LPWSTR Wvalue = wvalue;
	// thực hiện thêm biến môi trường
	SetEnvironmentVariable(Wname, Wvalue);
}

