# Watch-Dog-Service Project
Watch Dog service ensures the continuous operation of the user's program.

This Project is keeping the user's program running by providing API with 2 functions: 
MakeMeImmortal(MMI) and DNR( do not resuscitate).
The Service is keeping the user's program alive by:
1 Creating a mutual thread for the watch-dog process and the user's process.
2 Recongnizing the process ID to watch by retrieving the environment variables.
3 Setting signal handlers and masks using SIGUSR1(for MMI function) and SIGUSR2 (for DNR function)
4 Implementing syncronization using semaphore.

# How to install this project:
1 Clone this project. 
2 Use the 'RUN_WD.text' file to execute it in  your terminal (or use the makefile).
3 In 'watchdog_text.c' you can add you code under line 22 ("code of user to keep alive"). You can also change the n/interval as per your 
requiremenrs.
4 Run the program and observe the terminal output after executing the main.out (observe the two process IDs runnig).
