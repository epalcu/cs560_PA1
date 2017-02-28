#include <iostream>
#include <vector>
#include <string>
#include <ctime>
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
		cout << "/" << path[i];
	}

	cout << "> ";
}

void validate_and_call(vector<string> cmd) {
  int size = cmd.size();
  string usage = "";
  if (cmd[0].compare("mkfs") == 0) {
    if (size == 1) {
		mkfs();
		print_path();
		return;
	}
    else return invalid_cmd(cmd[0], "Usage: mkfs");
  }
  else if (cmd[0].compare("open") == 0) {
    if (size == 3) {
		if(current_dir == NULL) print_fs_error();
		else open(cmd[1], cmd[2]);

		print_path();
		return;
	}
    else return invalid_cmd(cmd[0], "Usage: open [filename] [flag]");
  }
  else if (cmd[0].compare("read") == 0) {
    if (size == 3) {
		if(current_dir == NULL) print_fs_error();
		else read(cmd[1], cmd[2]);

		print_path();
		return;
	}
    else return invalid_cmd(cmd[0], "Usage: read [fd] [size]");
  }
  else if (cmd[0].compare("write") == 0) {
    if (size == 3) {
		if(current_dir == NULL) print_fs_error();
		else write(cmd[1], cmd[2]);

		print_path();
		return;
	}
    else return invalid_cmd(cmd[0], "Usage: write [fd] [string]");
  }
  else if (cmd[0].compare("seek") == 0) {
    if (size == 3) {
		if(current_dir == NULL) print_fs_error();
		else seek(cmd[1], cmd[2]);

		print_path();
		return;
	}
    else return invalid_cmd(cmd[0], "Usage: seek [fd] [offset]");
  }
  else if (cmd[0].compare("close") == 0) {
    if (size == 2) {
		if(current_dir == NULL) print_fs_error();
		else close(cmd[1]);

		print_path();
		return;
	}
    else return invalid_cmd(cmd[0], "Usage: close [fd]");
  }
  else if (cmd[0].compare("mkdir") == 0) {
    if (size == 2) {
		if(current_dir == NULL) print_fs_error();
		else mkdir(cmd[1]);

		print_path();
		return;
	}
    else return invalid_cmd(cmd[0], "Usage: mkdir [dirname]");
  }
  else if (cmd[0].compare("rmdir") == 0) {
    if (size == 2) {
		if(current_dir == NULL) print_fs_error();
		else rmdir(cmd[1]);

		print_path();
		return;
	}
    else return invalid_cmd(cmd[0], "Usage: rmdir [dirname]");
  }
  else if (cmd[0].compare("cd") == 0) {
    if (size == 2) {
		if(current_dir == NULL) print_fs_error();
		else cd(cmd[1]);

		print_path();
		return;
	}
    else return invalid_cmd(cmd[0], "Usage: cd [dirname]");
  }
  else if (cmd[0].compare("ls") == 0) {
    if (size == 1) {
		if(current_dir == NULL) print_fs_error();
		else ls();

		print_path();
		return;
	}
    else return invalid_cmd(cmd[0], "Usage: ls");
  }
  else if (cmd[0].compare("cat") == 0) {
    if (size == 2) {
		if(current_dir == NULL) print_fs_error();
		else cat(cmd[1]);

		print_path();
		return;
	}
    else return invalid_cmd(cmd[0], "Usage: cat [filename]");
  }
  else if (cmd[0].compare("tree") == 0) {
    if (size == 1) {
		if(current_dir == NULL) print_fs_error();
		else tree();

		print_path();
		return;
	}
    else return invalid_cmd(cmd[0], "Usage: tree");
  }
  else if (cmd[0].compare("import") == 0) {
    if (size == 3) {
		if(current_dir == NULL) print_fs_error();
		else import_file(cmd[1], cmd[2]);

		print_path();
		return;
	}
    else return invalid_cmd(cmd[0], "Usage: import [srcname] [dstname]");
  }
  else if (cmd[0].compare("export") == 0) {
    if (size == 3) {
		if(current_dir == NULL) print_fs_error();
		else export_file(cmd[1], cmd[2]);

		print_path();
		return;
	}
    else return invalid_cmd(cmd[0], "Usage: export [srcname] [dstname]");
  }
  else return invalid_cmd(cmd[0], usage);
}

void create_file_system() {
  directory *dir;

  dir = new directory;
  dir->parent_dir == NULL;
  dir->size = 0;
  dir->name = "home";
  current_dir = dir;

  //Update path
  path.push_back(current_dir->name);

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

file_struct scan_directory(string fname, file_struct fs, string flag) {
  string line, content, val;
  bool file_found = false;
  for (int i=0; i<current_dir->files.size(); i++) {
    if (current_dir->files[i]->fname == fname) {
      file_found = true;
      fs = *current_dir->files[i];
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
    fs.fd = file_descriptor;
    fs.operation = flag;
    fs.path = path;
    file_descriptor++;
  }
  return fs;
}

string remove_quotes(string statement) {
  string altered;
  for (int i=1; i<(statement.length()-1); i++) {
    altered += statement[i];
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
		cout << files[i].fname << endl;
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

// int check_file_existance(vector<file_struct> v, string fd=false, string fname=false) {
//   int not_found = -1;
//   for (int i=0; i<v.size(); i++) {
//     if (fd) {
//       if (v[i].fd == stoi(fd)) return i;
//     }
//     else {
//       if (v[i].fname == fname) return i;
//     }
//   }
//   return not_found;
// }
