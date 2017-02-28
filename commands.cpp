#include <iostream>
#include <string>
#include <sstream>
#include "commands.h"

using namespace std;

vector<file_struct> files;
int file_descriptor = 0;
directory *current_dir = NULL;
vector <string> path;
int depth = 0;

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
      if (new_file.size == 0) {
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
        int current_offset = files[i].offset;
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
  bool new_line = false;
  for (int i=0; i<files.size(); i++) {
    if (files[i].fd == stoi(fd)) {
      file_found = true;
      if (files[i].operation.compare("r") == 0) {
        cout << "File previously opened for reading. Please close file and reopen for writing.\n";
      }
      else {
        string file_contents = files[i].contents;
        contents = remove_quotes(contents);
        int current_offset = files[i].offset;
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
        files[i].size = updated_contents.length();
        files[i].offset = updated_offset;
        if (new_line) files[i].contents = updated_contents+'\n';
        else files[i].contents = updated_contents;
      }
      return;
    }
  }
  if (!file_found) cout << "File does not exist. Please open file with write flag to create file.\n";
}

void seek(string fd, string offset) {;}

void close(string fd) {
  bool file_open = false;
  file_struct *f;
  f = new file_struct;
  bool file_directory = false;
  // Traverse open files vector; if desired file is open, remove it from vector
  for (int i=0; i<files.size(); i++) {
    if (files[i].fd == stoi(fd)) {
      *f = files[i];
      file_open = true;
      files.erase(files.begin()+i);
    }
  }
  // If file exists in directory vector of files, simply update its contents
  if (file_open) {
    for (int i=0; i<current_dir->files.size(); i++) {
      if (current_dir->files[i]->fname == f->fname) {
        file_directory = true;
        current_dir->files[i]->fname = f->fname;
        current_dir->files[i]->size = f->size;
        current_dir->files[i]->offset = f->offset;
        current_dir->files[i]->contents = f->contents;
		current_dir->files[i]->size = f->size;
		current_dir->files[i]->date = f->date;
        return;
      }
    }
  }
  else {
    cout << "File does not exist. Please open file with write flag to create file.\n";
    return;
  }
  // If file does not exist in directory vector of files, push it on
  if (!file_directory) current_dir->files.push_back(f);
}

void mkdir(string dname) {
	directory *dir, *tmpdir, *top_dir;
	vector<string> *spath;
	spath = new vector<string>;
	int i, j;
	bool dir_exists = false;

	split_path(dname, spath);

	top_dir = current_dir;
	for(i = 0; i < spath->size(); i++) {
		for(j = 0; j < current_dir->sub_dirs.size(); j++) {
			if(current_dir->sub_dirs[j]->name == (*spath)[i]) {
				dir_exists = true;
				tmpdir = current_dir->sub_dirs[i];
				break;
			}
		}

		if(dir_exists == false) {
			dir = new directory;
			dir->name = (*spath)[i];
			dir->parent_dir = current_dir;
			dir->size = 0;
			current_dir->sub_dirs.push_back(dir);
			current_dir = dir;
		} else current_dir = tmpdir;

		dir_exists = false;
	}

	current_dir = top_dir;
}

void rmdir(string dname) {
	int i, j;
	directory *tmpdir = NULL;

	//Check if specified directory exists
	for(i = 0; i < current_dir->sub_dirs.size(); i++) {
		//Specified directory was found, move into that directory
		if(current_dir->sub_dirs[i]->name == dname) {
			path.push_back(current_dir->sub_dirs[i]->name);
			current_dir = current_dir->sub_dirs[i];

			//Recursively remove all subdirectories within specified directory
			for(j = 0; j < current_dir->sub_dirs.size(); j++) {
				rmdir(current_dir->sub_dirs[j]->name);
			}

			//Delete all closed files in specified directory
			for(j = 0; j < current_dir->files.size(); j++) {
				delete current_dir->files[j];
			}

			//Delete all open files in specified directory
			for(j = 0; j < files.size(); j++) {
				if(path == files[j].path) {
					files.erase(files.begin()+j);
				}
			}

			//Finally, delete specified directory
			tmpdir = current_dir;
			current_dir = current_dir->parent_dir;
			path.pop_back();
			current_dir->sub_dirs.erase(current_dir->sub_dirs.begin()+i);
			delete tmpdir;

			return;
		}
	}

	//Specified directory was not found
	cout << "Directory does not exist." << endl;
	return;
}

