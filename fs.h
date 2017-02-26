#include <vector>
#include <string>

using namespace std;

extern string file_system;

class file_struct {
    public:
      string fname;
      int fd;
      string size;
      string offset;
      string contents;
      string operation;
	  string date;
};

class directory {
    public:
        directory *parent_dir;
        vector<file_struct *> files;
        vector<directory *> sub_dirs;
        int size;
		string name;
};
