#include <iostream>
#include <vector>
#include <string>
#include "utils.h"
#include "commands.cpp"

using namespace std;

// ##########################################################################
// # Function for breaking up user input into commands delimeted by spaces, #
// # at which point each space separated input is placed into a vector.     #
// # Also checks for quotation marks for the writing of content to files.   #
//                                                                          #
// # Ex: write 5 "Hey!!" ---> vector: {write, 5, "Hey!!"}                   #
//                                                                          #
// ##########################################################################
vector<string> break_string(string s) {
  string input;
  vector<string> inputs;
  int q_flag = 0;
  for (int i=0; i<s.length(); i++) {
    input += s[i];
    if (s[i] == '"') {
      if (q_flag == 1) inputs.push_back(input);
      q_flag = 1;
    }
    if (((s[i+1] == ' ') || (s[i+1] == '\n')) && (q_flag == 0)) {
      inputs.push_back(input);
      input = "";
      i=i+1;
    }
  }
  return inputs;
}

// ##########################################################################
// # Function for validating whether or not entered command is correct.     #
// # Yeah, I realize that it still looks bad, but it's def more readable    #
// # now! Maybe..                                                           #
//                                                                          #
// # Ex: write 5 ---> Usage: write [fd] [string]                            #
// # Ex: cl ---> cl: command not found                                      #
//                                                                          #
// ##########################################################################
void validate_and_call(vector<string> cmd) {
  int size = cmd.size();
  string usage = "";
  if (cmd[0].compare("mkfs") == 0) {
    if (size == 1) return mkfs();
    else return invalid_cmd(cmd[0], "Usage: mkfs");
  }
  else if (cmd[0].compare("open") == 0) {
    if (size == 3) return open(cmd[1], cmd[2]);
    else return invalid_cmd(cmd[0], "Usage: open [filename] [flag]");
  }
  else if (cmd[0].compare("read") == 0) {
    if (size == 3) return read(cmd[1], cmd[2]);
    else return invalid_cmd(cmd[0], "Usage: read [fd] [size]");
  }
  else if (cmd[0].compare("write") == 0) {
    if (size == 3) return write(cmd[1], cmd[2]);
    else return invalid_cmd(cmd[0], "Usage: write [fd] [string]");
  }
  else if (cmd[0].compare("seek") == 0) {
    if (size == 3) return seek(cmd[1], cmd[2]);
    else return invalid_cmd(cmd[0], "Usage: seek [fd] [offset]");
  }
  else if (cmd[0].compare("close") == 0) {
    if (size == 2) return close(cmd[1]);
    else return invalid_cmd(cmd[0], "Usage: close [fd]");
  }
  else if (cmd[0].compare("mkdir") == 0) {
    if (size == 2) return mkdir(cmd[1]);
    else return invalid_cmd(cmd[0], "Usage: mkdir [dirname]");
  }
  else if (cmd[0].compare("rmdir") == 0) {
    if (size == 2) return rmdir(cmd[1]);
    else return invalid_cmd(cmd[0], "Usage: rmdir [dirname]");
  }
  else if (cmd[0].compare("cd") == 0) {
    if (size == 2) return cd(cmd[1]);
    else return invalid_cmd(cmd[0], "Usage: cd [dirname]");
  }
  else if (cmd[0].compare("ls") == 0) {
    if (size == 1) return ls();
    else return invalid_cmd(cmd[0], "Usage: ls");
  }
  else if (cmd[0].compare("cat") == 0) {
    if (size == 2) return cat(cmd[1]);
    else return invalid_cmd(cmd[0], "Usage: cat [filename]");
  }
  else if (cmd[0].compare("tree") == 0) {
    if (size == 1) return tree();
    else return invalid_cmd(cmd[0], "Usage: tree");
  }
  else if (cmd[0].compare("import") == 0) {
    if (size == 3) return import_file(cmd[1], cmd[2]);
    else return invalid_cmd(cmd[0], "Usage: import [srcname] [dstname]");
  }
  else if (cmd[0].compare("export") == 0) {
    if (size == 3) return export_file(cmd[1], cmd[2]);
    else return invalid_cmd(cmd[0], "Usage: export [srcname] [dstname]");
  }
  else return invalid_cmd(cmd[0], usage);
}
