#include <vector>
#include <string>

using namespace std;

class file_struct {
    public:
        int size;
        int offset;
        string contents;
};

class directory {
    public:
        vector<directory *> parent_dirs;
        vector<file_struct *> files;
        vector<directory *> sub_dirs;
        int size;
};
