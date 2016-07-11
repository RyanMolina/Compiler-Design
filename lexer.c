#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include "lexer.h"
#define MAX_CHAR 64



void printList(struct res_symbol *head){
	struct res_symbol *temp = head;

    printf("\n -------Printing list Start------- \n");
    while(temp != NULL){


        printf(" element %s \n",temp->sym);
        printf(" element %s \n",temp->val);
        temp = temp->next;
    }
	printf("-------Printing list End------- \n");
}

struct res_symbol *fnClassifySyntax(FILE *fSyntax){
	struct res_symbol *head= NULL;
	struct res_symbol *curr = NULL;
	char buffer[32];
	int intCounter = 0;
	char *temp;

	//initializer
	while(intCounter < 32){
		buffer[intCounter] = '\0';
		intCounter++;
	}

	//gets header of symbols
	fgets(buffer, 32, fSyntax);

	fgets(buffer, 32, fSyntax);

	 //stores symbol in lists
	 while(buffer[0] != '\n' && (feof(fSyntax)) == 0){

	 				if(head == NULL){
	 					head = malloc(sizeof(struct res_symbol));
						curr = head;
	 				}else{

	 					curr->next = malloc(sizeof(struct res_symbol));
	 					curr= curr->next;
	 				}



        		 	temp = strtok(buffer, "\t");
        			strcpy(curr->sym,temp);
     				temp = strtok(NULL, "\t");
     				strcpy(curr->val,temp);

        		 	fgets(buffer, 32, fSyntax);
        		 	curr->next = NULL;
     }
		//printList(head);
		return head;
}

void fnReadSyntaxList(FILE *fSyntax){
	RESSYM = fnClassifySyntax(fSyntax);
	KEYW = fnClassifySyntax(fSyntax);
}
char tempSymbol[32];
	char tempValue[32];
int fnValidSym2(FILE *fileOutput, char key[], struct res_symbol *curr){

		while(curr!=NULL){
		if(key[0]== curr->sym[0] && key[1] == curr->sym[1]){
				strcpy(tempSymbol, curr->sym);
				strcpy(tempValue, curr->val);
				return 1;
		}
			curr = curr->next;
		}
		return 0;
}

int isValidSym(FILE *fileSrc, FILE *fOutput, char key){




	char sym2[2];

	int intValid = 0;
	sym2[0] = key;
	struct res_symbol *curr = RESSYM;

	while(curr!=NULL){
		if(key == curr->sym[0]){
			strcpy(tempSymbol, curr->sym);
		 	strcpy(tempValue, curr->val);
			return 1;
		}else{
			curr = curr->next;
		}
	}
	return 0;
}



//done
void readDecimal(FILE *fileSrc, FILE *fOutput, char strDIG[], int intCounter){

			char temp;
			int intIteration=1;
			strDIG[intCounter] = '.';
			intCounter++;
			while(isdigit(temp = fgetc(fileSrc))){

				strDIG[intCounter] = temp;
				intCounter++;
				if(intIteration > 5){

					while(!isspace(temp = fgetc(fileSrc)) && temp!=EOF){
							strDIG[intCounter] = temp;
							intCounter++;
					}
					fprintf(fOutput, "%s\t%s (Error: Out of Range)\n", strDIG, "DOUBLE" );
					return;
				}


				intIteration++;
			}


			if(isspace(temp) || temp == EOF){
					fprintf(fOutput, "%s\t%s\n", strDIG, "DOUBLE");
			}else if(isalpha(temp)){
					strDIG[intCounter] = temp;
					intCounter++;
				while(!isspace(temp = fgetc(fileSrc))  && temp!=EOF){
						strDIG[intCounter] = temp;
						intCounter++;
				}
				fprintf(fOutput, "%s\t%s (Error: Invalid ID)\n", strDIG, "DOUBLE");
			}else if(ispunct(temp)){
					if(isValidSym(fileSrc, fOutput, temp)){
						fprintf(fOutput, "%s\t%s\n", strDIG, "DOUBLE");
					}
			}

}

