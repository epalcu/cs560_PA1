#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "fs.h"

using namespace std;

// ##########################################################################
// # Function for breaking up user input into commands delimeted by spaces, #
// # at which point each space separated input is placed into a vector.     #
// # Also checks for quotation marks for the writing of content to files.   #
//                                                                          #
// # Ex: write 5 "Hey!!" ---> vector: {write, 5, "Hey!!"}                   #
//                                                                          #
// ##########################################################################
vector<string> break_string(string s);

// ##########################################################################
// # Function for validating whether or not entered command is correct.     #
// # Yeah, I realize that it still looks bad, but it's def more readable    #
// # now! Maybe..                                                           #
//                                                                          #
// # Ex: write 5 ---> Usage: write [fd] [string]                            #
// # Ex: cl ---> cl: command not found                                      #
//                                                                          #
// ##########################################################################
void validate_and_call(vector<string> cmd);

// #################################################################################
// # Called by command "mkfs()", and is repsonsible for creating the "file system" #
// #################################################################################
void create_file_system();

// ############################################################################
// # Responsible for scanning the file system for desired file. If it exists, #
// # its details are placed in a class and entered into a vector to indicate  #
// # that it has been accessed. If the desired file does not exist, it is     #
// # simply created, with newly initialized information.                      #
// ############################################################################
file_struct scan_directory(string fd, file_struct fs, string flag);

// ############################################################################
// # Responsible for removing quotation marks from user contents upon a write #
// # command.                                                                 #
// ############################################################################
string remove_quotes(string statement);

void print_directory_contents();

void split_path(string path, vector<string> *spath);

void recursive_write(ostream &ofile);

void write_file_system();
