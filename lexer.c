#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<conio.h>
#define MAX_SIZE 64

//function declaration
int isValidFileExtension(char strFileName[]);
void checkFile(FILE *filePtr);
void checkSrc(FILE *filePtr);
void fnLexical(FILE *filePtr);
int fnKeywords(char tempKey[], FILE *fOutput, FILE *fileSrc);
int fnDataTypes(char tempType[], FILE *fOutput, FILE *fileSrc);
int fnIdentifiers(char tempIden[], FILE *fileSrc, FILE *fOutput);
int fnOperator(char optr,FILE *fileSrc,FILE *fOutput);
void fnReadDigit(char tempDigit[], FILE *fOutput,FILE *fileSrc);
void fnReadDecimal(int intCtr, char tempDigit[], FILE *fOutput,FILE *fileSrc);
//end

int fnOperator(char optr,FILE *fileSrc,FILE *fOutput)
{	
char temp;
	switch(optr){
		
		case'+':{
			temp= fgetc(fileSrc);
			if(temp== '='){
				fputs("+=", fOutput);
				fputs("\t\t\t\t\t\tADD_EQUAL\n", fOutput);
				return 1;				
			}else if(temp=='+'){
				fputs("++", fOutput);
				fputs("\t\t\t\t\t\tUNA_INC\n", fOutput);
				return 1;
			}else{
				fputc('+', fOutput);
				fputs("\t\t\t\t\t\tOPTR_ADD\n", fOutput);
				return 1;
			}
		break;
		}
			
		
		case'-':{
			temp =fgetc(fileSrc);
			if(temp == '-'){
				fputs("--",fOutput);
				fputs("\t\t\t\t\t\tUNA_DEC\n",fOutput );
			}else if(temp == '='){
				fputs("-=", fOutput);
				fputs("\t\t\t\t\t\tMIN_EQUAL\n", fOutput);
			}else{
				fputc('-', fOutput);
				fputs("\t\t\t\t\t\tOPTR_SUB\n", fOutput);
			}
		break;	
		}
		
		
		case'*':{
			temp =fgetc(fileSrc);			
			if(temp == '='){
				fputs("*=", fOutput);
				fputs("\t\t\t\t\t\tMUL_EQUAL\n", fOutput);
			}else{
				fputc('*', fOutput);
				fputs("\t\t\t\t\t\tOPTR_MUL\n", fOutput);
			}	
		break;	
		}
		
		
		case'%':{
			temp = fgetc(fileSrc);
			if(temp == '='){
				fputs("%=", fOutput);
				fputs("\t\t\t\t\t\tMOD_EQUAL\n", fOutput);
			}else{
				fputc('%',fOutput );
				fputs("\t\t\t\t\t\tOPTR_MOD\n", fOutput);
			}
		break;
		}
		
		case '^':{
			temp = fgetc(fileSrc);
			if(temp == '='){
				fputs("^=", fOutput);
				fputs("\t\t\t\t\t\tEXP_EQUAL\n", fOutput);
			}else{
				fputc('^',fOutput );
				fputs("\t\t\t\t\t\tOPTR_EXP\n", fOutput);
			}
			break;
		}
		
		case'/':{
			temp = fgetc(fileSrc);
			if(temp == '='){
				fputs("/=", fOutput);
				fputs("\t\t\t\t\t\tDIV_EQUAL\n", fOutput);
			}else{
				fputc('/',fOutput );
				fputs("\t\t\t\t\t\tOPTR_DIV\n", fOutput);
			}
			break;
		}
		
		case'=':{
			temp = fgetc(fileSrc);
			if(temp == '='){
				fputs("==", fOutput);
				fputs("\t\t\t\t\t\tCOND_EQUALS\n", fOutput);
			}else{
				fputc('=',fOutput ); 
				fputs("\t\t\t\t\t\tEQUAL\n", fOutput);	fseek(fileSrc, -1, SEEK_CUR);
			}
			break;
		}
		
		case'>':{
			temp = fgetc(fileSrc);
			if(temp == '='){
				fputs(">=", fOutput);
				fputs("\t\t\t\t\t\tGREAT_EQUAL\n", fOutput);
			}else{
				fputc('>',fOutput );
				fputs("\t\t\t\t\t\tGREAT_THAN\n", fOutput);
			}
			break;
		}
		
		case'<':{
			temp = fgetc(fileSrc);
			if(temp == '='){
				fputs("<=", fOutput);
				fputs("\t\t\t\t\t\tLESS_EQUAL\n", fOutput);
			}else if(temp =='>'){
				fputs("<>",fOutput );
				fputs("\t\t\t\t\t\tNOT_EQUAL\n", fOutput);
			}else{
				fputc('<', fOutput );
				fputs("\t\t\t\t\t\tLESS_THAN\n", fOutput);
			}
			break;
		}
		
		case'{':{
			fputc('{', fOutput);
			fputs("\t\t\t\t\t\tCURL_START\n", fOutput);
			break;	
		}
		
		case'}':{
			fputc('}', fOutput);
			fputs("\t\t\t\t\t\tCURL_END\n", fOutput);
			break;	
		}
		
		case'(':{
			fputc('(', fOutput);
			fputs("\t\t\t\t\t\tPAREN_START\n", fOutput);
			break;
		}
		
		case')':{
			fputc(')', fOutput);
			fputs("\t\t\t\t\t\tPAREN_END\n", fOutput);
			break;
		}
		
		case'[':{
			fputc('[', fOutput);
			fputs("\t\t\t\t\t\tSQR_START\n", fOutput);
			break;
		}
		
		case']':{
			fputc(']', fOutput);
			fputs("\t\t\t\t\t\tSQR_END\n", fOutput);
			break;
		}
		
		case';':{
			fputc(';', fOutput);
			fputs("\t\t\t\t\t\tTERMINATOR\n", fOutput);
			break;
		}
		
		case',':{
			fputc(',', fOutput);
			fputs("\t\t\t\t\t\tSEPARATOR\n", fOutput);
			break;
		}
		
	}  
	 			
}