//done
void readDigit(FILE *fileSrc, FILE *fileOutput, char strDIG[], int intCounter){
	int int_val;
	int intDigitCount = 1;
	int int_ret_val; //0 error 1 no error
	char strValue[20];
	char ch;
	while(isdigit(ch = fgetc(fileSrc))){
				strDIG[intCounter]= ch;
				intCounter++;
		if(intDigitCount > 10){
					while(isdigit(ch) && !isspace(ch = fgetc(fileSrc)) && ch!=EOF){
							strDIG[intCounter] = ch;
							intCounter++;
					}
			break;
		}
		intDigitCount++;

	}

  		if(isalpha(ch)){
  			strDIG[intCounter] = ch;
			intCounter++;
    		//if there is alpha num , error
    			while(!isspace(ch= fgetc(fileSrc))  && ch!=EOF){
						if(ispunct(ch) && isValidSym(fileSrc, fileOutput, ch)){
							fseek(fileSrc, -1, SEEK_CUR);
							break;
						}else{
								strDIG[intCounter] = ch;

								intCounter++;
						}

				}
   	 		 	int_ret_val = 0;
  		}
 	 	else if (isspace(ch) || ch == EOF){
 	 		if(intDigitCount > 10){
 	 			fprintf(fileOutput, "%s\tError: Out of Range\n", strDIG);
 	 			return;
 	 		}else{
 	 			int_ret_val = 1;
 	 		}
   			 //if whitespace ,

 	    }
  		else if(ispunct(ch)) {
   			 //call is validsym
    		if(isValidSym(fileSrc, fileOutput, ch)){
    			fseek(fileSrc, -1, SEEK_CUR);
    			int_ret_val = 1;
    		}else if(ch == '.'){
    		readDecimal(fileSrc, fileOutput, strDIG,intCounter);
    		return;
  			}else{
  				int_ret_val = 0;
  			}
 		 }

		if(int_ret_val == 1){
  			int_val = atoi(strDIG);
			//final checking para sure na walang error.
   			 if(int_val > 999999999){
      			fprintf(fileOutput,"%s\t%s \n",strDIG, "ERROR");
    		}
    		else{
      			fprintf(fileOutput,"%s\t%s \n",strDIG, "INT");
    		}
		}else{
  			fprintf(fileOutput,"%s\t%s\n",strDIG, "ERROR");
		}
}

void fnCheckKeyword(FILE *fpIn, FILE *fpOut, char id[]) {

    struct res_symbol symbol, *temp = KEYW;
 	int i = 0;
 	while(temp != NULL) {
        i = 0;
        while(temp->sym[i] == id[i]) {
            if(temp->sym[i] == '\0' && id[i] == '\0') {
		    	fprintf(fpOut, "%s\t%s", temp->sym, temp->val);
            	break;
            }
            i++;
        }
        temp = temp->next;
 	}


}

void fnReadAlpha(int intUpper, FILE *fileSrc, FILE *fileOutput, char strID[]){
	int intIndex = 1;
	char ch;

	while(isalpha(ch = fgetc(fileSrc))){
		if(intIndex >= 64){
			strID[intIndex] = ch;
			intIndex++;
			while(!isspace(ch = fgetc(fileSrc)) && ch!=EOF && (!isValidSym(fileSrc, fileOutput, ch))){
							strID[intIndex] = ch;
							intIndex++;
			}
			puts(strID);
			fprintf(fileOutput, "%s\tINVALID ID/OUT OF RANGE", strID);
			return;
		}

		strID[intIndex] = ch;
		intIndex++;
	}

	if(isspace(ch) || ch == EOF || isValidSym(fileSrc, fileOutput, ch)){
		if(intUpper == 1){
			fnCheckKeyword(fileSrc, fileOutput, strID);
			fseek(fileSrc, -1, SEEK_CUR);
		}else{
			fprintf(fileOutput, "%s\t%s\n", strID, "IDENTIFIER");
			fseek(fileSrc, -1, SEEK_CUR);
		}
	}else{
			strID[intIndex] = ch;
			intIndex++;
			while(!isspace(ch = fgetc(fileSrc)) && ch!=EOF && (!isValidSym(fileSrc, fileOutput, ch))){
							strID[intIndex] = ch;
							intIndex++;
			}
			fprintf(fileOutput, "%s\tINVALID ID/OUT OF RANGE\n", strID);
	}

}



