using namespace std;

#include<iostream>
#include<fstream>
#include<dirent.h>
#include<ctype.h>
#include<string>
#include<stdlib.h>

void listProc(); //opens proc, reads the folders, checks if process and lists the names in a file. static for now, later refresh every 10 sec
void procDetails(); //lists the details of all processes

main()
{
	listProc();
	procDetails();
}

void listProc()
{
	ofstream data;
	data.open("pid.txt",ios::in|ios::out|ios::trunc);
	DIR *proc;
	struct dirent *folder;
	if(proc=opendir("/proc"))
	{
		while(folder=readdir(proc))
		{
			if(isdigit(folder->d_name[0]))
			{
				long 
temp=strtol(folder->d_name,NULL,10);
				//cout<<temp<<endl;
				data<<temp<<endl;
			}
		}
	}
	else
		perror("Error");
	closedir(proc);
	data.close();
}

void procDetails()
{
	ifstream data,stat;
	DIR *proc;
	data.open("pid.txt",ios::in);
	while(!data.eof())
	{
		string pidString;
		long pid;
		//getline(data,pidString);
		data>>pidString;
		pid=strtol(pidString.c_str(),NULL,10);
		string addrString = "/proc/" +to_string(pid);
		cout<<"\n"<<pid<<endl;
		const char *addr = addrString.c_str(); //opendir accepts const char * only
		cout<<addr<<endl;
		if(proc = opendir(addr))
		{
		//	if(!(strcmp(pidString,"0")))
		//	{
				//system("cat stat");
				//stat.open("status",ios::in);
				FILE *stat;
				char line[1024];
				stat=fopen("status","r");
				//fgets(line,sizeof(line),stat);
				fscanf(stat,"%s",line);
				printf("\nout:%s",line);
				//cout<<temp
				//puts(temp.c_str());
				/*while(!stat.eof())
				{
					char t;
					stat>>t;
					cout<<t;
				}*/
				//stat.close();
				int n=fclose(stat);
		//	}
		}
		//else
		//	perror("Error!");
		closedir(proc);
	}
	data.close();
}
