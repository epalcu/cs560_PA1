#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <ctime>
#include "commands.h"

using namespace std;

vector<file_struct> files;
int file_descriptor = 0;
vector<int> fd_list;
directory *current_dir = NULL;
directory *home_dir = NULL;
vector <string> path;
int depth = 0;

void mkfs() {
  return create_file_system();
}

void open(string fname, string flag) {
  bool duplicate = false;
  file_struct new_file;
	
  // Check to see if file exists in vector of open files
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
	
  // If file not currently open
  if (!duplicate) {
    if (flag.compare("w") == 0) {
	    
      //Scan directory to see if file already exists in directory
      new_file = scan_directory(fname, new_file, flag);
      files.push_back(new_file);
	    
      cout << "SUCCESS, fd=" << new_file.fd << endl;
	    
      return;
    }
    else if (flag.compare("r") == 0) {
	    
      //Scan directory to see if file already exists in directory
      new_file = scan_directory(fname, new_file, flag);
	    
      // Can't read empty file
      if (new_file.size == 0) {
        file_descriptor--;
        cout << "File does not exist. Please open file with write flag to create file.\n";
      }
      else {
	      
        // Place onto vector of open files
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
	
  // Check to see if file in vector of open files
  for (int i=0; i<files.size(); i++) {
    if (files[i].fd == stoi(fd)) {
      file_found = true;
	    
      if (files[i].operation.compare("w") == 0) {
        cout << "File previously opened for writing. Please close file and reopen for reading.\n";
      }
      else {
        string contents = files[i].contents;
        int current_offset = files[i].offset;
        int updated_offset = stoi(size)+current_offset;
        string output;
        int j = 0;
	      
        // Begin reading from specified offset
        for (j=current_offset; j<updated_offset; j++) {
          output += contents[j];
        }
	      
        // Set offset to end of read
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
	
  // Check to see if file exists in open file vector
  for (int i=0; i<files.size(); i++) {
    if (files[i].fd == stoi(fd)) {
      file_found = true;
	    
      if (files[i].operation.compare("r") == 0) {
        cout << "File previously opened for reading. Please close file and reopen for writing.\n";
      }
      else {
        string file_contents = files[i].contents;
        int current_offset, updated_offset;
        string first_half, second_half, updated_contents;
        contents = remove_quotes(contents);
        current_offset = files[i].offset;
	      
        // Upon first write, offset is set to end of file
        if (current_offset == 0)  {
          updated_offset = contents.length();
          updated_contents = contents;
        }
        else {
          updated_offset = contents.length()+current_offset;
		
          // Gather the first half contents preceding the offset
          for (int j=0; j<current_offset; j++) {
            first_half += file_contents[j];
          }
		
          // Update the contents by concatenating the ifrst half of origin alcontents and new contents
          updated_contents = first_half + contents;
		
          // Update contents if newly created contents shorter than previous content amount
          if (updated_offset < file_contents.length()) {
            for (int j=updated_offset; j<file_contents.length(); j++) {
              updated_contents += file_contents[j];
            }
          }
        }
        files[i].offset = updated_offset;
        files[i].size = updated_contents.length();
        files[i].contents = updated_contents;
      }
	    
      return;
    }
  }
  if (!file_found) cout << "File does not exist. Please open file with write flag to create file.\n";
}

void seek(string fd, string offset) {
  bool file_found = false;
	
  // Check to see if file is in vector of open files
  for (int i=0; i<files.size(); i++) {
    if (files[i].fd == stoi(fd)) {
      file_found = true;
	    
      // Set the offset value to index value
      files[i].offset = stoi(offset)-1;
	    
      return;
    }
  }
  if (!file_found) cout << "File does not exist. Please open file with write flag to create file.\n";
}

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
	    
      fd_list.push_back(files[i].fd);
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
        current_dir->files[i]->offset = 0;
        current_dir->files[i]->contents = f->contents;
		    current_dir->files[i]->size = f->size;
		    current_dir->files[i]->date = f->date;
	      
        // Write file to file system
        return write_file_system();
      }
    }
  }
  else {
    cout << "File does not exist. Please open file with write flag to create file.\n";
	  
    return;
  }
	
  // If file does not exist in directory vector of files, push it on
  if (!file_directory) {
    f->offset = 0;
    current_dir->files.push_back(f);
	  
    // Write file to file system
    return write_file_system();
  }
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
      write_file_system();
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
        current_dir->files.erase(current_dir->files.begin()+j);
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
      write_file_system();
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
	
  // Check to see if file exists in directory
  for (int i=0; i<current_dir->files.size(); i++) {
    if (current_dir->files[i]->fname == fname) {
      file_found = true;
      string contents = current_dir->files[i]->contents;
	    
      cout << contents;
	    
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

void import_file(string source, string destination) {
  file_struct *f;
  f = new file_struct;
	
  // Read in file specified in the host machine
  ifstream file(source.c_str());
  stringstream ss;
  ss << file.rdbuf();
  string contents = ss.str();
	
  // Create file_struct for imported file and push onto directory's vector of files
  f->fname = destination;
  f->contents = contents;
  f->size = contents.length();
  f->offset = contents.length();
  time_t now = time(0);
  f->date = ctime(&now);
  f->path = path;
  current_dir->files.push_back(f);
	
  return;
}

void export_file(string source, string destination) {
  bool file_found = false;
  ofstream ofile;
	
  // Check to make sure desired file for exporting actually exists
  for (int i=0; i<current_dir->files.size(); i++) {
    if (current_dir->files[i]->fname == source) {
      file_found = true;
	    
      // Write file out to specified file on host machine
      ofile.open(source, std::ofstream::out | std::ofstream::app);
      ofile << current_dir->files[i]->contents;
      ofile.close();
	    
      return;
    }
  }
  if (!file_found) {
    cout << "File does not exist.\n";
	  
    return;
  }
}

void invalid_cmd(string cmd, string usage) {
  if (usage.compare("") == 0) cout << cmd << ": command not found\n";
  else cout << usage << endl;
}