int fnIdentifiers(char tempIden[], FILE *fileSrc, FILE *fOutput)
{   int intValid;
    int intCtr = 0;
    if(strlen(tempIden)<=64&&isalpha(tempIden[0])&&islower(tempIden[0]))
         {
	    		fputs(tempIden, fOutput); 
           	fputs("\t\t\t\t\tIDENTIFIER\n",fOutput); 
		 	return 1;}
    for(intCtr=0; intCtr<strlen(tempIden); intCtr++){
        if(!(isalnum(tempIden[intCtr])||tempIden[intCtr]=='_'))
           {fputs(tempIden, fOutput); 
           return 0;//fputs("\t\t\t\t\tERROR\n",fOutput); 
		 }
    }
    

}


int fnDataTypes(char tempType[], FILE *fOutput, FILE *fileSrc)
{	char Val = 0;
	
        switch (tempType[0])
        {      case 'I': switch (tempType[1])
               {    case 'n': switch (tempType[2])
                    {    case 't': switch (tempType[3])
					{	case 0:
						{	fputs("Int", fOutput); 
                         		fputs("\t\t\t\t\t\tINT\n",fOutput); 
                         		Val = 1;
						}
                         }
                    }
               } 
        
        
        		case 'C': switch (tempType[1])
               {    case 'h': switch (tempType[2])
                    {    case 'a': switch (tempType[3])
                         {    case 'r': switch (tempType[4])
                             {    case 0:
							{
								fputs("Char", fOutput); 
                                   	fputs("\t\t\t\t\tCHAR\n",fOutput); 
                                   	Val = 1;
							}
                              }
                         }
                    }
               } 
        		
			case 'S': switch (tempType[1])
               {    case 't': switch (tempType[2])
                    {    case 'r': switch (tempType[3])
                         {    case 'i': switch (tempType[4])
                              {     case 'n': switch (tempType[5])
                                    {    case 'g': switch(tempType[6])
			                         {    case 0:
									{	fputs("String", fOutput); 
                                                 fputs("\t\t\t\t\tSTRING\n",fOutput); 
                                                 Val = 1;
					                    }
                                         }
                                    }     
                              }
                         }
                    }
               }
               
              
	  		case 'D': switch (tempType[1])
               {    case 'o': switch (tempType[2])
                    {	case 'u': switch (tempType[3])
                         {    case 'b': switch (tempType[4])
                              {     case 'l': switch (tempType[5])
                                    {    case 'e': switch(tempType[6])
			                         {    case 0:
									{	fputs("Double", fOutput); 
                                              	fputs("\t\t\t\t\tDOUBLE\n",fOutput); 
									 	Val = 1; 
									}
                                         }
                                    }     
                              }
                         }
                    }
               } 
	        	
		   	case 'B': switch (tempType[1])
               {    case 'o': switch (tempType[2])
                    {    case 'o': switch (tempType[3])
                         {    case 'l': switch (tempType[4])
                              {     case 'e': switch (tempType[5])
                                    {    case 'a': switch (tempType[6])
                                         {    case 'n': switch(tempType[7])
										{	case 0:
											{	fputs("Boolean", fOutput); 
                                                   		fputs("\t\t\t\t\tBOOLEAN\n",fOutput); 
												Val = 1;
											}
										 } 
                                              
                                         }
                                    }     
                              }
                         }
                    }
               } 
       		
          	return Val;      
		//default: fputs(tempBuffer,fOutput); fputs("\t\t\t\t\tBOOLEAN\n",fOutput);	
	}

               
}

