#include <sys/types.h>
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <vector>
#include <sstream>
#include <string.h>
using namespace std;

int main() {
	string commandLine;
	while (1){
		char host[128];
		gethostname(host, sizeof host);
		cout << getlogin()  << "@" << host << "$ ";
		getline(cin, commandLine);
		
		bool isComment = false;
		int commentIndex = 0;
		for (unsigned int i = 0; i < commandLine.size(); ++i) {
			if (commandLine.at(i) == '#') {
				commentIndex = i;
				isComment = true;
			} 
		}

		if (isComment) {
			commandLine.erase(commentIndex, commandLine.size() - commentIndex);
		}

		bool isAmp = false;

		for (unsigned int i = 0; i < commandLine.size(); ++i) {
			if (commandLine.at(i) == '&') {
				isAmp = true;
			}
		}

		istringstream commandStream(commandLine);
		string commandName;
		int commandLineSize = commandLine.size();	
		char** commands = new char*[commandLineSize * sizeof(char*)];

		string tD = "/bin/";
		int size = 0;
		for (int i = 0; commandStream >> commandName; ++i) {
			if (i == 0) {
				commandName.insert(0, tD);
			}
			if (commandName != "&") {	
				commands[i] = new char[commandName.length() + 1];
				strcpy(commands[i], commandName.c_str());
			}
			++size;
		}
		if (tD == "/bin/exit") 
			exit(1);

		int pid = fork();
		if (pid == 0) {

			if (-1 == (execv(commands[0], commands))) {
				
				for (int i = 0; i <= size; ++i) {
					delete [] commands[i];
				}
				
				delete [] commands;

				perror("execv failed");
				exit(1);
			}
			exit(0);
		}
		else if (pid > 0 && !isAmp) {
			wait();

				for (int i = 0; i < size; ++i) {
					delete [] commands[i];
				}
				
				delete [] commands;
		}
		else if (pid > 0 && isAmp) {
			continue;

				for (int i = 0; i < size; ++i) {
					delete [] commands[i];
				}
				
				delete [] commands;
		}
		

	}	
}