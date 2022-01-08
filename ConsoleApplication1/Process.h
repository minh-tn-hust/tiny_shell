#pragma once
#include <Windows.h>
#include <stdio.h>
#include <psapi.h>
#include <tchar.h>
#include <TlHelp32.h>
#define PROCESS_BUFSIZE 4096


static DWORD inUseProcessId  = 0;
HANDLE terminateThreadHandle;

void printProcessNameAndID( DWORD processID )
{
    // khởi tạo biến nhận tên của process, có một vài process không thể lấy được tên, vì vậy cần thiết phải khởi tạo
    TCHAR ProcessName[MAX_PATH] = TEXT("<unknown>");

    // thực hiện mở process sử dụng processID
    HANDLE hProcess = OpenProcess( 
        PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, // mở để đọc thống tin + yêu cầu đọc bộ nhớ trong quá trìn sử dụng process
        FALSE, // không kế thừa những cái tài nguyên của process cha
        processID // PID
    );
    // thực hiện truy xuất tên của process
    if (NULL != hProcess )
    {
        HMODULE hMod;
        DWORD cbNeeded;

        if ( EnumProcessModules( 
            hProcess, // Process Handle
            &hMod,  // lấy ra module handle
            sizeof(hMod), // kích thức
            &cbNeeded)  // trả về số lượng byte để có thể lấy được hết thông tin của module hanlde
        ){
            GetModuleBaseName( 
                hProcess, // để có thể sử dụng yêu cầu phải kích hoạt 2 trường PROCESS_QUERY_INFORMATION và PROCESS_VM_READ
                hMod, // module handle đã được lấy ra từ câu lệnh if
                ProcessName, // biến nhận tên của Process
                sizeof(ProcessName)/sizeof(TCHAR)  // kích thước của biến nhận tên
            );
        }
    }
    DWORD getStatus;
    GetExitCodeProcess(hProcess, &getStatus);
    _tprintf( TEXT("%-30s  (PID: %-5u)   "), ProcessName, processID );
    if (getStatus == STILL_ACTIVE) {
        printf("STILL_ACTIVE\n");
    }
    else {
        printf("UNKNOW\n");
    }
    CloseHandle(hProcess);
}

void listProcess() {
	// thực hiện liệt kê các process đang được chạy
    printf("%-30s  (Process ID)   Status\n", "Module Name");
    DWORD processes[PROCESS_BUFSIZE];
    DWORD size = sizeof(processes);
    DWORD rSize;
    if (EnumProcesses(
        processes, // mảng để nhậ PID của các process hiện tại đang chạy
        size, // kích thước của mảng được sử dụng để nhận
        &rSize  // số lượng byte đã sử dụng trong mảng được truyền vào
    )){
        for (int i = 0; i < rSize / sizeof(DWORD); i++) {
            printProcessNameAndID(processes[i]);
        }
    }
    else {
        printf("Can't access system, please try a gain");
    }

}

void killProcess(DWORD PID) {
	HANDLE pHandle = OpenProcess(THREAD_TERMINATE, FALSE, PID);
	if (pHandle) {
		TerminateProcess(pHandle, 0);
	    CloseHandle(pHandle);
	}
}

void killProcess(char **arg) {
    if (strcmp(*arg, "") == 0) {
        printf("Unknow PID of process\n");
        return;
    }
    DWORD PID = atoi(arg[0]);
	HANDLE pHandle = OpenProcess(THREAD_TERMINATE, FALSE, PID);
	if (pHandle) {
		TerminateProcess(pHandle, 0);
		CloseHandle(pHandle);
	}
    printf("Process with PID = %d has been killed\n", PID);
}

void resumeProcess(char **arg) {
    if (strcmp(*arg, "") == 0) {
        printf("Unknow PID of process\n");
        return;
    }
    DWORD PID = atoi(*arg);
    // chụp lại hiện tại đang có những luồng nào được sử dụng
    HANDLE getThreadSnapshots = CreateToolhelp32Snapshot(
        TH32CS_SNAPTHREAD,
        PID
    );
    THREADENTRY32 threadEntry;
    threadEntry.dwSize = sizeof(THREADENTRY32);
    Thread32First(getThreadSnapshots, &threadEntry);

    // enumerate qua danh sách luồng được chụp lại, cái nào có cùng PID thì resume cái đó
    do {
        if (threadEntry.th32OwnerProcessID == PID) {
            HANDLE getThreadHandle = OpenThread(THREAD_ALL_ACCESS, FALSE, threadEntry.th32ThreadID);
            ResumeThread(getThreadHandle);
            CloseHandle(getThreadHandle);
        }
    } while (Thread32Next(getThreadSnapshots, &threadEntry));
    printf("Process with PID %d has been resume\n", PID);
}


