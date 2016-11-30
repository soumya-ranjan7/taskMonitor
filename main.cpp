#include<iostream>
#include<fstream>
#include<dirent.h>
#include<ctype.h>
#include<cstdlib>
#include<stdio.h>
#include<string.h>
#include <unistd.h> 
#include<pthread.h>
#include<stdlib.h>
#include<signal.h>
#include<cstdlib>
#include<termios.h>

using namespace std;


typedef struct statstruct_proc {
  int           pid;                      // The process id. 

  char          exName [1000];            // Process Name
  char          state; // 1               // R is running, S is sleeping, D is sleeping in an uninterruptible wait, Z is zombie, T is traced or stopped 
  unsigned      euid,                      // effective user id 
                egid;                      // effective group id 
  int           ppid;                     // The pid of the parent. 
  int           pgrp;                     // The pgrp of the process. 
  int           session;                  // The session id of the process.
  int           tty;                      // The tty the process uses 
  int           tpgid;                    // (too long) 
  unsigned int	flags;                    // The flags of the process. 
  unsigned int	minflt;                   // The number of minor faults 
  unsigned int	cminflt;                  // The number of minor faults with childs 
  unsigned int	majflt;                   // The number of major faults 
  unsigned int  cmajflt;                  // The number of major faults with childs 
  int           utime;                    // user mode jiffies 
  int           stime;                    // kernel mode jiffies 
  int		cutime;                   // user mode jiffies with childs 
  int           cstime;                   // kernel mode jiffies with childs 
  int           counter;                  // process's next timeslice 
  int           priority;                 // the standard nice value, plus fifteen 
  unsigned int  timeout;                  // The time in jiffies of the next timeout 
  unsigned int  itrealvalue;              // The time before the next SIGALRM is sent to the process 
  int           starttime; // 20          // Time the process started after system boot 
  unsigned int  vsize;                    // Virtual memory size 
  unsigned int  rss;                      // Resident Set Size 
  unsigned int  rlim;                     // Current limit in bytes on the rss 
  unsigned int  startcode;                // The address above which program text can run 
  unsigned int	endcode;                  // The address below which program text can run 
  unsigned int  startstack;               // The address of the start of the stack 
  unsigned int  kstkesp;                  // The current value of ESP
  unsigned int  kstkeip;                 // The current value of EIP 
  int		signal;                   // The bitmap of pending signals 
  int           blocked; // 30            // The bitmap of blocked signals 
  int           sigignore;                // The bitmap of ignored signals 
  int           sigcatch;                 // The bitmap of catched signals 
  unsigned int  wchan;  // 33             // (too long) 
  int		sched, 		  // scheduler 
                sched_priority;		  // scheduler priority 
		
}procinfo;

bool term = false;

void procDetails();
void *thread2(void *);
void *thread1(void *);
void signal_kill();
char getch();

main()
{	
	pthread_t pthread1;
	int t1;
	t1 = pthread_create(&pthread1,NULL,thread1,NULL);
	if(t1)
	{
		perror("THREAD1");
		exit(0);
	}
	pthread_exit(NULL);
}

void *thread1(void *z)
{
	pthread_t pthread2;
	int t2;
	int j;
	t2 = pthread_create(&pthread2,NULL,thread2,NULL);
	if(t2)
	{
		perror("THREAD2");
		exit(0);
	}
	while(true)
	{
		char key;
		key = getch();

		if(key == 'k')
		{
			term = true;
			pthread_join(pthread2,NULL);
			signal_kill();
			term=false;	
			sleep(5);		
			t2 = pthread_create(&pthread2,NULL,thread2,NULL);
		}
		else if(key == 'q')
			exit(0);
	}
}

void *thread2(void *z)
{

	while (true) 
	{
		system ("clear");
		procDetails();
		sleep(10);
	}
}

