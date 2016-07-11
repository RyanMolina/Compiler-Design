#include<stdio.h>
struct res_symbol{
	
	char sym[32];
	char val[32];
	struct res_symbol *next;
} *RESSYM, *KEYW;


void printList(struct res_symbol *head);
struct res_symbol *fnClassifySyntax(FILE *fSyntax);

void fnReadSyntaxList(FILE *fSyntax);
void readDecimal(FILE *fileSrc, FILE *fOutput, char strDIG[], int intCounter);
void readDigit(FILE *fileSrc, FILE *fileOutput, char strDIG[], int intCounter);
void fnReadAlpha(int intUpper, FILE *fileSrc, FILE *fileOutput, char strID[]);
int fnValidSym2(FILE *fileOutput, char key[], struct res_symbol *curr);
int isValidSym(FILE *fileSrc, FILE *fOutput, char key);
void fnLexical(FILE *fSrc);