int fnKeywords(char tempKey[], FILE *fOutput, FILE *fileSrc)
{	char Val = 0;
	switch (tempKey[0])
	{	case 'I': switch (tempKey[1])
       	{    case 'f': 
				{	fputs("If", fOutput);
					fputs("\t\t\t\t\t\tIF\n", fOutput);
					Val = 1;
				}							
 		} 
        
	    case 'C': switch (tempKey[1])
	    {   case 'o': switch (tempKey[2])
 	    	   {    case 'n': switch (tempKey[3])
		        {    case 's': switch (tempKey[4])
		         	   {    case 't': 
					   		{   fputs("Const", fOutput);
							fputs("\t\t\t\t\tCONST\n", fOutput);
							Val = 1;
							}
  			        }
  			   }
   		   }
		} 
        
	   	case 'S': switch (tempKey[1])
		{    case 't': switch (tempKey[2])
		 {	case 'o': switch (tempKey[3])
		     {    case 'p':	
					{   fputs("Stop", fOutput);
						fputs("\t\t\t\t\tSTOP\n", fOutput);
						Val = 1;
					}
		     }
		 }
		}
        
	   	case 'D': switch (tempKey[1])
            {	case 'e': switch (tempKey[2])
                {    case 'f': switch(tempKey[4])
				{	case 0:
					{	fputs("Def", fOutput);
						fputs("\t\t\t\t\t\tDEF\n", fOutput);
						Val = 1;
					}
				}
                }
                case 'o':{fputs("Do", fOutput);
						fputs("\t\t\t\t\t\tDO\n", fOutput);
						Val = 1;}
					
					
		      case 'i': switch(tempKey[2])
			 {	case 'v': 
			 		{	fputs("Div", fOutput);
						fputs("\t\t\t\t\t\tDIV\n", fOutput);
						Val = 1;
					}
			 }
					
            } 
      
 	 	  case 'A': switch (tempKey[1])
            {    case 'n': switch (tempKey[2])
                {    case 'd': switch(tempKey[3])
				{	case 0:
					{	fputs("And", fOutput);
						fputs("\t\t\t\t\t\tAND\n", fOutput);
						Val = 1;
					}
					}
                }
			} 
      		
 		case 'O': switch (tempKey[1])
            {    case 'r': switch(tempKey[2])
				{	case 0:
				{	fputs("Or", fOutput);
					fputs("\t\t\t\t\t\tOR\n", fOutput);
					Val = 1;
				}
				}
            } 
      	
	 	case 'N': switch (tempKey[1])
            {    case 'o': switch (tempKey[2])
                {    case 't': 
					{	fputs("Not", fOutput);
						fputs("\t\t\t\t\t\tNOT\n", fOutput);
						Val = 1;
					}
				}
                
				case 'u': switch (tempKey[2])
	                {    case 'l': switch (tempKey[3])
	                    {    case 'l': 
						{	fputs("Null", fOutput);
							fputs("\t\t\t\t\tNULL\n", fOutput); 
							Val = 1;
						}	
                    }
                }
				
				case 'e': switch (tempKey[2])
                {    case 'x': switch (tempKey[3])
                    {    case 't':
						{	fputs("Next", fOutput);
							fputs("\t\t\t\t\tNEXT\n", fOutput);
							Val = 1;
						}
                    }
                }
            } 
       
	  	  case 'R': switch (tempKey[1])
            {    case 'e': switch (tempKey[2])
                {    case 'a': switch (tempKey[3])
                    {	case 'd': 
						{	fputs("Read", fOutput);
							fputs("\t\t\t\t\tREAD\n", fOutput);
							Val = 1;
						}
                    }
        
				case 't': switch (tempKey[3])
                    {    case 'u': switch (tempKey[4])
                        {    case 'r': switch (tempKey[5])
                            {    case 'n': 
								{	fputs("Return", fOutput);
									fputs("\t\t\t\t\tRETURN\n", fOutput);
									Val = 1;
								}
								
                            }
						}
                    }
                }
            } 
        	  
		  case 'V': switch (tempKey[1])
            {   case 'i': switch (tempKey[2])
                {   case 'e': switch (tempKey[3])
                    {   case 'w': 
                        {	fputs("View", fOutput);
					fputs("\t\t\t\t\tVIEW\n", fOutput);
					Val = 1;
					}
                    }
                }
                        
			case 'o': switch (tempKey[2])
                {   case 'i': switch (tempKey[3])
                    {    case 'd': 
					{	fputs("Void", fOutput);
						fputs("\t\t\t\t\tVOID\n", fOutput);
						Val = 1;
					}
                    }
                }
            } 
            
		  case 'E': switch (tempKey[1])
            {   case 'l': switch (tempKey[2])
                {   case 's': switch (tempKey[3])
                    {   case 'e': 
                        {	fputs("Else", fOutput);
						fputs("\t\t\t\t\tELSE\n", fOutput);
						Val = 1;
					}
                    }
               
				case 'i': switch (tempKey[3])
                    {    case 'f':
						{	fputs("Elif", fOutput);
							fputs("\t\t\t\t\tELIF\n", fOutput);
							Val = 1;
						}
                    }
                }
            } 
            
            case 'T': switch (tempKey[1])
            {   case 'r': switch (tempKey[2])
                {   case 'u': switch (tempKey[3])
                    {   case 'e': 
                        {	fputs("True", fOutput);
						fputs("\t\t\t\t\tTRUE\n", fOutput);
						Val = 1;
					}
                    }
                }
            } 
	
		   case 'W': switch (tempKey[1])
            {   case 'h': switch (tempKey[2])
                {   case 'i': switch (tempKey[3])
                    {   case 'l': switch (tempKey[4])
                        {	case 'e':
                            {	fputs("While", fOutput);
						fputs("\t\t\t\t\tWHILE\n", fOutput);
						Val = 1;
						} 
                        }
                    }
                }
            } 
       
	  	  case 'F': switch (tempKey[1])
            {    case 'o': switch (tempKey[2])
                {    case 'r': 
                    {	fputs("For", fOutput);
					fputs("\t\t\t\t\t\tFOR\n", fOutput);
					Val = 1;
				}
                }
                case 'a': switch (tempKey[2])
                {	case 'l': switch (tempKey[3])
                	{ 	case 's': switch (tempKey[4])
                	  	{	case 'e': 
               	  		{	fputs("False", fOutput);
							fputs("\t\t\t\t\tFALSE\n", fOutput);
							Val = 1;
						}
				     }
				}
	           }
            } 
      
	 	  case 'J': switch (tempKey[1])
		  {    case 'u': switch (tempKey[2])
	                {    case 'm': switch (tempKey[3])
	                    {    case 'p': switch (tempKey[4])
	                        {     case 't': switch (tempKey[5])
							{    case 'o': 
								{	fputs("Jumpto", fOutput);
									fputs("\t\t\t\t\tJUMPTO\n", fOutput);
									Val = 1;
								}
							}     
				         }
	                    }
	                }
 		 } 
        return Val;
        }
     
  
}

