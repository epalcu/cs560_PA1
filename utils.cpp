#include <iostream>
#include <vector>
#include "utils.h"

using namespace std;

// #####################################################
// # Function for breaking up user input into commands #
// #####################################################
vector<string> break_string(string s) {
  string input;
  vector<string> inputs;
  int q_flag = 0;
  for (int i=0; i<s.length(); i++) {
    input += s[i];
    if (s[i] == '"') {
      if (q_flag == 1) inputs.push_back(input);
      q_flag = 1;
    }
    if (((s[i+1] == ' ') || (s[i+1] == '\n')) && (q_flag == 0)) {
      inputs.push_back(input);
      input = "";
      i=i+1;
    }
  }
  return inputs;
}