void fnLexical(FILE *fileSrc){
	FILE *fOutput = fopen("symbol_table.txt", "w");
	char tempBuffer[MAX_CHAR];
	int intLineNumber = 1;
	int intValid = 0;
	int intCounter = 0;
	char ch;
	rewind(fileSrc);
	while((ch = (fgetc(fileSrc))) != EOF){
		memset(tempBuffer, 0, sizeof tempBuffer);
		intCounter = 0;
		/* if(ch == '\n')
			intLineNumber++;
		*/


		if(isalpha(ch)){
			if(islower(ch)){
					tempBuffer[0] = ch;
					fnReadAlpha(0, fileSrc, fOutput, tempBuffer);

			}
			else if(isupper(ch)){
				 tempBuffer[0] = ch;
				 fnReadAlpha(1, fileSrc, fOutput, tempBuffer);
			}
		} else if(isdigit(ch)){
				tempBuffer[0] = ch;
				readDigit(fileSrc, fOutput, tempBuffer, 1);
		}else if(ispunct(ch)){


			if(ch == '.'){
				readDecimal(fileSrc, fOutput, tempBuffer, 0);
			}else if(ch == '-'){
				tempBuffer[0] = ch;
				if(ispunct(ch = fgetc(fileSrc))){
					tempBuffer[1] = ch;
					if(!(fnValidSym2(fileSrc, tempBuffer, RESSYM))){
						fseek(fileSrc, -1, SEEK_CUR);
					}else if(ch == '.'){
						readDecimal(fileSrc, fOutput, tempBuffer, 0);
					}
				}else if(isdigit(ch)){
					tempBuffer[1] = ch;
					readDigit(fileSrc, fOutput, tempBuffer, 2);
				}
			}else if(ch == '"'){
				tempBuffer[0] = ch;
				intCounter = 1;

				while((ch=fgetc(fileSrc)) != '"' && ch!= '\n' && ch!=EOF){
					tempBuffer[intCounter] = ch;
					intCounter++;
				}

				if(ch == '"'){
					tempBuffer[intCounter] = ch;
					fprintf(fOutput, "%s\t%s\n", tempBuffer, "STRING");
				}else{
					fprintf(fOutput, "%s\t%s (Missing Closing Quote)\n", "STRING");
				}

			}else if(ch == '\''){
					tempBuffer[0] = ch;
					intCounter = 1;

					while((ch=fgetc(fileSrc))!= '\'' && ch!='\n'){
						tempBuffer[intCounter] = ch;
						intCounter++;
					}
					if(ch == '\''){
					tempBuffer[intCounter] = ch;
					fprintf(fOutput, "%s\t%s\n", tempBuffer, "CHARACTER");
					}else{
					fprintf(fOutput, "%s\t%s (Missing Closing Quote)\n", tempBuffer, "CHARACTER");
					}

			}else if(ch == '/') {
				ch = fgetc(fileSrc);
				char msg[MAX_CHAR] = "COMMENT";
				if(ch == '/') {
					while((ch = fgetc(fileSrc)) != '\n') {
						tempBuffer[intCounter++] = ch;
					}
					fprintf(fOutput, "%s\t%s\n", tempBuffer, msg);

				} else if(ch == '*') {
					char cur;
					ch = fgetc(fileSrc);
					do {
						cur = ch;
						ch = fgetc(fileSrc);

						if(cur != '*' || ch != '/') tempBuffer[intCounter++] = cur;

					} while((cur != '*' || ch != '/') && ch != EOF);
					if(cur != '/' && ch == EOF) strcat(msg, "(Unclosed comment)");
					fprintf(fOutput, "%s\t%s\n", tempBuffer, msg);
				} else {
                    fprintf(fOutput, "/\tDIV\n");
                }







			}else if(isValidSym(fileSrc, fOutput, ch)){
				tempBuffer[0] = ch;

					if((tempBuffer[1] = fgetc(fileSrc)) != EOF && ispunct(tempBuffer[1])){
		 					if(fnValidSym2(fOutput, tempBuffer, RESSYM)){
		 						fprintf(fOutput, "%s\t%s",tempSymbol, tempValue );
		 					}else{
		 						fseek(fileSrc, -1, SEEK_CUR);
		 					}
		 			}else{
		 					fprintf(fOutput, "%s\t%s",tempSymbol, tempValue );
		 					fseek(fileSrc, -1, SEEK_CUR);
		 			}


			} else{
				fprintf(fOutput, "%c\tINVALID SYMBOL",ch);
			}



		}
	}
}
