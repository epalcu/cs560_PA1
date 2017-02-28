#include <vector>
#include <string>

using namespace std;

extern string file_system;

class file_struct {
    public:
      string fname;
      int fd;
      int size;
      int offset;
      string contents;
      string operation;
	  string date;
      vector<string> path;
};

class directory {
    public:
        directory *parent_dir;
        vector<file_struct *> files;
        vector<directory *> sub_dirs;
        int size;
		string name;
};
