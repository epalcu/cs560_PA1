#include <string>

using namespace std;

// #############################################
// # Responsible for creating the file system. #
// #############################################
void mkfs();

// #############################################################################
// # Checks the open flag for whether it is to be a read or write operation.   #
// # Calls "scan_file" to read in desired file from file system. The resulting #
// # class of information is then placed inside a vector of opened files, if   #
// # not already in there.                                                     #
// #############################################################################
void open(string fname, string flag);

void read(string fd, string size);

void write(string fd, string contents);

void seek(string fd, string offset);

void close(string fd);

void mkdir(string directory);

void rmdir(string directory);

void cd(string directory);

void ls();

void cat(string fname);

void tree();

void import_file(string source, string destination);

void export_file(string source, string destination);

void invalid_cmd(string, string);
