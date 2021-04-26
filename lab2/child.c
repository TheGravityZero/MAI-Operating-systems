#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdbool.h>
#define SIZEBR 2000

int main(){
	// dynamic string
	char* str = (char*)malloc(sizeof(char)*SIZEBR);
	size_t index = 0;
	char c;
	float res , data[2];
	bool fstvalue = 0;
	while(1){
		// Reading char
		if(read(STDIN_FILENO, &c, sizeof(char)) == -1){
			perror("Err reading char");
			return -1;
		}
		// chech on
		if((c == ' ') || (c =='\n')){
			if(c == '\n'){
				if( data[1] == 0){
					perror("Segmentation 0");
					return -1;
				}
				res = data[0]/data[1];
				printf("%f ",res);
				break;
			}
			if (index > SIZEBR - 1){
				if((str = (char*)realloc(str,sizeof(char)*SIZEBR + 1)) == NULL){
					perror("Err realloc");
					return -1;
				}
			}
			if(!fstvalue){
				str[index] = '\0';
				data[0] = atof(str);
				fstvalue = 1;
				index = 0;
			}
			else{
				str[index] = '\0';
				data[1] = atof(str);
				
				// chech on segmentation 0
				if( data[1] == 0){
					perror("Segmentation 0");
					return -1;
				}
				res = data[0]/data[1];
				data[0] = data[1];
				printf("%f ",res);
				index = 0;
			}
		}
		else{
			// check on buffer overflow
			if (index > SIZEBR - 1){
				if((str = (char*)realloc(str,sizeof(char)*SIZEBR*2)) == NULL){
					perror("Err realloc");
					return -1;
				}
				str[index++] = c;
			}
			else{
				str[index++] = c;
			}
		}
	}
	free(str);
	return 0;
}
