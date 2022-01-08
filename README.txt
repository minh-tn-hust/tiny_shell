# tiny_shell
A small project to study how a shell works with Visual C++
+-------------------------------------------------------------------+
|     _____  .__       .__      _________.__           .__  .__     |
|    /     \ |__| ____ |  |__  /   _____/|  |__   ____ |  | |  |    |
|   /  \ /  \|  |/    \|  |  \ \_____  \ |  |  \_/ __ \|  | |  |    |
|  /    Y    \  |   |  \   Y  \/        \|   Y  \  ___/|  |_|  |__  |
|  \____|__  /__|___|  /___|  /_______  /|___|  /\___  >____/____/  |
|          \/        \/     \/        \/      \/     \/             |
+-------------------------------------------------------------------+


This is the most common command need to know with MinhShell
If you want to do some thing with Process, please use:
   launch filename -f     launch a process foreground with filename
   launch filename -b     launch a process background with filename
   launch filepath -f     launch a process foreground with filepath
   launch filepath -b     launch a process background with filepath

   process       main command to work with process
       kill             PID    use to kill a process with PID
       resume   PID    use to resume a process with PID
       suspend  PID    use to suspend a process with PID
       list            use to list all running processes

   cd            main command to move around in system
       ..              move to parents directory
       file_name       move to child with filename
       file_path       move to file with filepath
   ls            list all child file and folder

   exit          exit shell

   path          list all environment variables
   addpath variable_name variable_value add a variable with name and value