void cd(string dname) {
	int i, j;
	vector<string> *spath;
	bool found = false;

	spath = new vector<string>;

	split_path(dname, spath);

	for(i = 0; i < spath->size(); i++) {
		if((*spath)[i] == ".." && current_dir->parent_dir != NULL) {
			path.pop_back();
			current_dir = current_dir->parent_dir;
			found = true;
		} else {
			for(j = 0; j < current_dir->sub_dirs.size(); j++) {
				if(current_dir->sub_dirs[j]->name == (*spath)[i]) {
					path.push_back((*spath)[i]);
					current_dir = current_dir->sub_dirs[j];
					found = true;
					break;
				}
			}
		}

		if(found == false) {
			cout << "Directory does not exist." << endl;
			return;
		}

		found = false;
	}
}

void ls() {
	print_directory_contents();
}

void cat(string fname) {
  bool file_found = false;
  for (int i=0; i<current_dir->files.size(); i++) {
    if (current_dir->files[i]->fname == fname) {
      file_found = true;
      string contents = current_dir->files[i]->contents;
      for (int j=0; j<contents.length(); j++) {
        if ((contents[j] == '\\') && (contents[j+1] == 'n')) {
          cout << endl;
          j++;
        }
        else cout << contents[j];
      }
      return;
    }
  }
  if (!file_found) cout << "File does not exist. Please open file with write flag to create file.\n";
}

void tree() {
	int i, j;

	if(depth != 0) {
		for(i = 0; i < depth-1; i++) {
			cout << "    ";
		}

		cout << "|" << endl;
	}

	for(i = 0; i < depth; i++) {
		if(i == depth-1) cout << "----";
		else cout << "    ";
	}

	cout << current_dir->name << endl;

	//Recursively call tree on all subdirectories
	for(i = 0; i < current_dir->sub_dirs.size(); i++) {
		depth++;
		path.push_back(current_dir->sub_dirs[i]->name);
		current_dir = current_dir->sub_dirs[i];
		tree();
		current_dir = current_dir->parent_dir;
		path.pop_back();
		depth--;
	}

	//Print closed files
	for(i = 0; i < current_dir->files.size(); i++) {
		for(j = 0; j < depth; j++) {
			cout << "    ";
		}

		cout << "|" << endl;

		for(j = 0; j < depth+1; j++) {
			if(j == depth) cout << "----";
			else cout << "    ";
		}

		cout << current_dir->files[i]->fname << "    "
			 << "size: " << current_dir->files[i]->size << " bytes" << "    "
			 << "created: " << current_dir->files[i]->date << endl;
	}

	//Print open files
	for(i = 0; i < files.size(); i++) {
		if(path == files[i].path) {
			for(j = 0; j < depth; j++) {
				cout << "    ";
			}

			cout << "|" << endl;

			for(j = 0; j < depth+1; j++) {
				if(j == depth) cout << "----";
				else cout << "    ";
			}

			cout << files[i].fname << "    "
				 << "size: " << files[i].size << " bytes" << "    "
				 << "created: " << files[i].date << endl;
		}
	}
}

void import_file(string source, string destination) {;}

void export_file(string source, string destination) {;}

void invalid_cmd(string cmd, string usage) {
  if (usage.compare("") == 0) cout << cmd << ": command not found\n";
  else cout << usage << endl;
}
