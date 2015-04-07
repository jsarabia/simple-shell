//Joseph Sarabia 
//J2378365
//COP 4600 
//2/25/13


#include <iostream>
#include <istream>
#include <string>
#include <sstream>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <vector>
#include <sys/wait.h>

//#include <pthreads>
using namespace std;

void execute(string command, string dir, vector<string> params);

pid_t pid;

int main()
{	
	

	//char *args[] = {"/bin/ls", (char *) 0 };
	//execv("/bin/ls", args);


	while(true){
		string input, command, dir;
		vector<string> params;
		
		//print the prompt
		cout << "# ";
		//read in input, convert to a stringstream for parsing
		getline (cin,input); 
		stringstream ss;
		ss << input;
		
		ss >> command;
		
		ss >> dir;
		
		while( !ss.eof() ){
			string temp;
			ss >> temp;
			params.push_back(temp);			
		}

		//handle running the users command
		execute(command, dir, params);
	}



	return 0;
}

void execute(string command, string dir, vector<string> params)
{
	if (command.compare("quit") == 0)
		exit(0);
	else if (command.compare("run") == 0) { 
		pid = fork();
		if (pid == 0){
		  
			//handle converting to cstrings for execv
			char *cptr = (char*)dir.c_str();
			char *args[params.size()+2];
			args[0] =(char*) dir.c_str();
			for(int i = 0; i<params.size();i++)
				args[i+1] = (char*)params.at(i).c_str();
			args[params.size()+1] = (char*) 0;
			//for(int i=0;i<params.size()+1;i++)
			//	printf("%s\n", args[i]);			
			int x = execv(cptr, args);	
			if (x == -1)
				cout << "Invalid command" << endl;
			exit(0);
		}
		if (pid > 0){
			pid_t wp; 
			wp = waitpid(pid, NULL, WUNTRACED);
			return;
		}		
	}
	else if (command.compare("background") == 0){
		pid = fork();
		if (pid == 0){
			//handle converting to cstrings for execv
			char *cptr = (char*)dir.c_str();
			char *args[params.size()+2];
			args[0] =(char*) dir.c_str();
			for(int i = 0; i<params.size();i++)
				args[i+1] = (char*)params.at(i).c_str();
			args[params.size()+1] = (char*) 0;
			//for(int i=0;i<params.size()+1;i++)
			//	printf("%s\n", args[i]);
			
			//put the new process on it's own process group (puts in the background)
			setpgid(0,0);
			int x = execv(cptr, args);	
			if (x == -1)
				cout << "Invalid command" << endl;
			exit(0);
		}
		if (pid > 0){
			cout << "Process " << pid << " has been moved to the background." << endl;
			pid_t wp; 
			wp = waitpid(pid, NULL, WNOHANG);
			return;
		}	  
	  
	  
	}

	else if (command.compare("murder") == 0){
		int status = kill(atoi(dir.c_str()), SIGKILL);
		if (status == 0){
		  cout << "Successfully killed process " << atoi(dir.c_str()) << endl;
		}
		else
		  cout << "Unable to kill process " << atoi(dir.c_str()) << endl;
	}
	else if (command.compare("for") == 0){
		if (params.size() < 4){
		  cout << "Syntax error on for loop" << endl;
		  return;
		  
		}
	  
		int start, end;
		start = atoi(dir.c_str());
		
		//params.at(0) should hold "to"
		if(params.at(0).compare("to") != 0){
		  cout << "To expected in for loop" << endl;
		  return;
		  
		}

		end = atoi(params.at(1).c_str());
		
		//check for do 
		if(params.at(2).compare("do") != 0){
		  cout << "Do expected in for loop" << endl;
		  return;
		  
		}
		vector<string> forparams;
		if (params.size() >= 6)
		for (int i = 5; i < params.size(); i++){
		    forparams.push_back(params.at(i));
		}
		
		
		//execute the command based on the for loop entered
		for (int i = start; i <= end; i++){
		    if (params.size() < 5){
		      string temp;
		      execute(params.at(3), temp, forparams);
		      
		    }
		    else  
		    execute(params.at(3), params.at(4), forparams);
		  
		}
		  
	  
	}
	else {
		cout << "invalid command." << endl;
	}
	return;
}
