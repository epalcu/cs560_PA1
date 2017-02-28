#include <iostream>
#include <string>
#include "utils.cpp"
//#include "fs.h"

using namespace std;

extern vector<file_struct> files;
extern int file_descriptor;

int main(int argc, char* argv[]) {
    string cmd;
    vector<string> command;
    //cout << "> ";
    while(getline(cin, cmd)) { // May end up being its own function and moved into utils.cpp
      if ((cmd.compare("exit()") == 0) || (cmd.compare("exit") == 0)) break;
      cmd += '\n';
      command = break_string(cmd);
      validate_and_call(command);
      //cout << "sh ";
    }
    //NOTE: Uncomment to verify opened files properly placed in files vector!!
    // for (int i=0; i<files.size(); i++) {
    //   cout << files[i].fd << endl;
    // }
    // cout << files.size() << endl;
    return 0;
}
