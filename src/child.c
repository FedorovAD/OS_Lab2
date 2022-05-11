#include <stdio.h>
#include <stdbool.h>
#include "unistd.h"

typedef enum{
	reading_suc,
	reading_eol,
	reading_wrong_value,
	reading_eof,
} read_rvl_stat;

read_rvl_stat reading_int(int fd, int* cur){
	char c;
	bool negative = false;
	*cur = 0;
	int k = read(fd, &c, sizeof(char));
	if(c == '-'){
		negative = true;
		k = read(fd, &c, sizeof(char));
	}
	while(k > 0){
		if(c == '\n') {
			if(negative) *cur = -(*cur);
			return reading_eol;
		}
		if(c == ' ') break;
		if(((c < '0') || (c > '9'))){
			return reading_wrong_value;
		}
		*cur = *cur * 10 + c - '0';
		k = read(fd, &c, sizeof(char));
	}
	if(negative) *cur = -(*cur);
	if(k == 0) return reading_eof;
	return reading_suc;
}

int main(){
	int cur = 0;
	float first = 0.0f;
	bool fst = true;
	read_rvl_stat status = reading_int(0, &cur);
	char c = '\n';
	while(status != reading_wrong_value){
		if(fst){
			first = cur;
			fst = false;
		} else{
			if (cur == 0) return -3;
			else {
				first = first / cur; 
			}
		}
		if(status == reading_eof){
			fprintf(stderr, "Test should end with <endline>\n");
			return -1;
		} else if(status == reading_eol){
			break;
		}
		cur = 0;
		status = reading_int(0, &cur);
	}
	write(1, &first, sizeof(float));
	if (status == reading_wrong_value){
		fprintf(stderr, "Wrong value in test_file \n");
		return -2;
	}
	return 0;
}	