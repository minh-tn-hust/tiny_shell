#pragma once
#include <stdio.h>
#include "CommandAnalysis.h"
#include "ChangeDirectory.h"
#include "ListFile.h"
#include "Process.h"
#include "EnvirontmentVariable.h"



int shellExcute(char** args) {
	if (args == NULL || strcmp(args[0], "cd") == 0) {
		cd(args + 1);
	 }
	else if (strcmp(args[0], "cls") == 0) {
		system("cls");
	}
	else if (strcmp(args[0], "ls") == 0) {
		ls(args + 1);
	}
	else if (strcmp(args[0], "process") == 0 && strcmp(args[1], "kill") == 0) {
		killProcess(args + 2);
	}
	else if (strcmp(args[0], "process") == 0 && strcmp(args[1], "list") == 0) {
		listProcess();
	}
	else if (strcmp(args[0], "process") == 0 && strcmp(args[1], "resume") == 0) {
		resumeProcess(args + 2);
	}
	else if (strcmp(args[0], "process") == 0 && strcmp(args[1], "suspend") == 0) {
		suspendProcess(args+2);
	}  
	else if (strcmp(args[0], "path") == 0) {
		listVariable();
	} 
	else if (strcmp(args[0], "addpath") == 0) {
		setEnvironmentVariable(args[1], args[2]);
	}
	else if (strcmp(args[0], "exit") == 0) {
		return 0;
	}
	else if (strcmp(args[0], "launch") == 0) {
		launchProcess(args + 1);
	} 
	else if (strcmp(args[0] + strlen(args[0]) -4, ".bat") == 0) {
		system(args[0]);
	} 
	else if (strcmp(args[0], "help") == 0) {
		printf("+-------------------------------------------------------------------+\n");
		printf("|     _____  .__       .__      _________.__           .__  .__     |\n");
		printf("|    /     \\ |__| ____ |  |__  /   _____/|  |__   ____ |  | |  |    |\n");
		printf("|   /  \\ /  \\|  |/    \\|  |  \\ \\_____  \\ |  |  \\_/ __ \\|  | |  |    |\n");
		printf("|  /    Y    \\  |   |  \\   Y  \\/        \\|   Y  \\  ___/|  |_|  |__  |\n");
		printf("|  \\____|__  /__|___|  /___|  /_______  /|___|  /\\___  >____/____/  |\n");
		printf("|          \\/        \\/     \\/        \\/      \\/     \\/             |\n");
		printf("+-------------------------------------------------------------------+\n");
		printf("\n\n");
		printf("This is the most common command need to know with MinhShell\n");
		printf("If you want to do some thing with Process, please use:\n");
		printf("   launch filename -f     launch a process foreground with filename\n");
		printf("   launch filename -b     launch a process background with filename\n");
		printf("   launch filepath -f     launch a process foreground with filepath\n");
		printf("   launch filepath -b     launch a process background with filepath\n\n");
		printf("   process       main command to work with process\n");
		printf("       kill		PID    use to kill a process with PID\n");
		printf("       resume	PID    use to resume a process with PID\n");
		printf("       suspend  PID    use to suspend a process with PID\n");
		printf("       list            use to list all running processes\n\n");
		printf("   cd            main command to move around in system\n");
		printf("       ..              move to parents directory\n");
		printf("       file_name       move to child with filename\n");
		printf("       file_path       move to file with filepath\n");
		printf("   ls            list all child file and folder\n\n");
		printf("   exit          exit shell\n\n");
		printf("   path          list all environment variables\n");
		printf("   addpath variable_name variable_value add a variable with name and value\n");
	}
	else if (strcmp(args[0], "") == 0) {
		return 1;
	} else {
		printf("Unknow command, use help for more info\n");
	}
	return 1;
}

void shellLoop(void)
{
	char *line;
	char** args;
	int status;
	do {
		printf(">> ");
		line = shellReadline();
		args = shellSplitLine(line);
		status = shellExcute(args);
	} while (status);
}
