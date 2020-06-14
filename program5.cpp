//Joseph Barsis
//cs3377.501
//jtb170930@utdallas.edu

#include <iostream>
#include <map>
#include <fstream>
#include <string>
#include <tclap/CmdLine.h>
#include <rude/config.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include "project.h"
#include <signal.h>
#include<sys/stat.h>
#include <syslog.h>
using namespace std;
using namespace rude; 
int main(int argc, char** argv)
{
  TCLAP::CmdLine cmd("CS3377.502 Program 5",' ', "1.0");
  TCLAP::SwitchArg daemonCall("d", "daemon", "Run in daemon mode (forks to run ad daemon).", cmd,false);
  TCLAP::UnlabeledValueArg<string> configFile("config", "The name of the configuration file. Defaults to cs3377dirmond.conf", true, "cs3377dirmond.conf", "conf file name", false); 
  cmd.add(configFile); 

  map<string, string> cmdMap; 
  cmd.parse(argc, argv);

  bool daemon = daemonCall.getValue();
  string confFile = configFile.getValue(); 
  const  char *logFile;
  const char *dirW;
  string dir;  
  Config config;
  string log;  
  cmdMap["File"] = confFile; 
  if(config.load("cs3377dirmond.conf"))
    {
      config.setSection("Parameters");
      logFile = config.getStringValue("LogFile");
      dirW = config.getStringValue("WatchDir");
      dir = dirW;
      log = logFile;
    }
  
  if(daemon == true)
    {
      cmdMap["demon"] = "Summoned";
    }
  else
    {
      cmdMap["demon"] = "Slumber"; 
    }

  if(cmdMap["demon"] == "Summoned")
    {
 
      demonRun(dir); 
      
    }
  else
    {
      watchDir(dir, false);
      while(1)
	{
	  notify(false, dir);
	}
  
      
    }
  
  return 0; 
 


}
