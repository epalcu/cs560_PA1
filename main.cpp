#include <iostream>
#include <string>
#include "utils.cpp"
#include "fs.cpp"

using namespace std;

int main(int argc, char* argv[]) {
    string cmd;
    vector<string> command;
    cout << "sh ";
    while(getline(cin, cmd)) { // May end up being its own function and moved into utils.cpp
      if (cmd.compare("exit()") == 0) break;
      cmd += '\n';
      command = break_string(cmd);
      validate_and_call(command);
      cout << "sh ";
    }
    //NOTE: Uncomment to verify user inputs properly separated!!
    // for (int i=0; i<command.size(); i++) {
    //   cout << command[i] << endl;
    // }
    // cout << command.size();
    return 0;
}
