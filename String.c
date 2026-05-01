#include "String.h"
#include <stdlib.h> 
#include "Tdata.h"

void print_string(str s){
	printf("%s", s);
}

str str_copy(str s) {
	if (s == NULL) return NULL;
	str copy = (str)malloc(strlen(s) + 1);  // +1 para el '\0'
	strcpy(copy , s);
	return copy;
}

str str_new(const char* cstr) {
	if (cstr == NULL) return NULL;
	size_t len = strlen(cstr);
	str r = (str)malloc(len + 1);
	if (r) memcpy(r, cstr, len + 1);
	return r;
}

int str_equal(str s1, str s2) {
	if (s1 == NULL && s2 == NULL) return 1;
	if (s1 == NULL || s2 == NULL) return 0;
	return strcmp(s1, s2) == 0;
}

str str_concat(str s1, str s2) {
	if (s1 == NULL && s2 == NULL) return NULL;
	if (s1 == NULL) return str_copy(s2);
	if (s2 == NULL) return str_copy(s1);
	size_t len = strlen(s1) + strlen(s2) + 1;
	str r = (str)malloc(len);
	if (r) {
		strcpy(r, s1);
		strcat(r, s2);
	}
	return r;
}

void str_free(str s) {
	if (s) free(s);
}

int str_len(str s) {
	return s ? (int)strlen(s) : 0;
}
