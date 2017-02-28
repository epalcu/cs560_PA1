#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <sstream>
#include "utils.h"
#include "commands.cpp"

using namespace std;

string file_system = "file_system.txt";
//int file_descriptor = 1;

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

void print_fs_error() {
	cout << "No file system found. Please use 'mkfs' command first." << endl;
	return;
}

void print_path() {
	int i;

	for(i = 0; i < path.size(); i++) {
		//cout << "/" << path[i];
	}

	//cout << "> ";
}

void validate_and_call(vector<string> cmd) {
  int size = cmd.size();
  string usage = "";
  if (cmd[0].compare("mkfs") == 0) {
    if (size == 1) {
		mkfs();
	}
    else invalid_cmd(cmd[0], "Usage: mkfs");

	print_path();
	return;
  }
  else if (cmd[0].compare("open") == 0) {
    if (size == 3) {
		if(current_dir == NULL) print_fs_error();
		else open(cmd[1], cmd[2]);
	}
    else invalid_cmd(cmd[0], "Usage: open [filename] [flag]");

	print_path();
	return;
  }
  else if (cmd[0].compare("read") == 0) {
    if (size == 3) {
		if(current_dir == NULL) print_fs_error();
		else read(cmd[1], cmd[2]);
	}
    else invalid_cmd(cmd[0], "Usage: read [fd] [size]");

	print_path();
	return;
  }
  else if (cmd[0].compare("write") == 0) {
    if (size == 3) {
		if(current_dir == NULL) print_fs_error();
		else write(cmd[1], cmd[2]);
	}
    else invalid_cmd(cmd[0], "Usage: write [fd] [string]");

	print_path();
	return;
  }
  else if (cmd[0].compare("seek") == 0) {
    if (size == 3) {
		if(current_dir == NULL) print_fs_error();
		else seek(cmd[1], cmd[2]);
	}
    else invalid_cmd(cmd[0], "Usage: seek [fd] [offset]");

	print_path();
	return;
  }
  else if (cmd[0].compare("close") == 0) {
    if (size == 2) {
		if(current_dir == NULL) print_fs_error();
		else close(cmd[1]);
	}
    else invalid_cmd(cmd[0], "Usage: close [fd]");

	print_path();
	return;
  }
  else if (cmd[0].compare("mkdir") == 0) {
    if (size == 2) {
		if(current_dir == NULL) print_fs_error();
		else mkdir(cmd[1]);
	}
    else invalid_cmd(cmd[0], "Usage: mkdir [dirname]");

	print_path();
	return;
  }
  else if (cmd[0].compare("rmdir") == 0) {
    if (size == 2) {
		if(current_dir == NULL) print_fs_error();
		else rmdir(cmd[1]);
	}
    else invalid_cmd(cmd[0], "Usage: rmdir [dirname]");

	print_path();
	return;
  }
  else if (cmd[0].compare("cd") == 0) {
    if (size == 2) {
		if(current_dir == NULL) print_fs_error();
		else cd(cmd[1]);
	}
    else invalid_cmd(cmd[0], "Usage: cd [dirname]");

	print_path();
	return;
  }
  else if (cmd[0].compare("ls") == 0) {
    if (size == 1) {
		if(current_dir == NULL) print_fs_error();
		else ls();
	}
    else invalid_cmd(cmd[0], "Usage: ls");

	print_path();
	return;
  }
  else if (cmd[0].compare("cat") == 0) {
    if (size == 2) {
		if(current_dir == NULL) print_fs_error();
		else cat(cmd[1]);
	}
    else invalid_cmd(cmd[0], "Usage: cat [filename]");

	print_path();
	return;
  }
  else if (cmd[0].compare("tree") == 0) {
    if (size == 1) {
		if(current_dir == NULL) print_fs_error();
		else tree();
	}
    else invalid_cmd(cmd[0], "Usage: tree");

	print_path();
	return;
  }
  else if (cmd[0].compare("import") == 0) {
    if (size == 3) {
		if(current_dir == NULL) print_fs_error();
		else import_file(cmd[1], cmd[2]);
	}
    else invalid_cmd(cmd[0], "Usage: import [srcname] [dstname]");

	print_path();
	return;
  }
  else if (cmd[0].compare("export") == 0) {
    if (size == 3) {
		if(current_dir == NULL) print_fs_error();
		else export_file(cmd[1], cmd[2]);
	}
    else invalid_cmd(cmd[0], "Usage: export [srcname] [dstname]");

	print_path();
	return;
  }
  else {
	  invalid_cmd(cmd[0], usage);
	  print_path();
	  return;
  }
}

