#include "parser_lib.h"
#include "parser.h"
#include <string>
#include <iostream>
#include <vector>
#include <cstdio>
#include <cstring>

using namespace std;

static const string exp("(<[^\\$]*>)\?");

unsigned int getSizeTags(char* str) {
	string s(str);
	
	if (s=="aa") {
		return 3;
	} else if (s=="bb") {
		return 3;
	} else if (s=="cc") {
		return 3;
	}
	if (s=="ii") {
		return 3;
	} else if (s=="jj") {
		return 3;
	} else if (s=="kk") {
		return 3;
	}
	if (s=="xx") {
		return 3;
	} else if (s=="yy") {
		return 3;
	} else if (s=="zz") {
		return 3;
	} else return 3;
}

unsigned int getPos(char* str) {
	string s(str);
	
	if (s=="aa") {
		return 0;
	} else if (s=="bb") {
		return 1;
	} else if (s=="cc") {
		return 2;
	}
	if (s=="ii") {
		return 0;
	} else if (s=="jj") {
		return 1;
	} else if (s=="kk") {
		return 2;
	}
	if (s=="xx") {
		return 0;
	} else if (s=="yy") {
		return 1;
	} else if (s=="zz") {
		return 2;
	} else return 0;
}

string andStrings(string str1, string str2) {
	unsigned int i1 = 0;
	unsigned int i2 = 0;
	int cnt;
	string res = "";
	
	while (i1 < str1.length() || i2 < str2.length()) {
		if (i1 >= str1.length()) 
		{
			res += str2.substr(i2);
			break;
		}
		if (i2 >= str2.length()) 
		{
			res += str1.substr(i1);
			break;
		}
		
		
		if (str1[i1]=='.' && str2[i2]=='.') 
		{
			res += ".." + exp;
			i1 += exp.length() + 2;
			i2 += exp.length() + 2;
		} 
		else if (str1[i1] != '.' && str2[i2] == '.') 
		{
			res += str1[i1];
			res += str1[i1+1];
			if (str1[i1+2] == '(') {
				res += exp;
				i1 += exp.length() + 2;
				i2 += exp.length() + 2;
			} else {
				i1 += 2;
				i2 += exp.length() + 2;
				cnt = 0;
				do {
					if (str1[i1] == '<') cnt++;
					if (str1[i1] == '>') cnt--;
					res += str1[i1];
					i1++;
				} while (cnt > 0);
			}			
		} 
		else if (str1[i1] == '.' && str2[i2] != '.') 
		{
			res += str2[i2];
			res += str2[i2+1];
			if (str2[i2+2] == '(') {
				res += exp;
				i1 += exp.length() + 2;
				i2 += exp.length() + 2;
			} else {
				i2 += 2;
				i1 += exp.length() + 2;
				cnt = 0;
				do {
					if (str2[i2] == '<') cnt++;
					if (str2[i2] == '>') cnt--;
					res += str2[i2];
					i2++;
				} while (cnt > 0);
			}			
		} 
		else 
		{
			res += str1[i1];
			res += str1[i1+1];
			if (str1[i1+2] == '(' && str2[i2+2] == '(') {
				res += exp;
				i1 += exp.length() + 2;
				i2 += exp.length() + 2;
			} else if (str1[i1+2] == '(') {
				i2 += 2;
				i1 += exp.length() + 2;
				cnt = 0;
				do {
					if (str2[i2] == '<') cnt++;
					if (str2[i2] == '>') cnt--;
					res += str2[i2];
					i2++;
				} while (cnt > 0);
			} else if (str2[i2+2] == '(') {
				i1 += 2;
				i2 += exp.length() + 2;
				cnt = 0;
				do {
					if (str1[i1] == '<') cnt++;
					if (str1[i1] == '>') cnt--;
					res += str1[i1];
					i1++;
				} while (cnt > 0);
			} else {
				i1 += 2;
				i2 += 2;
				int i1Temp = i1;
				int i2Temp = i2;								
				cnt = 0;
				do {
					if (str1[i1] == '<') cnt++;
					if (str1[i1] == '>') cnt--;				
					i1++;
				} while (cnt > 0);			
				do {
					if (str2[i2] == '<') cnt++;
					if (str2[i2] == '>') cnt--;					
					i2++;
				} while (cnt > 0);
				
				res += "<" + andStrings(str1.substr(i1Temp+1,i1-i1Temp-2), str2.substr(i2Temp+1,i2-i2Temp-2)) + ">";				
			}			
		}					
	}
	
	return res;
}


char* handleUnit(void* expr) {
	vector<string>* tags = (vector<string>*) expr;
	string res = "";
	
	for(vector<string>::const_iterator i = tags->begin(); i != tags->end()-1; ++i) {
  	res += "(" + *i + ")||";
  }
  if (tags->size() > 1) {
  	res += "(" + tags->back() + ")";
  } else {
  	res += tags->back();
  }
  res = "(" + res + ")";
  
  delete tags;  
  char* retC = new char[res.length() + 1];
  memcpy(retC, res.c_str(), res.length() + 1);
	return retC;
}

void* handleExpr(void* orGroup, void* expr) { // OR
	vector<string>* tags1 = (vector<string>*) orGroup;
	vector<string>* tags2 = (vector<string>*) expr;
	
	tags1->insert(tags1->end(), tags2->begin(), tags2->end());
	delete tags2;
	
	return orGroup;
}


void* handleOrGroup(void* andGroup, void* orGroup) { // AND
	vector<string>* tags1 = (vector<string>*) andGroup;
	vector<string>* tags2 = (vector<string>*) orGroup;	
	vector<string>* tagsAnd = new vector<string>;
	
	for (unsigned int i = 0; i < tags1->size(); i++) {
		 for (unsigned int j = 0; j < tags2->size(); j++) {
		 		tagsAnd->push_back(andStrings(tags1->at(i),tags2->at(j)));
		 }
	}
	
	delete tags1;
	delete tags2;
	return (void*) tagsAnd;
}


void* handleTreeTag1(char* tag) {
	vector<string>* tags = new vector<string>;
	string s(tag);	
	unsigned int pos = getPos(tag);
	unsigned int size = getSizeTags(tag);
	
	s += exp;
	for (unsigned int i = 0; i < pos; i++) {
		s = ".." + exp + s;
	}
	for (unsigned int i = pos+1; i < size; i++) {
		s = s + ".." + exp;
	} 
	
	tags->push_back(s);
	return (void*) tags;
}

void* handleTreeTag2(char* tag, void* expr) {
	vector<string>* tags = (vector<string>*)expr;
	string s(tag);	
	unsigned int pos = getPos(tag);
	unsigned int size = getSizeTags(tag);
	
	for (unsigned int i = 0; i < pos; i++) {
		s = ".." + exp + s;
	}
	for (unsigned int i = 0; i < tags->size(); i++) { 
		tags->at(i) = s + "<" + tags->at(i) + ">";
		for (unsigned int j = pos+1; j < size; j++) {
		
			tags->at(i) += ".." + exp;
			//cout << "loop ran " << s << endl;
		} 
	}
	
	
	
	return expr;
}