//thực hiện suspend những luồng mà process hiện tại đang sử dụng
void suspendProcess(char **arg) {
    if (strcmp(*arg, "") == 0) {
        printf("Unknow PID of process\n");
        return;
    }
    DWORD PID = atoi(*arg);
    // chụp lại hiện tại đang có những luồng nào được sử dụng
    HANDLE getThreadSnapshots = CreateToolhelp32Snapshot(
        TH32CS_SNAPTHREAD,
        PID
    );
    THREADENTRY32 threadEntry;
    threadEntry.dwSize = sizeof(THREADENTRY32);
    Thread32First(getThreadSnapshots, &threadEntry);

    // enumerate qua danh sách luồng được chụp lại, cái nào có cùng PID thì suspend cái đó
    do {
        if (threadEntry.th32OwnerProcessID == PID) {
            HANDLE getThreadHandle = OpenThread(THREAD_ALL_ACCESS, FALSE, threadEntry.th32ThreadID);
            SuspendThread(getThreadHandle);
            CloseHandle(getThreadHandle);
        }
    } while (Thread32Next(getThreadSnapshots, &threadEntry));
    printf("Process with PID %d has been suspend\n", PID);
}

BOOL WINAPI ctrlHandle(DWORD ctrlHandleId) {
    if (inUseProcessId != 0) {
        HANDLE inUse = OpenProcess(THREAD_TERMINATE, FALSE, inUseProcessId);
        if (inUse) {
            TerminateProcess(inUse, 0);
            CloseHandle(inUse);
            SetEvent(terminateThreadHandle);
            printf("\nKill process with Ctrl + C\n");
            ResetEvent(terminateThreadHandle);
            inUseProcessId = 0;
        }
    }
    return TRUE;
}

void createForegroundProcess(LPWSTR path) {
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    SetConsoleCtrlHandler(ctrlHandle, true);
    terminateThreadHandle = CreateEvent(
        NULL,
        TRUE,
        FALSE,
        TEXT("Terminte Thread")
    );
    BOOL check = CreateProcess(
        path, // đường dẫn tới file exe
        NULL, //
        NULL, //
        NULL, //
        FALSE, // nó không kế thừa tài nguyên từ process cha
        CREATE_NEW_PROCESS_GROUP,
        NULL,
        NULL,
        &si,
        &pi
    );
    if (check == 1) {
		HANDLE waitHandle[2] = { pi.hProcess, terminateThreadHandle };
		inUseProcessId = pi.dwProcessId;
		WaitForMultipleObjects(2, waitHandle, FALSE, INFINITE);
    }
    else {
        printf("Invalid file path: %ls\n", path);
    }
	CloseHandle(pi.hProcess);
}

void createBackgroundProcess(LPWSTR path) {
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    CreateProcess(
        path,
        NULL,
        NULL,
        NULL,
        FALSE,
        CREATE_NO_WINDOW,
        NULL,
        NULL,
        &si,
        &pi
    );
    printf("This program run with PID: %d\n", pi.dwProcessId);
}

// khởi tạo một tiến trình mà tiến trình đó chạy chương trình ở đường dẫn path
// args sẽ nhận biến số để biết được là chương trình cần chạy background hay là foreground
void launchProcess(char **args) { 
    if (strcmp(args[0], "") == 0) {
        printf("Invalid file path\n");
        return;
    }
    char *path = args[0];
	wchar_t *filePath = new wchar_t[strlen(path)];
	size_t rReturn;
	mbstowcs_s(&rReturn, filePath, strlen(path) + 1, path, strlen(path) + 1);
	LPWSTR ptr = filePath;

    if (path[0] == 'C' && path[1] == ':') {
		if (strcmp(args[1], "") == 0 || strcmp(args[1], "-f") == 0) {
			createForegroundProcess(ptr);
		}
		else {
			createBackgroundProcess(ptr);
		}
    }
    else {
		TCHAR BUFFER[1000];
		GetCurrentDirectory(1000, BUFFER);
        wcscat_s(BUFFER, TEXT("\\"));
		wcscat_s(BUFFER, ptr);
		if (strcmp(args[1], "") == 0 || strcmp(args[1], "-f") == 0) {
			createForegroundProcess(BUFFER);
		}
		else {
			createBackgroundProcess(BUFFER);
		}

    }
}