void create_file_system() {
  directory *dir;
  ofstream ofile;

  //Must delete any previous file system
  if(home_dir != NULL) {
    current_dir = home_dir;
    path.clear();
    path.push_back(current_dir->name);

    //Delete all subdirectories in home directory
    for(int i = 0; i < current_dir->sub_dirs.size(); i++) {
      rmdir(current_dir->sub_dirs[i]->name);
    }

    //Delete all closed files in specified directory
    for(int j = 0; j < current_dir->files.size(); j++) {
      delete current_dir->files[j];
      current_dir->files.erase(current_dir->files.begin()+j);
    }

    //Delete all open files in specified directory
    for(int j = 0; j < files.size(); j++) {
      if(path == files[j].path) {
        files.erase(files.begin()+j);
      }
    }

    current_dir->size = 0;
  } else {
    dir = new directory;
    dir->parent_dir = NULL;
    dir->size = 0;
    dir->name = "home";
    current_dir = dir;
    home_dir = dir;

    //Update path
    path.push_back(current_dir->name);
  }
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

file_struct scan_directory(string fname, file_struct fs, string flag) {
  string line, content, val;
  bool file_found = false;
  bool fd_less = false;
  for (int i=0; i<fd_list.size(); i++) {
    if (fd_list[i] < file_descriptor) {
      fd_less = true;
      fs.fd = fd_list[i];
      fd_list.erase(fd_list.begin()+i);
      break;
    }
  }
  if (!fd_less) {
    fs.fd = file_descriptor;
    file_descriptor++;
  }
  for (int i=0; i<current_dir->files.size(); i++) {
    if (current_dir->files[i]->fname == fname) {
      file_found = true;
      fs.fname = current_dir->files[i]->fname;
      fs.size = current_dir->files[i]->size;
      fs.offset = current_dir->files[i]->offset;
      fs.contents = current_dir->files[i]->contents;
      fs.path = current_dir->files[i]->path;
      fs.operation = flag;
      //cout << current_dir->files[i]->contents << endl;
    }
  }
  if (file_found == false) {
    fs.fname = fname;
    fs.size = 0;
    fs.offset = 0;
	time_t now = time(0);
	fs.date = ctime(&now);
    fs.contents = " ";
    fs.operation = flag;
    fs.path = path;
  }
  return fs;
}

string remove_quotes(string statement) {
  string altered;
  for (int i=1; i<(statement.length()-1); i++) {
    if ((statement[i] == '\\') && (statement[i+1] == 'n')) {
      altered += '\n';
      i++;
    }
    else altered += statement[i];
  }
  return altered;
}

void print_directory_contents() {
	//Print subdirectories
	for(int i=0; i < current_dir->sub_dirs.size(); i++) {
		cout << current_dir->sub_dirs[i]->name << endl;
	}

	//Print closed files
	for(int i=0; i < current_dir->files.size(); i++) {
		cout << current_dir->files[i]->fname << endl;
	}

	//Print open files
	for(int i = 0; i < files.size(); i++) {
		if(path == files[i].path) {
			cout << files[i].fname << endl;
		}
	}
}

void split_path(string path, vector<string> *spath) {
	size_t index;
	string tmpstr1, tmpstr2;

	index = path.find('/');
	if(index != string::npos) {
		tmpstr1 = path.substr(0, index);
		spath->push_back(tmpstr1);
		tmpstr2 = path.substr(index+1);
		split_path(tmpstr2, spath);
	} else {
		spath->push_back(path);
		return;
	}
}

void recursive_write(ostream &ofile) {
  //Recursively traverse through directory tree
  for(int i = 0; i < current_dir->sub_dirs.size(); i++) {
      current_dir = current_dir->sub_dirs[i];
      path.push_back(current_dir->name);
      recursive_write(ofile);
      path.pop_back();
      current_dir = current_dir->parent_dir;
  }

  //Read out closed files
  for(int i = 0; i < current_dir->files.size(); i++) {
    ofile << "File Name: " << current_dir->files[i]->fname << endl;
    ofile << "File Size: " << current_dir->files[i]->size << endl;
    ofile << "File Offset: " << current_dir->files[i]->offset << endl;
    ofile << "File Contents: " << current_dir->files[i]->contents << endl;
    ofile << "File Path: ";
    for (int j=0; j<current_dir->files[i]->path.size(); j++) {
      ofile << "/" << current_dir->files[i]->path[j];
    }
    ofile << endl << endl;
  }
}

void write_file_system() {
  directory *tmpdir;
  vector<string> tmpvec;
  ofstream ofile;
  ofile.open(file_system);

  tmpdir = current_dir;
  tmpvec = path;
  current_dir = home_dir;
  path.clear();
  path.push_back(current_dir->name);
  recursive_write(ofile);
  current_dir = tmpdir;
  path = tmpvec;

  ofile.close();
}