void procDetails()
{
	if(term)
		pthread_exit(NULL);
	DIR *proc;
	unsigned int clock_tick ;
	struct dirent *folder;

	clock_tick = sysconf (_SC_CLK_TCK);
	proc = opendir ("/proc");
	if (opendir == 0) {
		printf ("can't read /proc \n");
		return;
		}
	
	printf ("%-8.8s  %-25.25s  %-8.8s  %-6.6s  %-7.7s  %-15.15s\n", "pid", "name", "ppid", 
"state", "utime", "vir Mem in KB");
	printf ("%-8.8s  %-25.25s  %-8.8s  %-6.6s  %-7.7s  %-15.15s\n", "--------", "------", 
"------", "------", "------", "--------");

	while(folder=readdir(proc)) {
		if(term)
			pthread_exit(NULL);
		long pid;
		FILE *fp;
		procinfo pinfo;
		char szStatStr[2048]={'\0'};
		char *t,*s;
		int len=0;

		if(! isdigit(folder->d_name[0])) {
			continue ;  // if directory is not a pid then skip
			}
		pid =strtol(folder->d_name,NULL,10);
		string addrString = "/proc/";
		addrString.append (folder->d_name);
		 //opendir accepts const char * only
		if(pid == 0)
			continue;
		string fileAddr = addrString + "/stat";
		const char *file = fileAddr.c_str();
		fp = fopen(file,"r");
		if (fp == 0) {
			cout << " process " << pid << "doesn't exist \n"; 
			continue;
			}
		fgets (szStatStr, sizeof(szStatStr), fp);
		sscanf (szStatStr, "%u", &(pinfo.pid));
		s = strchr (szStatStr, '('); //strchr searcheds for a given character in an array
		for (t=s,len=0; *t != ')'; t++,len++) 
		     pinfo.exName[len] = *t;
		pinfo.exName[len++] = *t++;
		pinfo.exName[len] = '\0';
		++t;
		pinfo.exName[0]=' ';
		pinfo.exName[len-1]=' ';
		sscanf (t, "%c %d %d %d %d %d %u %u %u %u %u %d %d %d %d %d %d %u %u %d %u %u %u %u %u %u %u %u %d %d %d %d %u %d %d",
 /*          1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33*/
		  &(pinfo.state),
		  &(pinfo.ppid),
		  &(pinfo.pgrp),
		  &(pinfo.session),
		  &(pinfo.tty),
		  &(pinfo.tpgid),
		  &(pinfo.flags),
		  &(pinfo.minflt),
		  &(pinfo.cminflt),
		  &(pinfo.majflt),
		  &(pinfo.cmajflt),
		  &(pinfo.utime),
		  &(pinfo.stime),
		  &(pinfo.cutime),
		  &(pinfo.cstime),
		  &(pinfo.counter),
		  &(pinfo.priority),
		  &(pinfo.timeout),
		  &(pinfo.itrealvalue),
		  &(pinfo.starttime),
		  &(pinfo.vsize),
		  &(pinfo.rss),
		  &(pinfo.rlim),
		  &(pinfo.startcode),
		  &(pinfo.endcode),
		  &(pinfo.startstack),
		  &(pinfo.kstkesp),
		  &(pinfo.kstkeip),
		  &(pinfo.signal),
		  &(pinfo.blocked),
		  &(pinfo.sigignore),
		  &(pinfo.sigcatch),
		  &(pinfo.wchan),
	          &(pinfo.sched),
	          &(pinfo.sched_priority));
		printf ("%-8d  %-25.25s  %-8d  %-6c  %-7.3f  %-15d\n", pinfo.pid, 
pinfo.exName, pinfo.ppid, pinfo.state,1.0*(pinfo.cutime+pinfo.cstime)/clock_tick, pinfo.vsize/1024);
		fclose(fp);
		// to give a scroll feeling sleep for 0.01 seconds
		struct timespec t1,t2;
		t1.tv_nsec = 10*1000* 1000; 
		t1.tv_sec = 0;
		nanosleep (&t1, &t2); //sleep doesn't take fractions
		if(term)
			pthread_exit(NULL);
	}
	closedir(proc);

}

void signal_kill()
{
	unsigned int pid,i;
	char spid[10], buf[1024], buf1[1024];

	printf ("process id to kill  :  ");
	scanf ("%d", &pid);
	string proc_dir, proc_cmdline;
	proc_dir.assign("/proc/");
	sprintf (spid,"%d",pid);
	proc_dir.append(spid);
	proc_dir.append("/");
	proc_cmdline = proc_dir + "cmdline";
	
	FILE *fp = fopen (proc_cmdline.c_str(), "r");

	for (i=0; i<1024;i++) 
		buf[i] = '1';

	if ( fgets (buf, sizeof(buf), fp) == NULL)
	{
		cout << "Reading error of " << proc_cmdline << "\n";
		exit(0);
	}
	fclose (fp);
	for (i=0; i<1000;i++) 
	{
		if (buf[i] == '\0' && buf[i+1] == '\0')  break;
		if (buf[i] == '\0')  buf[i] = ' ';
	}
	char choice;
	cout << "Should following process be killed? y/Y for kill .....\n";
	cout << "Name : " << buf << "\n"; 
	cout << "/proc entry " << proc_cmdline << "\n";
	cin >> choice;
	if ((choice == 'y' || choice == 'Y') == 0 ) 
	{	
		cout << "not killed \n";
		exit(0);
	}
	kill (pid, SIGKILL);
	cout << "Sleep for some time and check for the process \n";
	sleep (5);
	fp = fopen (proc_cmdline.c_str(), "r");
	if (fp == NULL)
		cout << "Process doesn't exist \n";
	else
	{
		cout << "Process is not yet killed \n";
		fclose (fp); // should close only if fp is not NULL
	}
}

char getch()
{
        char buf = 0;
        struct termios old = {0};
        if (tcgetattr(0, &old) < 0)
                perror("tcsetattr()");
        old.c_lflag &= ~ICANON;
        old.c_lflag &= ~ECHO;
        old.c_cc[VMIN] = 1;
        old.c_cc[VTIME] = 0;
        if (tcsetattr(0, TCSANOW, &old) < 0)
                perror("tcsetattr ICANON");
        if (read(0, &buf, 1) < 0)
                perror ("read()");
        old.c_lflag |= ICANON;
        old.c_lflag |= ECHO;
        if (tcsetattr(0, TCSADRAIN, &old) < 0)
                perror ("tcsetattr ~ICANON");
        return (buf);
}
