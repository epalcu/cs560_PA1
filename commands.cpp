#include <iostream>
#include "commands.h"

using namespace std;

vector<file_struct> files;

void mkfs() {
  return create_file_system();
}

// TODO: Account for "r" flag (i.e. should not create new file if one doesn't exist like "w" flag does)
void open(string fname, string flag) {
  if ((flag.compare("w") == 0) || (flag.compare("r") == 0)) {
    bool push = true;
    file_struct new_file;
    new_file = scan_file(fname, new_file);
    for (int i=0; i<files.size(); i++) {
      if (files[i].fd == new_file.fd) {
        push = false;
        break;
      }
    }
    if (push) files.push_back(new_file);
    cout << "SUCCESS, fd=" << new_file.fd << endl;
  }
  else return invalid_cmd("open", "Usage: open [filename] [flag]");
}

void read(string fd, string size) {;}

void write(string fd, string contents) {;}

void seek(string fd, string offset) {;}

void close(string fd) {;}

void mkdir(string directory) {;}

void rmdir(string directory) {;}

void cd(string directory) {;}

void ls() {;}

void cat(string fname) {;}

void tree() {;}

void import_file(string source, string destination) {;}

void export_file(string source, string destination) {;}

void invalid_cmd(string cmd, string usage) {
  if (usage.compare("") == 0) cout << cmd << ": command not found\n";
  else cout << usage << endl;
}
