/* 
 * DictWrapper.cpp
 * Jan Beran
 * 
 */

#include "DictWrapper.h"
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <climits>

using namespace std;

DictWrapper::DictWrapper(const char* dictFile) {
    this->initialized = false;

    if (loadDict(dictFile) == false) {
        cerr << "Error: Dictionary could not be initialized!" << endl;
    } else {
        this->initialized = true;
    }
}

std::string DictWrapper::getUtf8(const std::string word) const {

    unsigned int codePoint = getCodePoint(word);
    if (codePoint == 0) {
        return "";
    }

    string utf = codePoint2Utf(codePoint);
    if (utf == "") {
        cerr << "Warning: code point could not be converted to an UTF-8 string" << endl;
        return "";
    }

    return utf;
}

unsigned int DictWrapper::getCodePoint(const std::string word) const {
    if (this->initialized == false) {
        cerr << "Warning: Lookup cannot be performed, dictionary not initialized" << endl;
        return 0;
    }

    unsigned int codePoint = lookupDict(word);
    if (codePoint == 0) {
        cerr << "Warning: Lookup not successful, type/tag not found in dictionary" << endl;
        return 0;
    }

    return codePoint;
}

bool DictWrapper::loadDict(const char* dictFile) {
    ifstream dictStream;
    dictStream.open(dictFile, ios::in);
    if (!dictStream.is_open()) {
        cerr << "Warning: Dictionary file could not be opened" << endl;
        return false;
    }

    string word, codePoint;
    while (dictStream >> word >> codePoint) {
        this->dict[word] = codePoint;
    }

    dictStream.close();
    return true;
}

unsigned int DictWrapper::lookupDict(const std::string word) const {

    map<string, string>::const_iterator it = this->dict.find(word);
    if (it == dict.end()) {
        return 0;
    }

    unsigned int codePoint = strtoul(this->dict.find(word)->second.c_str(), NULL, 16);
    if (codePoint == 0 || codePoint == ULONG_MAX) {
        return 0;
    }

    return codePoint;
}

std::string DictWrapper::codePoint2Utf(unsigned int codePoint) const {
    int length;
    string out = "";

    if (codePoint <= 0x7f) length = 1;
    else if (codePoint <= 0x7ff) length = 2;
    else if (codePoint <= 0xffff) length = 3;
    else if (codePoint <= 0x1fffff) length = 4;
    else return "";

    for (int i = 0; i < length; i++) {
        out += '\0';
    }

    if (length == 1) {
        out[0] = (char) codePoint;
        return out;
    }

    for (int i = 7; i > 7 - length; i--) {
        out[0] |= (1 << i);
    }

    for (int i = 1; i < length; i++) {
        out[i] |= (1 << 7);
    }

    int byte = length - 1;
    int offset = 0;
    for (int i = 0; i < 5 * length + 1; i++) {
        if (codePoint & (1 << i)) {
            out[byte] |= (1 << offset);
        }
        if (offset == 5) {
            byte--;
            offset = 0;
        } else offset++;
    }

    return out;
}
