#ifndef PARSER_LIB_H
#define PARSER_LIB_H

#ifdef __cplusplus
#include <iostream>

extern "C" {
#endif

char* parse(char* in);

unsigned int getSizeTags(char* str);
unsigned int getPos(char* str);

char* handleUnit(void* expr);
void* handleExpr(void* orGroup, void* expr);
void* handleOrGroup(void* andGroup, void* orGroup);
void* handleTreeTag1(char* tag);
void* handleTreeTag2(char* tag, void* expr); 

#ifdef __cplusplus
}

#endif

#endif /* PARSER_LIB_H */
