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
// # Yeah, I realize that it looks disgusting, but..it works!!              #
//                                                                          #
// # Ex: write 5 ---> Usage: write [fd] [string]                            #
// # Ex: cl ---> cl: command not found                                      #
//                                                                          #
// ##########################################################################
void validate_and_call(vector<string> cmd) {
  string usage = "";
  if (cmd.size() == 1) {
    if (cmd[0].compare("mkfs") == 0) return mkfs();
    else if (cmd[0].compare("mkfs") == 0) usage = "Usage: open [filename] [flag]";
    else if (cmd[0].compare("read") == 0) usage = "Usage: read [fd] [size]";
    else if (cmd[0].compare("write") == 0) usage = "Usage: write [fd] [string]";
    else if (cmd[0].compare("seek") == 0) usage = "Usage: seek [fd] [offset]";
    else if (cmd[0].compare("close") == 0) usage = "Usage: close [fd]";
    else if (cmd[0].compare("mkdir") == 0) usage = "Usage: mkdir [dirname]";
    else if (cmd[0].compare("rmdir") == 0) usage = "Usage: rmdir [dirname]";
    else if (cmd[0].compare("cd") == 0) usage = "Usage: cd [dirname]";
    else if (cmd[0].compare("ls") == 0) return ls();
    else if (cmd[0].compare("cat") == 0) usage = "Usage: cat [filename]";
    else if (cmd[0].compare("tree") == 0) return tree();
    else if (cmd[0].compare("import") == 0) usage = "Usage: import [srcname] [dstname]";
    else if (cmd[0].compare("export") == 0) usage = "Usage: export [srcname] [dstname]";
    return invalid_cmd(cmd[0], usage);
  }
  else if (cmd.size() == 2) {
    if (cmd[0].compare("mkfs") == 0) usage = "Usage: mkfs";
    else if (cmd[0].compare("open") == 0) usage = "Usage: open [filename] [flag]";
    else if (cmd[0].compare("read") == 0) usage = "Usage: read [fd] [size]";
    else if (cmd[0].compare("write") == 0) usage = "Usage: write [fd] [string]";
    else if (cmd[0].compare("seek") == 0) usage = "Usage: seek [fd] [offset]";
    else if (cmd[0].compare("close") == 0) return close(cmd[1]);
    else if (cmd[0].compare("mkdir") == 0) return mkdir(cmd[1]);
    else if (cmd[0].compare("rmdir") == 0) return rmdir(cmd[1]);
    else if (cmd[0].compare("cd") == 0) return cd(cmd[1]);
    else if (cmd[0].compare("ls") == 0) usage = "Usage: ls";
    else if (cmd[0].compare("cat") == 0) return cat(cmd[1]);
    else if (cmd[0].compare("tree") == 0) usage = "Usage: tree";
    else if (cmd[0].compare("import") == 0) usage = "Usage: import [srcname] [dstname]";
    else if (cmd[0].compare("export") == 0) usage = "Usage: export [srcname] [dstname]";
    return invalid_cmd(cmd[0], usage);
  }
  else if (cmd.size() == 3) {
    if (cmd[0].compare("mkfs") == 0) usage = "Usage: mkfs";
    else if (cmd[0].compare("open") == 0) return open(cmd[1], cmd[2]);
    else if (cmd[0].compare("read") == 0) return read(cmd[1], cmd[2]);
    else if (cmd[0].compare("write") == 0) return write(cmd[1], cmd[2]);
    else if (cmd[0].compare("seek") == 0) return seek(cmd[1], cmd[2]);
    else if (cmd[0].compare("close") == 0) usage = "Usage: close [fd]";
    else if (cmd[0].compare("mkdir") == 0) usage = "Usage: mkdir [dirname]";
    else if (cmd[0].compare("rmdir") == 0) usage = "Usage: rmdir [dirname]";
    else if (cmd[0].compare("cd") == 0) usage = "Usage: cd [dirname]";
    else if (cmd[0].compare("ls") == 0) usage = "Usage: ls";
    else if (cmd[0].compare("cat") == 0) usage = "Usage: cat [filename]";
    else if (cmd[0].compare("tree") == 0) usage = "Usage: tree";
    else if (cmd[0].compare("import") == 0) return import_file(cmd[1], cmd[2]);
    else if (cmd[0].compare("export") == 0) return export_file(cmd[1], cmd[2]);
    return invalid_cmd(cmd[0], usage);
  }
  else return invalid_cmd(cmd[0], usage);
}