void fnReadDecimal(int intCtr, char tempDigit[], FILE *fOutput, FILE *fileSrc){
	int intCtr2=0, intItr=0, flagVal=1;//1 no error, 0 error
	for(intCtr2=intCtr+1; intCtr2<strlen(tempDigit); intCtr2++){
		if(isalpha(tempDigit[intCtr2])){
			flagVal=0;
			break;
		}
		intItr++;
	}
	
	if(flagVal == 0){
		fprintf(fOutput, "%s\t\t\t\t\t%s (Error: Invalid ID)\n", tempDigit, "DOUBLE_NUM");
	}else{
		if(intItr>5){
			fprintf(fOutput, "%s\t\t\t\t\t%s (Error: Out of Range)\n", tempDigit, "DOUBLE_NUM" );
		}else{
			fprintf(fOutput, "%s\t\t\t\t\t%s\n", tempDigit, "DOUBLE_NUM");
		}
	
	}
	fseek(fileSrc, -1, SEEK_CUR);
	
}

void fnReadDigit(char tempDigit[], FILE *fOutput, FILE *fileSrc){
	int intCtr=0, intItr=0, intVal=0, flagVal=1;//1 no error, 0 error
	for(intCtr=0; intCtr<strlen(tempDigit); intCtr++){
		
		if(isalpha(tempDigit[intCtr])){
			flagVal=0;
			break;
		}else if(tempDigit[intCtr]=='.'){
			fnReadDecimal(intCtr, tempDigit, fOutput, fileSrc);
			return;
		}
		intItr++;		
	}
	
	if(flagVal == 0){
		fprintf(fOutput,"%s\t\t\t\t\t%s\n",tempDigit, "ERROR!");
	}else if(flagVal == 1){
		intVal = atoi(tempDigit);
		if(intVal>999999999){
			fprintf(fOutput,"%s\t\t\t\t\t%s \n",tempDigit, "ERROR!");
		}else if(intItr>10){
			fprintf(fOutput,"%s\t\t\t\t\t%s \n",tempDigit, "ERROR! OUT OF RANGE");
		}else{
			fprintf(fOutput,"%s\t\t\t\t\t%s \n",tempDigit, "INTEGER_NUM");
		}
	}
	fseek(fileSrc, -1, SEEK_CUR);
	
}

