/* 
 * DictWrapper.h
 * Jan Beran
 *
 */

#ifndef DICTWRAPPER_H
#define	DICTWRAPPER_H

#include <string>
#include <map>

class DictWrapper {
public:
    DictWrapper(const char* dictFile);
    std::string getUtf8(const std::string word) const;
    unsigned int getCodePoint(const std::string word) const;
private:
    bool initialized;
    std::map<std::string,std::string> dict;
    
    bool loadDict(const char* dictFile);
    unsigned int lookupDict(const std::string word) const;
    std::string codePoint2Utf(unsigned int codePoint) const;
};

#endif	/* DICTWRAPPER_H */

