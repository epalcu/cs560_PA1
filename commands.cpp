#include <iostream>
#include "commands.h"

using namespace std;

void mkfs() {;}

void open(string fname, string flag) {;}

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
