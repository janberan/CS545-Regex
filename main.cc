#include <iostream>
#include <cstdlib>
#include <string>
#include "parser_lib.h"
#include "DictWrapper.h"
#include <fstream>
#include <re2/re2.h>

using namespace std;


#define DICT_FILE "dict.txt"
#define DB_FILE "db.txt"

bool match(string regexStr, const char* filename) {
	// compile the regexp
  RE2 regexp(regexStr);
  if (!regexp.ok()) {
    cerr << "Regexp could not be compiled" << endl;
    return 1;
  }  
  
  // open the input file
  ifstream txtFile;
  txtFile.open (filename, ios::in);
  if (txtFile.fail())
  {
    cerr << "File could not be opened" << endl;
    return 1;
  }
  
  // process the file
  string line;
  while (getline(txtFile, line))
  {    
    if (RE2::PartialMatch(line, regexp))
    {
    	//cout << line << endl;
      return true;
    }
  }
  return false; 
}  


int main (int argc, char** argv) {
	if (argc != 4) {
		cout << "Usage: "<< argv[0] << " <word> <lemma> <tags>" << endl;
		return 1;
	}
	
	DictWrapper dictionary(DICT_FILE);
	string word, lemma, regex;
	string any = "..";
	
	if (any.compare(argv[1]) == 0) {
		word = ".";
	} else {
		word = dictionary.getUtf8(argv[1]);
	}
	if (any.compare(argv[2]) == 0) {
		lemma = ".";
	} else {
		lemma = dictionary.getUtf8(argv[2]);
	}
	if (any.compare(argv[3]) == 0) {
		regex = word + word + lemma + lemma + "[^\\$]*\\$";	
	} else {
		regex = word + word + lemma + lemma + parse(argv[3]) + "\\$";	
	}
	
	bool res = match(regex,DB_FILE);
	
	cout << "Regex: " << regex << endl;
	cout << "Matched: " << res << endl;
	
	
	
	
	return 0;
}

