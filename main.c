#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "lexer.h"


void checkFile(FILE *filePtr){

		if(filePtr == NULL){
		printf("File does not exist!");
		exit(0);
	}

	fseek(filePtr, 0, SEEK_END);

	if (ftell(filePtr) == 0) {
        	printf("File is empty.\n");
       	    exit(1);
 	 }else{
        rewind(filePtr);
	 }
}

//checks if source code contains tokens
void checkSrc(FILE *filePtr){
	char ch;
	while((ch=fgetc(filePtr))!=EOF){
		if(ch>32){
			return;
		}

	}
	printf("Your file contains nothing but spaces, tabs and/or new lines.");
	exit(1);
}


int isValidFileExtension(char strFileName[]){
	char *ret = NULL;

    ret = strchr(strFileName, '.');

    if(ret == NULL) return 0;

	char strFileExt[] = ".pype";
	int intCounter;
	int intValid = 0;
	if((strlen(strFileExt))!=(strlen(ret)))
		return 0;

	for(intCounter=0; intCounter<(strlen(strFileExt)); intCounter++){
		if(strFileExt[intCounter] == ret[intCounter]){
			intValid = 1;
		}
	}
   return intValid;
   
}


int main(int argc, char *argv[]){
	FILE *fSyntax = fopen("reservedsymbols.txt", "r");
	FILE *fileSourceCode;

	if(isValidFileExtension(argv[1])){

			fileSourceCode = fopen(argv[1], "r");
			checkFile(fileSourceCode);
			checkSrc(fileSourceCode);
			fnReadSyntaxList(fSyntax);

			fnLexical(fileSourceCode);
	}else{
		printf("FILE NOT FOUND");
	}
	return 0;
}
