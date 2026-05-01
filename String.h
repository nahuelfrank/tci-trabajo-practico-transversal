#ifndef STRING_H
#define STRING_H

typedef char* str;

str str_copy(str s);
int str_equal(str s1, str s2);
str str_concat(str s1, str s2);
void str_free(str s);
int str_len(str s);
void print_string(str s);
str str_new(const char* cstr);

#endif
