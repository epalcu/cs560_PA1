#include <iostream>
#include <string>
#include "commands.h"

using namespace std;

vector<file_struct> files;
int file_descriptor = 0;

void mkfs() {
  return create_file_system();
}

void open(string fname, string flag) {
  bool duplicate = false;
  file_struct new_file;
  for (int i=0; i<files.size(); i++) {
    if (files[i].fname == fname) {
      duplicate = true;
      if (files[i].operation.compare("r") == 0) {
        cout << "File already open for reading.\n";
      }
      else if (files[i].operation.compare("w") == 0) {
        cout << "File already open for writing.\n";
      }
      return;
    }
  }
  if (!duplicate) {
    if (flag.compare("w") == 0) {
      new_file = scan_directory(fname, new_file, flag);
      files.push_back(new_file);
      //TODO: If file does not exist in directory vector, push it into it
      cout << "SUCCESS, fd=" << new_file.fd << endl;
      return;
    }
    else if (flag.compare("r") == 0) {
      new_file = scan_directory(fname, new_file, flag);
      if (new_file.size.compare("0") == 0) {
        file_descriptor--;
        cout << "File does not exist. Please open file with write flag to create file.\n";
      }
      else {
        files.push_back(new_file);
        cout << "SUCCESS, fd=" << new_file.fd << endl;
      }
      return;
    }
    else return invalid_cmd("open", "Usage: open [filename] [flag]");
  }
}

void read(string fd, string size) {
  for (int i=0; i<files.size(); i++) {
    if (files[i].fd == stoi(fd)) {
      if (files[i].operation.compare("w") == 0) {
        cout << "File previously opened for writing. Please close file and reopen for writing.\n";
      }
      else {
        string contents = files[i].contents;
        int current_offset = stoi(files[i].offset);
        int updated_offset = current_offset+stoi(size);
        string output;
        int j = 0;
        for (int j=current_offset; j<updated_offset; j++) {
          output += contents[j];
        }
        files[i].offset = j;
        cout << output << endl;
      }
      return;
    }
    else cout << "File does not exist. Please open file with write flag to create file.\n";
  }
}

void write(string fd, string contents) {
  ;
}

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
