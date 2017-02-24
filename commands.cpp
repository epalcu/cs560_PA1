#include <iostream>
#include <string>
#include <sstream>
#include "commands.h"

using namespace std;

vector<file_struct> files;
vector<file_struct> directory; // Dummy vector that will be replaced by vector of actual directory
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
  bool file_found = false;
  for (int i=0; i<files.size(); i++) {
    if (files[i].fd == stoi(fd)) {
      file_found = true;
      if (files[i].operation.compare("w") == 0) {
        cout << "File previously opened for writing. Please close file and reopen for reading.\n";
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
  }
  if (!file_found) cout << "File does not exist. Please open file with write flag to create file.\n";
}

void write(string fd, string contents) {
  bool file_found = false;
  stringstream ss;
  for (int i=0; i<files.size(); i++) {
    if (files[i].fd == stoi(fd)) {
      file_found = true;
      if (files[i].operation.compare("r") == 0) {
        cout << "File previously opened for reading. Please close file and reopen for writing.\n";
      }
      else {
        string file_contents = files[i].contents;
        contents = remove_quotes(contents);
        int current_offset = stoi(files[i].offset);
        int updated_offset = current_offset+contents.length();
        string first_half, second_half, updated_contents;
        int j = 0;
        for (j=current_offset+1; j<file_contents.length(); j++) {
          second_half += file_contents[j];
        }
        for (j=0; j<(current_offset); j++) {
          first_half += file_contents[j];
        }
        updated_contents = first_half + contents + second_half;
        ss << updated_contents;
        cout << ss << endl;
        ss >> updated_contents;
        files[i].size = updated_contents.length();
        files[i].offset = to_string(updated_offset);
        files[i].contents = updated_contents;
      }
      return;
    }
  }
  if (!file_found) cout << "File does not exist. Please open file with write flag to create file.\n";
}

void seek(string fd, string offset) {;}

void close(string fd) {
  bool file_open = false;
  file_struct f;
  bool file_directory = false;
  for (int i=0; i<files.size(); i++) {
    if (files[i].fd == stoi(fd)) {
      f = files[i];
      file_open = true;
      files.erase(files.begin()+i);
    }
  }
  if (file_open) {
    for (int i=0; i<directory.size(); i++) {
      if (directory[i].fname == f.fname) {
        file_directory = true;
        directory[i].fname = f.fname;
        directory[i].size = f.size;
        directory[i].offset = f.offset;
        directory[i].contents = f.contents;
        return;
      }
    }
  }
  else {
    cout << "File does not exist. Please open file with write flag to create file.\n";
    return;
  }
  if (!file_directory) directory.push_back(f);
}

void mkdir(string directory) {;}

void rmdir(string directory) {;}

void cd(string directory) {;}

void ls() {;}

void cat(string fname) {
  bool file_found = false;
  for (int i=0; i<directory.size(); i++) {
    if (directory[i].fname == fname) {
      file_found = true;
      cout << directory[i].contents << endl;
      return;
    }
  }
  if (!file_found) cout << "File does not exist. Please open file with write flag to create file.\n";
}

void tree() {;}

void import_file(string source, string destination) {;}

void export_file(string source, string destination) {;}

void invalid_cmd(string cmd, string usage) {
  if (usage.compare("") == 0) cout << cmd << ": command not found\n";
  else cout << usage << endl;
}
