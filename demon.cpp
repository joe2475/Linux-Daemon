//Joseph Barsis
//cs3377.501
//jtb170930@utdallas.edu

#define MAX_EVENTS 1024
#define LEN_NAME 16
#define EVENT_SIZE (sizeof(struct inotify_event) + PATH_MAX +1)
#define BUF_LEN (MAX_EVENTS * (EVENT_SIZE + LEN_NAME))
#include <iostream> 
#include <fstream>
#include <string>
#include <stdio.h>
#include <unistd.h>
#include<sys/types.h>
#include "project.h"
#include <syslog.h>
#include<signal.h>
#include<sys/stat.h>
#include <stdlib.h>
#include <sys/inotify.h>
#include <linux/limits.h>
#include <ctime>
#include<cstdlib>
#include <sstream>
using namespace std;
ofstream out;
ofstream pid ("cs3377dirmond.pid");   
ofstream logFile ("cs3377dirmond.log");
int length, i = 0, wd;
int fd;
char buffer[BUF_LEN]; 


void signal_callback_handler(int signum)
{
  cout << "Caught kill signal." << endl;
  remove("cs3377dirmond.pid");
    remove("cs3377dirmond.log");

    exit(signum);

}

void copyFile(string dir, string file, string time)
{ 
  string nfile = file + "." + time; 
  ostringstream cmd;
  cmd << "cp " << dir << "/" << file << " " << dir << "/versions/" << nfile;  
  system(cmd.str().c_str());

}

//Used to get the date and time as a string.
string getDateTime()
{
  time_t rawtime;
  struct tm * tinfo;
  char buff[80];
  time (&rawtime);
  tinfo = localtime(&rawtime);
  strftime(buffer,sizeof(buff), "%Y.%m.%d-%H:%M:%S", tinfo);
  string time(buffer);
  return time;

}


void watchDir(string dirWatch, bool daemon)
{
 
  fd = inotify_init();
  if (fd < 0)
    {
      if(daemon == true)
      logFile << "Daemon could not start inotifiy" << endl;
      else
	printf("Could not start inotifiy\n");
    }
  else
    if(daemon == true)
    logFile << "Daemon has started inotify" << endl;
    else 
      printf("Inotify has started\n"); 

  wd = inotify_add_watch(fd, dirWatch.c_str(), IN_MODIFY);

  if (wd == -1)
    {
      if(daemon == true)
      logFile << "Daemon could not watch directory" << endl;
      else
	printf("Could not watch directory.\n");
    }

  else
    {
      if(daemon == true)
      logFile << "Daemon is watching directory " << endl;
      else
	printf("Watching directory\n");
    }
  logFile.close();
}

void notify(bool daemonCheck, string dir)
{
  
      signal(SIGINT, signal_callback_handler);
      i = 0;
      length = read(fd, buffer, BUF_LEN);
      if(length < 0)
	{
	  if(daemonCheck == true)
	  logFile << "Reading" << endl;
	  else
	    printf("Reading");
	}
      while( i < length)
	{

	  struct inotify_event *event = (struct inotify_event * ) &buffer[i];
	  if(event->len)
	    {
	      if(event->mask & IN_MODIFY)
		{
		  if(event-> mask & IN_ISDIR)
		    {
		      if(daemonCheck == true)
			logFile << "Directory %s was modified" << endl;
		      else 
			printf("Directory %s was modified\n", event->name );
		    }

		  else 
		    {
		      if(daemonCheck == true)
			{
			logFile << "File was modified" << endl;
			}
		      else
			{ 
			printf("File %s was modified\n", event->name);
			}
		   string ev = event->name;
		   string time = getDateTime();
		   copyFile(dir, ev, time);  
		    }
		}

		}
	      i+= EVENT_SIZE + event -> len;
	}

    
}




int demonRun(string dir)
{
   
  pid_t forkvalue;
  forkvalue = fork();

  if(forkvalue == -1)
    {
      perror("There was an error in the fork.");
      return 1;
    }
  else if(forkvalue > 0)
    {
      return 0;  

    }
  //Child process is running
  else if(forkvalue == 0)
    {

      
      logFile << "File was created!" << endl; 
      cout << getpid() << endl;
      pid << getpid();
      pid.close();

      logFile << "Daemon was started." << endl;
      watchDir(dir, true); 
       while(true)
      	{
	  notify(true, dir);
	  
	}
    }
  return 0;    	  
}
   