void fnLexical(FILE *fileSrc)
{	FILE *fOutput = fopen("symbol_table.txt","w");
	char ch;
	char tempVal;
	char tempBuffer[MAX_SIZE];
	
	int intCounter = 0;
 	rewind(fileSrc);
    for(ch=fgetc(fileSrc); ch!=EOF; ch=fgetc(fileSrc))
     {	//recurse:
		memset(tempBuffer, 0, sizeof tempBuffer);
		
  		 intCounter = 0;
  		if(ch == '/' ) {
				
				ch = fgetc(fileSrc);
				if(isalnum(ch) || isspace(ch)){ fprintf(fOutput, "/ \t\t\t\t\tOPTR_DIV\n");}
				char msg[MAX_SIZE] = "COMMENT";
				if(ch == '/') {
					tempBuffer[intCounter++] = '/';
					tempBuffer[intCounter++] = '/';
					while((ch = fgetc(fileSrc)) != '\n') {
						tempBuffer[intCounter++] = ch;
					}
					fprintf(fOutput, "%s\t\t\t\t\t%s\n", tempBuffer, msg);
					
				} else if(ch == '*') {
					char cur;
					ch = ungetc(ch, fileSrc);
					tempBuffer[intCounter++] = ch;
					ch = fgetc(fileSrc);
					do {
						cur = ch;
						ch = fgetc(fileSrc);

						if(cur != '*' || ch != '/') 
						tempBuffer[intCounter++] = cur;

					} while((cur != '*' || ch != '/') && ch != EOF);
					if(cur != '/' && ch == EOF) strcat(msg, "(Unclosed comment)");
					else {
						tempBuffer[intCounter++] = '*';
						tempBuffer[intCounter++] = '/';
					}

					fprintf(fOutput, "%s\t\t\t\t\t%s\n", tempBuffer, msg);
					
					}
					
					}

		if(ch == '"')
		{
			fputc('"',fOutput);
		        	for(; (ch = fgetc(fileSrc)) != '"' && ch != EOF;){
		            	fputc(ch, fOutput);
		        	}
		        	fputc('"',fOutput);
		        	if(ch != EOF)
						fputs("\t\t\t\t\tSTRING_LIT\n", fOutput);
		        	else 
						fputs("\t\t\t\t\tNULL_TOKEN\n",fOutput);
					
		}
		
								
       	if(!isspace(ch) && !isalnum(ch)){
			tempVal = fnOperator(ch,fileSrc,fOutput);
			ch = tempVal ? tempVal:fgetc (fileSrc);
		 }
		 
		 
	   if(isalnum(ch))
        {		for(intCounter=0; isalnum(ch) || ch=='.'; ch = fgetc(fileSrc),intCounter++)
			{     
				 tempBuffer[intCounter]=ch;
            	}	
			tempVal = ch;
			//printf(tempBuffer); printf("\n");
        		if(isdigit(tempBuffer[0])){
        		fnReadDigit(tempBuffer, fOutput,fileSrc);
			}
			else if(isalpha(tempBuffer[0]))
			{
				printf(tempBuffer); printf("\n\n");
				if(isupper(tempBuffer[0]))
				{	if(fnKeywords(tempBuffer, fOutput, fileSrc) == 1){
					//printf(tempBuffer);	printf("\n");
					fseek(fileSrc, -1, SEEK_CUR);
					}else if(fnDataTypes(tempBuffer,fOutput, fileSrc) == 1){
					//printf(tempBuffer);	 printf("\n"); 
					//fseek(fileSrc, -1, SEEK_CUR);
					}else fprintf(fOutput,"%s \t\t\t\t\t ERROR\n",tempBuffer);
				}
				if(islower(tempBuffer[0]))
			 	{	if(fnIdentifiers(tempBuffer,fileSrc,fOutput) == 1){
					//	 printf(tempBuffer); printf("\n");
					fseek(fileSrc, -1, SEEK_CUR);
					}
					else fprintf(fOutput,"%s \t\t\t\t\t ERROR\n",tempBuffer);
				}
 			}
		}
		
	//	if(!isspace(ch)&&ch!=EOF){
//            goto recurse;
//        } 
  


     }
		
}


