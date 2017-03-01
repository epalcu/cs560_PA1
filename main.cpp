#include <iostream>
#include <string>
#include "utils.cpp"

using namespace std;

extern vector<file_struct> files;
extern int file_descriptor;

int main(int argc, char* argv[]) {
    string cmd;
    vector<string> command;
    //cout << "> ";
    while(getline(cin, cmd)) {
      if ((cmd.compare("exit()") == 0) || (cmd.compare("exit") == 0)) break;
      cmd += '\n';
      command = break_string(cmd);
      validate_and_call(command);
    }
    return 0;
}
