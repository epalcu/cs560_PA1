#include <iostream>
#include <vector>
#include <string>
#include "utils.h"
#include "commands.cpp"

using namespace std;

string file_system = "file_system.txt";
int file_descriptor = 1;

vector<string> break_string(string s) {
  string input;
  vector<string> inputs;
  int q_flag = 0;
  for (int i=0; i<s.length(); i++) {
    input += s[i];
    if (s[i] == '"') {
      if (q_flag == 1) {
        inputs.push_back(input);
        input = "";
        q_flag = 0;
        i=i+1;
      }
      else q_flag = 1;
    }
    else if (((s[i+1] == ' ') || (s[i+1] == '\n')) && (q_flag == 0)) {
      inputs.push_back(input);
      input = "";
      i=i+1;
    }
  }
  return inputs;
}

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

void create_file_system() {
  ofstream ofile;
  ofile.open(file_system);
  ofile.close();
  return;
}

string retrieve_value(string l, int *j) {
  string v = "";
  while (*j < l.length()) {
    v += l[*j];
    *j=*j+1;
  }
  return v;
}

file_struct scan_file(string fname, file_struct fs) {
  ifstream f(file_system);
  string line, content, val;
  bool fname_found = false;
  bool fd_found = false;
  bool size_found = false;
  bool offset_found = false;
  bool contents_found = false;
  int i=0;
  if (f.is_open()) {
    while(getline(f, line)) {
      content = "";
      for (int j=0; j<line.length(); j++) {
        if (content.compare("file name: ") == 0) {
          val = retrieve_value(line, &j);
          if (val.compare(fname) == 0) {
            fname_found = true;
            fs.fname = val;
            break;
          }
        }
        else if ((content.compare("fd: ") == 0) && (fname_found)) {
          val = retrieve_value(line, &j);
          fd_found = true;
          fs.fd = stoi(val);
          break;
        }
        else if ((content.compare("size: ") == 0) && (fname_found)) {
          val = retrieve_value(line, &j);
          size_found = true;
          fs.size = val;
          break;
        }
        else if ((content.compare("offset: ") == 0) && (fname_found)) {
          val = retrieve_value(line, &j);
          offset_found = true;
          fs.offset = val;
          break;
        }
        else if ((content.compare("contents: ") == 0) && (fname_found)) {
          val = retrieve_value(line, &j);
          contents_found = true;
          fs.contents = val;
          break;
        }
        else content += line[j];
      }
      i++;
      if ((fname_found) && (fd_found) && (size_found) && (offset_found) && (contents_found)) break;
    }
  }
  else {
    cout << "Unable to access file system.\n";
    exit(1);
  }
  f.close();
  if ((i == 0) || (fname_found == false)) {
    ofstream ofile;
    ofile.open(file_system, ios::out | ios::app);
    ofile << "file name: " << fname << '\n';
    ofile << "fd: " << file_descriptor << '\n';
    ofile << "size: " << "0" << '\n';
    ofile << "offset: " << "0" << '\n';
    ofile << "contents: " << " " << '\n' << '\n';
    ofile.close();
    fs.fname = fname;
    fs.fd = file_descriptor;
    fs.size = "0";
    fs.offset = "0";
    fs.contents = " ";
    file_descriptor++;
  }
  return fs;
}