void checkFile(FILE *filePtr)
{	if(filePtr == NULL)
	{	printf("File does not exist!");
		exit(0);
	}

	fseek(filePtr, 0, SEEK_END);

	if (ftell(filePtr) == 0) 
	{  	printf("File is empty.\n");
        exit(1);
	}else
	{   rewind(filePtr);
	}
}

//checks if source code contains tokens
void checkSrc(FILE *filePtr)
{	char ch;
	while((ch=fgetc(filePtr))!=EOF)
	{	if(ch>32)
		{
			return;
		}

	}
	printf("Your file contains nothing but spaces, tabs and/or new lines.");
	exit(1);
}


int isValidFileExtension(char strFileName[])
{	char *ret = NULL;

    ret = strchr(strFileName, '.');

    if(ret == NULL) return 0;

	char strFileExt[] = ".pype";
	int intCounter;
	int intValid = 0;
	if((strlen(strFileExt))!=(strlen(ret)))
		return 0;

	for(intCounter=0; intCounter<(strlen(strFileExt)); intCounter++)
	{	if(strFileExt[intCounter] == ret[intCounter])
		{	intValid = 1;
		}
	}
   return intValid;
   
}


int main(void)
{	FILE *fSyntax = fopen("reservedsymbols.txt", "r");
	FILE *fileSourceCode;
	char file[64];
	printf("Enter filename: ");
	scanf("%s", file);
	if(isValidFileExtension(file))
	{	fileSourceCode = fopen(file, "r");
		checkFile(fileSourceCode);
		checkSrc(fileSourceCode);
		fnLexical(fileSourceCode);
	}else
	{	printf("FILE NOT FOUND");
	}
	getch();
	return 0;
}
