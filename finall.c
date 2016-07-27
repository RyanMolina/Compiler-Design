#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <conio.h>
#define MAX_SIZE 9999
#define ERROR 404
#define ID 305
#define INT 962
#define BOOLEAN 866
#define CHAR 851
#define STRING 124
#define DOUBLE 762
#define AND 869
#define OR 610
#define NOT 662
#define READ 198
#define VIEW 249
#define IF 990
#define ELIF 959
#define ELSE 951
#define WHILE 344
#define DO 960
#define FOR 961
#define DIV 942
#define VOID 264
#define RETURN 192
#define CONST 766
#define STOP 126
#define NEXT 693
#define JUMPTO 426
#define MAIN 684
#define TRUE 111
#define FALSE 101
#define STRLIT 121
#define CHARLIT 122
#define COMMENT 956
#define INTCONST 753 
#define DECCONST 791
#define INCR 430
#define DECR 450
#define EQUAL 611
#define GE 620
#define LE 601
#define NE 606
#define DONE 888
#define LBRC 123
#define RBRC 125






int lookahead = 0;
char currToken[64];
FILE *fileSrc;
FILE *fOutput;
FILE *fParser;
int ctr = 0;

report_error(int symbol) {
	switch(symbol) {
		case '(':
		case ')':
		case '[':
		case ']':
		case '{':
		case '}':
		case '\'':
		case '\"':
		case ';': fprintf(stdout, "error: missing \'%c\'\n", symbol); break;
		case ',': fprintf(stdout, "error: missing value before \'%c\'\n", symbol); break;
		case ELIF: fprintf(stdout, "error: misplaced \"Elif\"\n"); break;
		case ELSE: fprintf(stdout, "error: misplaced \"Else\"\n"); break;
		case '+':
		case '-':
		case '*':
		case '/':
		case GE:
		case LE:
		case NE:
		case AND:
		case OR: fprintf(stdout, "%s error: missing operator\n", currToken); break;

		default: fprintf(stdout, "syntax error\n"); break;
	}
}

match(int t){

	if(t == lookahead) {
		lookahead = lexer();

	} else {
		report_error(t);
	}
}

parse_Pype(){
	fprintf(fParser, "<PROGRAM PYPE>\n");
	fprintf(fParser, "\t<Program Block>\n");
	parse_ProgramBlock();
}


parse_ProgramBlock() {

	fprintf(fParser, "\t\t%s\n", currToken);
	match(VOID);
	fprintf(fParser, "\t\t%s\n", currToken);
	match(MAIN);
	fprintf(fParser, "\t\t%s\n", currToken);
	match('(');
	fprintf(fParser, "\t\t%s\n", currToken);
	match(')');
	fprintf(fParser, "\t\t%s\n", currToken);
	match('{');
	fprintf(fParser, "\t\t<Stmt Sequence>\n", currToken);
	parse_StmtSeq();
	fprintf(fParser, "\t\t%s\n", currToken);
	match('}');
}


parse_StmtSeq(){
	
	while(1){
		fprintf(fParser, "\t\t\t<Single Stmt>\n");
		switch(lookahead){
			case '\n':
			case ID:
			case READ:
			case VIEW:
			case INT:
			case CHAR:
			case DOUBLE:
			case STRING:
			case BOOLEAN:
			case INCR:
			case DECR:
			case IF:
			case DO:
			case WHILE:
			case ELIF:
			case ELSE:
			case FOR:
			case JUMPTO:
			case STOP:
			case NEXT: 
			case CONST:
				parse_Single();
				continue;
				break;
			case ';': match(';'); continue;
			default:
				return;
		}
	}
}

parse_Single(){
	switch(lookahead){
		case '\n':
			match('\n');
			fprintf(fParser, "\t\t\t\tNEWLINE\n");
			break;
		case ID:
		case READ:
		case VIEW:
		case INT:
		case CHAR:
		case DOUBLE:
		case STRING:
		case BOOLEAN:
		case INCR:
		case DECR:
		case JUMPTO:
		case STOP:
		case NEXT: 
		case CONST:
			fprintf(fParser, "\t\t\t\t<Simple Stmt>\n");
			parse_Simple();
			fprintf(fParser, "\t\t\t\t%s\n", currToken);
			match(';');
			break;
		case IF:
		case DO:
		case WHILE:
		case FOR:
			fprintf(fParser, "\t\t\t\t<Compound Stmt>\n");
			parse_Compound();
			break;
		case ELSE: parse_Else(); report_error(ELSE); break;
		case ELIF: parse_Elif(); report_error(ELIF); break;
		default:
			report_error(lookahead);
	}
}

parse_Simple(){
	
	switch(lookahead){
		case ID:
			fprintf(fParser, "\t\t\t\t\t<Assignment Stmt>\n");
			parse_AssignStmt();
			break;
		case READ:
			fprintf(fParser, "\t\t\t\t\t<Input Stmt>\n");
			parse_Input();
			break;
		case VIEW:
			fprintf(fParser, "\t\t\t\t\t<Output Stmt>\n");
			parse_Output();
			break;
		case INT:
		case CHAR:
		case DOUBLE:
		case STRING:
		case BOOLEAN:
			fprintf(fParser, "\t\t\t\t\t<Declaration Stmt>\n");
			parse_Declare();
			break;
		case INCR:
		case DECR:
			fprintf(fParser, "\t\t\t\t\t<Unary Stmt>\n");
			parse_Prefix();
			break;
		case JUMPTO:
			fprintf(fParser, "\t\t\t\t\t<Jump Stmt>\n");
			parse_Jump();
			break;
		case STOP:
			fprintf(fParser, "\t\t\t\t\t%s\n", currToken);
			match(STOP);
			
			break;
		case NEXT: 
			fprintf(fParser, "\t\t\t\t\t%s\n", currToken);
			match(NEXT);
			break;
		case CONST:
			fprintf(fParser, "\t\t\t\t\t<Declare Constant Stmt>\n");
			parse_DeclareConst();
			
			break;
		default:
			report_error(lookahead);
	}
}

parse_Jump(){
	fprintf(fParser, "\t\t\t\t\t\t%s\n", currToken);
	match(JUMPTO);
	fprintf(fParser, "\t\t\t\t\t\t%s\n", currToken);
	match(INTCONST);
}

parse_DeclareConst(){
	fprintf(fParser, "\t\t\t\t\t\t%s\n", currToken);
	match(CONST);
	fprintf(fParser, "\t\t\t\t\t\t<Declare Stmt>\n");
	parse_Declare();
}

parse_Compound(){
	switch(lookahead) {
		case IF:
			fprintf(fParser, "\t\t\t\t\t<IF Stmt>\n");
			parse_If();
			break;
		case DO:
			fprintf(fParser, "\t\t\t\t\t<DO Stmt>\n");
			parse_Do();
			break;
		case WHILE:
			fprintf(fParser, "\t\t\t\t\t<WHILE Stmt>\n");
			parse_While();
			break;
		case FOR:
			fprintf(fParser, "\t\t\t\t\t<FOR Stmt>\n");
			parse_For();
			break;
		default:
			report_error(lookahead);
	}
}

parse_AssignStmt(){
	fprintf(fParser, "\t\t\t\t\t\t%s\n", currToken);
	match(ID);
	fprintf(fParser, "\t\t\t\t\t\t<Assignment/Unary Stmt>\n");
	parse_Assign_Unary_Stmt();
	
}

parse_Assign_Unary_Stmt(){
	switch(lookahead){
		case '=':
			fprintf(fParser, "\t\t\t\t\t\t\t%s\n", currToken);
			match('=');
			fprintf(fParser, "\t\t\t\t\t\t\t<Assign Expr>\n");
			parse_AssignExpr();
			break;
		case INCR:
			fprintf(fParser, "\t\t\t\t\t\t\t%s\n", currToken);
			match(INCR);
			break;
		case DECR:
			fprintf(fParser, "\t\t\t\t\t\t\t%s\n", currToken);
			match(DECR);
			break;
	}

}

parse_AssignExpr(){
	switch(lookahead){
		
		case STRLIT:
		case CHARLIT:
			fprintf(fParser, "\t\t\t\t\t\t\t\t<Literal>\n");
			parse_Literal();
			break;
		case TRUE:
		case FALSE:
			fprintf(fParser, "\t\t\t\t\t\t\t\t<Boolean>\n");
			parse_Boolean();
			break;
		case INTCONST: 
		case DECCONST:
		case ID:
		case INCR:
		case DECR:
		case '(':
			fprintf(fParser, "\t\t\t\t\t\t\t\t<Arithmetic/Unary Expr>\n");
			parse_Arith_Unary();
			break;
		default:
			report_error(lookahead);
	}
	
}

parse_Arith_Unary(){
	switch(lookahead){
		case '(':
		case INTCONST: 
		case DECCONST: 
	    case ID:
	    case INCR:
	    case DECR:
	    	fprintf(fParser, "\t\t\t\t\t\t\t\t\t<Operand>\n");
	    	parse_Operand();
	    	parse_Arith_Expr();
	    	break;
	    default:
	    	report_error(lookahead);
	}
}


parse_Arith_Expr(){
	while(1){
		fprintf(fParser, "\t\t\t\t\t\t\t\t\t<Arith Expr>\n");
		switch(lookahead){
		case '+':
		case '-':
			fprintf(fParser, "\t\t\t\t\t\t\t\t\t\t<Arith Plus>\n");
			parse_Arith_Plus();
			fprintf(fParser, "\t\t\t\t\t\t\t\t\t\t<Arithmetic/Unary Expr>\n");
			parse_Arith_Unary();
			continue;
			break;
		default:
			return;
		}
	}	
}

parse_Arith_Plus(){
	switch(lookahead){
		case '+':
			fprintf(fParser, "\t\t\t\t\t\t\t\t\t\t\t%s\n", currToken);
			match('+');
			break;
		case '-':
			fprintf(fParser, "\t\t\t\t\t\t\t\t\t\t\t%s\n", currToken);
			match('-');
			break;
	    default:
	    	report_error('+');
	}
	
}
parse_Operand(){
	switch(lookahead){
		case INTCONST: 
		case DECCONST: 
	    case ID:
	    case INCR:
	    case DECR:
	    case '(':
	    	fprintf(fParser, "\t\t\t\t\t\t\t\t\t\t<Factor>\n");
	    	parse_Factor();
	    	
	    	parse_Operand1();
	    	break;
	    default:
	    	report_error(lookahead);
	}

}

parse_Operand1(){
	while(1){
		fprintf(fParser, "\t\t\t\t\t\t\t\t\t\t<Operand\'>\n");
		switch(lookahead){
			case '*':
			case '/':
			case DIV:
			case '%':
				fprintf(fParser, "\t\t\t\t\t\t\t\t\t\t\t<Arith Muldiv>\n");
				parse_Arith_Muldiv();
				fprintf(fParser, "\t\t\t\t\t\t\t\t\t\t\t<Operand>\n");
				parse_Operand();
				continue;
				break;
			default:
				return;
		}	
	}
}

parse_Arith_Muldiv(){
	switch(lookahead){
		case '*':
			fprintf(fParser, "\t\t\t\t\t\t\t\t\t\t\t\t%s\n", currToken);
			match('*');
			break;
		case '/':
			fprintf(fParser, "\t\t\t\t\t\t\t\t\t\t\t\t%s\n", currToken);
			match('/');
			break;
		case DIV:
			fprintf(fParser, "\t\t\t\t\t\t\t\t\t\t\t\t%s\n", currToken);
			match(DIV);
			break;
		case '%':
			fprintf(fParser, "\t\t\t\t\t\t\t\t\t\t\t\t%s\n", currToken);
			match('%');
			break;
	    default:
	    	report_error(lookahead);
	}
}


parse_Factor(){
	switch(lookahead){
		case INTCONST: 
		case DECCONST: 
	    case ID:
	    case INCR:
	    case DECR:
	    case '(':
	    	fprintf(fParser, "\t\t\t\t\t\t\t\t\t\t\t<Term>\n");
	    	parse_Term();
	    	parse_Factor1();
	    	break;
	    default:
	    	report_error(lookahead);
	}

}

parse_Factor1(){
	while(1){
		fprintf(fParser, "\t\t\t\t\t\t\t\t\t\t\t<Factor\'>\n");
		switch(lookahead){
			case '^':
				match('^');
				fprintf(fParser, "\t\t\t\t\t\t\t\t\t\t\t\t<Factor>\n");
				parse_Factor();
				continue;
				break;
			default:
				return;
		}
	}
}

parse_Term(){
	switch(lookahead){
		case INTCONST:
		case DECCONST:
			fprintf(fParser, "\t\t\t\t\t\t\t\t\t\t\t\t<Digit>\n");
			parse_Digit();
			break;
		case ID:
			fprintf(fParser, "\t\t\t\t\t\t\t\t\t\t\t\t%s\n", currToken);
			match(ID);
			fprintf(fParser, "\t\t\t\t\t\t\t\t\t\t\t\t<Unary Op>\n", currToken); 
			parse_Unary_Op();
			break;
		case INCR:
		case DECR:
			fprintf(fParser, "\t\t\t\t\t\t\t\t\t\t\t\t<Prefix>\n");
			parse_Prefix();
			break;
		case '(':
			fprintf(fParser, "\t\t\t\t\t\t\t\t\t\t\t\t%s\n", currToken);
			match('(');
			fprintf(fParser, "\t\t\t\t\t\t\t\t\t\t\t\t<Arithmetic/Unary Expr>\n", currToken);
			parse_Arith_Unary();
			fprintf(fParser, "\t\t\t\t\t\t\t\t\t\t\t\t%s\n", currToken);
			match(')');
	    default:
	    	report_error(lookahead);
	}

}

	parse_Digit(){
	switch(lookahead){
		case INTCONST:
			fprintf(fParser, "\t\t\t\t\t\t\t\t\t\t\t\t\t%s\n", currToken);
			match(INTCONST);
			break;
		case DECCONST:
			fprintf(fParser, "\t\t\t\t\t\t\t\t\t\t\t\t\t%s\n", currToken);
			match(DECCONST);
			break;
	}
	
}

parse_Literal(){
	switch(lookahead){
		case STRLIT:
			fprintf(fParser, "\t\t\t\t\t\t\t\t\t\t\t\t\t%s\n", currToken);
			match(STRLIT);
			break;
		case CHARLIT:
			fprintf(fParser, "\t\t\t\t\t\t\t\t\t\t\t\t\t%s\n", currToken);
			match(CHARLIT);
			break;
	    default:
	    	report_error(lookahead);
	}
}

parse_Boolean(){
	switch(lookahead){
		case TRUE:
			fprintf(fParser, "\t\t\t\t\t\t\t\t\t\t\t\t\t%s\n", currToken);
			match(TRUE);
			break;
		case FALSE:
			fprintf(fParser, "\t\t\t\t\t\t\t\t\t\t\t\t\t%s\n", currToken);
			match(FALSE);
			break;
	    default:
	    	report_error(lookahead);
	}
}

parse_Prefix(){
	switch(lookahead){
		case INCR:
			fprintf(fParser, "\t\t\t\t\t\t\t\t\t\t\t\t%s\n", currToken);
			match(INCR);
			break;
		case DECR:
			fprintf(fParser, "\t\t\t\t\t\t\t\t\t\t\t\t%s\n", currToken);
			match(INCR);
			break;
	    default:
	    	report_error(lookahead);
	}
	fprintf(fParser, "\t\t\t\t\t\t\t\t\t\t\t\t<Unary Expr\n");
	parse_Unary_Expr();
}

parse_Unary_Expr(){
	switch(lookahead){
		case ID:
			fprintf(fParser, "\t\t\t\t\t\t\t\t\t\t\t\t%s\n", currToken);
			match(ID);
			break;

		case INTCONST:
		case DECCONST:
			fprintf(fParser, "\t\t\t\t\t\t\t\t\t\t\t\t<Digit>");
			parse_Digit();
			break;
		case '(':
			match('(');
			fprintf(fParser, "\t\t\t\t\t\t\t\t\t\t\t\t<Arith Unary>");
			parse_Arith_Unary();
			fprintf(fParser, "\t\t\t\t\t\t\t\t\t\t\t\t%s\n", currToken);
			match(')');
			break;
		default:
			report_error(lookahead);
	}
	
}
parse_Unary_Op(){
	switch(lookahead){
		case INCR:
			match(INCR);
			break;
		case DECR:
			match(DECR);
			break;
	    default:
	    	return;
	}
}

parse_Input(){
	match(READ);
	match('(');
	match(ID);
	match(')');
}

parse_Output(){
	match(VIEW);
	match('(');
	parse_OutputExpr();
	match(')');
}

parse_OutputExpr(){
	switch(lookahead){
		case STRLIT:
			match(STRLIT);
			parse_OutputExpr2();
			return;
			break;
		case ID:
			match(ID);
			return;
			break;
	}	
}

parse_OutputExpr2(){
	switch(lookahead){
		case ',': case '+':
		  parse_Args();
		  return;
		  break;
		default:
		 return;
	}
}

parse_Args(){
   switch(lookahead){
   	   case ',':
   	   	  while(1){
   	   	  	switch(lookahead){
   	   	  		case ',':
   	   	  			match(',');
   	   	  			match(ID);
   	   	  			continue;
   	   	  			break;
   	   	  		default:
   	   	  			return;
   	   	  	}
   	   	  }
   	   	  break;
   	   	case '+':
   	   	  while(1){
   	   	  	switch(lookahead){
   	   	  		case '+':
   	   	  			match('+');
   	   	  			parse_OutputExpr();
   	   	  			continue;
   	   	  			break;
   	   	  		default:
   	   	  			return;
   	   	  	}
   	   	  }
   	   	  break;
   }
}

parse_Declare(){
	switch(lookahead){
		case INT:
		case CHAR:
		case STRING:
		case BOOLEAN:
		case DOUBLE:
			parse_DataType();
			break;
		default:
			report_error(lookahead);
	}
	match(ID);
	parse_DeclareExpr();
}

parse_DataType(){
	switch(lookahead){
		case INT:
			match(INT);
			break;
		case CHAR:
			match(CHAR);
			break;
		case STRING:
			match(STRING);
			break;
		case BOOLEAN:
			match(BOOLEAN);
			break;
		case DOUBLE:
			match(DOUBLE);
			break;
		default:
			report_error(lookahead);
	}
}	

parse_DeclareExpr(){
	switch(lookahead){
		case ',':
			parse_Multiple();
			break;
		case '=':
			parse_Init();
			parse_Multiple();
			break;
		case '[':
			parse_DeclareArray();
			break;
		default:
			return;
	}
}

parse_Multiple(){
	while(1){
		
		switch(lookahead){
			case ',':
				match(',');
				match(ID);
				parse_Init();
				continue;
			default:
				return;
		}
		
	}
}
parse_Init(){
	switch(lookahead){
		case '=':
			match('=');
			parse_Constant();
			break;
		default:
			return;
			break;
	}
}

parse_DeclareArray(){
	match('[');
	match(INTCONST);
	match(']');
	parse_Array_Init();
}

parse_Array_Init(){
	switch(lookahead){
		case '=':
			match('=');
			match('{');
			parse_Array_Init_List();
			match('}');
			break;
		default:
			return;
	}

}

parse_Array_Init_List(){
	switch(lookahead){
		case INTCONST:
		case DECCONST:
		case STRLIT:
		case CHARLIT:
		case TRUE:
		case FALSE:
			 parse_Constant();
			 while(1){
				switch(lookahead){
					case ',':
			 			match(',');
			 			parse_Constant();
			 			continue;
			 			break;
					default:
						return;
				}

			 }
			 break;
		default:
			return;
	}
}

parse_Constant(){
	switch(lookahead){
		case INTCONST:
		case DECCONST:
			parse_Digit();
			break;
		case STRLIT:
		case CHARLIT:
			parse_Literal();
			break;
		case TRUE:
		case FALSE:
			parse_Boolean();
			break;
		default:
			report_error(lookahead);
	}
} 

parse_If(){
	match(IF);
	match('(');
	parse_LogicalExpr();
	match(')');
	match('{');
	parse_StmtSeq();
	match('}');
	parse_Elif();
	parse_Else();
}

parse_Elif(){
	while(1){
		switch(lookahead){
			case ELIF:
				match(ELIF);
				match('(');
				parse_LogicalExpr();
				match(')');
				match('{');
				parse_StmtSeq();
				match('}');
				continue;
				break;
			default:
				return;
		}
	}

}

parse_Else(){
	switch(lookahead){
		case ELSE:
				match(ELSE);
	match('{');
	parse_StmtSeq();
	match('}');
	break;
}

}

parse_Do(){
	match(DO);
	match('{');
	parse_StmtSeq();
	match('}');

	match(WHILE);
	match('(');
	parse_LogicalExpr();
	match(')');
	match(';');
}

parse_While(){
	match(WHILE);
	match('(');
	parse_LogicalExpr();
	match(')');
	match('{');
	parse_StmtSeq();
	match('}');
}
parse_Block() {

}

parse_For(){
	match(FOR);
	match('(');
	match(ID);
	match('=');
	parse_For_Init();
	match(';');
	parse_For_Cond();
	match(';');
	parse_For_Incr();
	match(')');
	match('{');
	parse_StmtSeq();
	match('}');
}

parse_For_Init(){
	switch(lookahead){
		case INTCONST:
		case DECCONST:
			parse_Digit();
			break;
		case ID:
			match(ID);
			break;
		default:
			report_error(lookahead);
	}
}

parse_For_Cond(){
	match(ID);
	parse_RelOp();
	parse_For_Init();
}

parse_For_Incr(){
	switch(lookahead){
		case ID:
			match(ID);
			parse_For_Unary();
			break;
		case INCR:
		case DECR:
			parse_For_Unary();
			match(ID);
			break;
		default:
			report_error(lookahead);
	}
}

parse_For_Unary(){
	switch(lookahead){
		case INCR:
			match(INCR);
			break;
		case DECR:
			match(DECR);
			break;
		default:
			report_error(lookahead);
	}
}

parse_LogicalExpr(){
	switch(lookahead){
		case NOT:
		case INTCONST:
		case DECCONST:
		case ID:
		case INCR:
		case DECR:
		case '(':
		case TRUE:
		case FALSE:
			parse_And();
			parse_LogicalExpr1();
			break;
		default:
			;
			//report_error(lookahead); FIXME
	}
}

parse_LogicalExpr1(){
	while(1){
		switch(lookahead){	
			case OR:
			match(OR);
			parse_LogicalExpr();
			continue;
			break;
		default:
			//report_error(OR);
			return;
		}
	}

}

parse_And(){
	switch(lookahead){
		case NOT:
		case INTCONST:
		case DECCONST:
		case ID:
		case INCR:
		case DECR:
		case '(':
		case TRUE:
		case FALSE:
			parse_Not();
			parse_And1();

			break;
		default: 
			;
			//report_error(AND); FIXME
	}	
}

parse_And1(){
	while(1){
		switch(lookahead){
			case AND:
				match(AND);
				parse_And();
				continue;
				break;
			default:
				//report_error(AND); FIXME
			 	return;	
		}
	}
}

parse_Not(){
	switch(lookahead){
	case NOT:
		match(NOT);
		match('(');
		parse_Not();
		match(')');
		break;
	case INTCONST:
	case DECCONST:
	case ID:
	case INCR:
	case DECR:
	case '(':
	case TRUE:
	case FALSE:
		parse_Comparison();
		break;
	}
}

parse_Comparison(){
	switch(lookahead){
		case INTCONST:
		case DECCONST:
		case ID:
		case INCR:
		case DECR:
		case '(':
			parse_BooleanExpr();
			break;
		case TRUE:
		case FALSE:
			parse_Boolean();
			break;
	}
}

int intCounter = 0;
parse_BooleanExpr(){
	switch(lookahead){
		case INTCONST:
		case DECCONST:
		case ID:
		case INCR:
		case DECR:
		case '(':
			parse_Arith_Unary();
			parse_BooleanExpr2();
			break;
	}
}

parse_BooleanExpr2(){
	switch(lookahead){
		case EQUAL:
		case GE:
		case LE:
		case NE:
		case '>':
		case '<':
			parse_RelOp();
			parse_BooleanExpr();
		default:
			//report_error(lookahead); FIXME
			return;
	}
}

parse_RelOp(){
	switch(lookahead){
		case EQUAL:
			match(EQUAL);
			break;
		case GE:
			match(GE);
			break;
		case LE:
			match(LE);
			break;
		case NE:
			match(NE);
			break;
		case '>':
			match('>');
			break;
		case '<':
			match('<');
			break;
	}	

}


int isInteger(FILE *fileSrc, FILE *fileOutput, char strDIG[], int intCounter){
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
			while(!isspace(ch= fgetc(fileSrc))  && ch!=EOF && ch != ';'){
				if(ispunct(ch)){
					fseek(fileSrc, -1, SEEK_CUR);
					break;
				}else{
						strDIG[intCounter] = ch;
						
						intCounter++;
				}
			
			}
		
			int_ret_val = 0;
		} else if(ch == '.') {
			strDIG[intCounter] = '.';
			intCounter++;
			return isDecimal(fileSrc, fileOutput, strDIG,intCounter);
		} else if (isspace(ch) || ch == EOF || ch == ';' || ispunct(ch)){
	 		if(intDigitCount > 10){
	 			strcpy(currToken, strDIG);
	 			fprintf(fileOutput, "%s\tError: Out of Range\n", strDIG);
	 			return ERROR;
	 		}else{
	 			int_ret_val = 1;
	 		}
			 //if whitespace ,
	    }

	 	fseek(fileSrc, -1, SEEK_CUR);
		if(int_ret_val == 1){
				int_val = atoi(strDIG);
			//final checking para sure na walang error.
				 if(int_val > 999999999){
				 	strcpy(currToken, strDIG);
	  			fprintf(fileOutput,"%s\tERROR \n",strDIG);
	  			return ERROR;
			}
			else{
				strcpy(currToken, strDIG);
	  			fprintf(fileOutput,"%s\tINT \n",strDIG);
	  			return INTCONST;
			}
		}else{
			strcpy(currToken, strDIG);
				fprintf(fileOutput,"%s\tERROR\n",strDIG);
				return ERROR;
		}
}

int isDecimal(FILE *fileSrc, FILE *fOutput, char strDIG[], int intCounter){
		
	char temp;	
	int intIteration=1;

	while(isdigit(temp = fgetc(fileSrc))){
	
		strDIG[intCounter] = temp;
		intCounter++;
		if(intIteration > 5){

			while(!isspace(temp = fgetc(fileSrc)) && temp!=EOF && temp!= ';' && !ispunct(temp)){
					strDIG[intCounter] = temp;
					intCounter++;
			}
			fseek(fileSrc, -1, SEEK_CUR);
			strcpy(currToken, strDIG);
			fprintf(fOutput, "%s\tDOUBLE (Error: Out of Range)\n", strDIG );
			return ERROR;
		}
		

		intIteration++;
	}
	
	
	if(isspace(temp) || temp == EOF || temp == ';' || ispunct(temp)){
			fseek(fileSrc, -1, SEEK_CUR);
			strcpy(currToken, strDIG);
			fprintf(fOutput, "%s\tDOUBLE\n", strDIG);
			return DECCONST;
	}else if(isalpha(temp)){
			strDIG[intCounter] = temp;
			intCounter++;
		while(!isspace(temp = fgetc(fileSrc))  && temp!=EOF && temp!= ';' && !ispunct(temp)){
				strDIG[intCounter] = temp;
				intCounter++;
		}
		fprintf(fOutput, "%s\tDOUBLE (Error: Invalid ID)\n", strDIG);
		strcpy(currToken, strDIG);
		fseek(fileSrc, -1, SEEK_CUR);
		return ERROR;
	}else if(temp == '.'){
		fprintf(fOutput, "%s\tDOUBLE (Error: Invalid ID)\n", strDIG);
		strcpy(currToken, strDIG);
		fseek(fileSrc, -1, SEEK_CUR);
		return ERROR;
	}
					
}	

int isDataType(char tempType[], FILE *fOutput, FILE *fileSrc){	
	int Val = 0;
	int temp;
    switch (tempType[0])
    {      case 'I': switch (tempType[1])
           {    case 'n': switch (tempType[2])
                {    case 't': switch (tempType[3])
				{	case 0:
					{	fputs("Int", fOutput); 
                     		fputs("\tINT",fOutput); 
                     		fprintf(fOutput,"\t%d\n",INT);
                     		Val = INT;
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
                               	fputs("\tCHAR",fOutput); 
                               	fprintf(fOutput,"\t%d\n",CHAR);
                               	Val = CHAR;
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
                                             fputs("\tSTRING",fOutput);
								    fprintf(fOutput,"\t%d\n",STRING); 
                                             Val = STRING;
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
                                          	fputs("\tDOUBLE",fOutput); 
                                          	fprintf(fOutput,"\t%d\n",DOUBLE);
								 	Val = DOUBLE; 
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
                                               		fputs("\tBOOLEAN",fOutput);
											 fprintf(fOutput,"\t%d\n",BOOLEAN); 
											Val = BOOLEAN;
										}
									 } 
                                          
                                     }
                                }     
                          }
                     }
                }
           } 
   		
      	return Val;      
	//default: fputs(tempBuffer,fOutput); fputs(" \tBOOLEAN\n",fOutput);	
	}

 	return Val;      
}
int isKeyword(char tempKey[], FILE *fOutput, FILE *fileSrc){
	int Val = 0;
	switch (tempKey[0]){
		case 'I': switch (tempKey[1]){  
				case 'f': {	
					fputs("If", fOutput);
					fputs("\tIF", fOutput);
					fprintf(fOutput,"\t%d\n",IF);
					Val = IF;
				}							
			} 
	    
	    case 'C': switch (tempKey[1])
	    {   case 'o': switch (tempKey[2])
		    	   {    case 'n': switch (tempKey[3])
		        {    case 's': switch (tempKey[4])
		         	   {    case 't': 
					   		{   fputs("Const", fOutput);
							fputs("\tCONST", fOutput);
							fprintf(fOutput,"\t%d\n",CONST);
							Val = CONST;
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
						fputs("\tSTOP", fOutput);
						fprintf(fOutput,"\t%d\n",STOP);
						Val = STOP;
					}
		     }
		 }
		}
	    
	   	case 'D': switch (tempKey[1])
	        {	case 'i': switch (tempKey[2])
	            {    case 'v': switch(tempKey[3])
				   {	fputs("Div", fOutput);
						fputs("\tDIV", fOutput);
						fprintf(fOutput,"\t%d\n",DIV);
						Val = DIV;
					}
				}
	        
	            case 'o':switch (tempKey[3])
				{	 case 0:
					{	fputs("Do", fOutput);
						fputs("\tDO", fOutput);
						fprintf(fOutput,"\t%d\n",DO);
						Val = DO;
					}
				}		
					
	        } 
	  
		 	  case 'A': switch (tempKey[1])
	        {    case 'n': switch (tempKey[2])
	            {    case 'd': switch(tempKey[3])
				{	case 0:
					{	fputs("And", fOutput);
						fputs("\tAND", fOutput);
						fprintf(fOutput,"\t%d\n",AND);
						Val = AND;
					}
					}
	            }
			} 
	  		
			case 'O': switch (tempKey[1])
	        {    case 'r': switch(tempKey[2])
				{	case 0:
				{	fputs("Or", fOutput);
					fputs("\tOR", fOutput);
					fprintf(fOutput,"\t%d\n",OR);
					Val = OR;
				}
				}
	        } 
	  	
	 	case 'N': switch (tempKey[1])
	        {    case 'o': switch (tempKey[2])
	            {    case 't': 
					{	fputs("Not", fOutput);
						fputs("\tNOT", fOutput);
						fprintf(fOutput,"\t%d\n",NOT);
						Val = NOT;
					}
				}
	            
				case 'e': switch (tempKey[2])
	            {    case 'x': switch (tempKey[3])
	                {    case 't':
						{	fputs("Next", fOutput);
							fputs("\tNEXT", fOutput);
							fprintf(fOutput,"\t%d\n",NEXT);
							Val = NEXT;
						}
	                }
	            }
	        } 
	   
	  	  case 'R': switch (tempKey[1])
	        {    case 'e': switch (tempKey[2])
	            {    case 'a': switch (tempKey[3])
	                {	case 'd': 
						{	fputs("Read", fOutput);
							fputs("\tREAD", fOutput);
							fprintf(fOutput,"\t%d\n",READ);
							Val = READ;
						}
	                }
	    
	            }
	        } 
	    	  
		  case 'V': switch (tempKey[1])
	        {   case 'i': switch (tempKey[2])
	            {   case 'e': switch (tempKey[3])
	                {   case 'w': 
	                    {	fputs("View", fOutput);
					fputs("\tVIEW", fOutput);
					fprintf(fOutput,"\t%d\n",VIEW);
					Val = VIEW;
					}
	                }
	            }
	                    
			case 'o': switch (tempKey[2])
	            {   case 'i': switch (tempKey[3])
	                {    case 'd': 
					{	fputs("Void", fOutput);
						fputs("\tVOID", fOutput);
						fprintf(fOutput,"\t%d\n",VOID);
						Val = VOID;
					}
	                }
	            }
	        } 
	        
		  case 'E': switch (tempKey[1])
	        {   case 'l': switch (tempKey[2])
	            {   case 's': switch (tempKey[3])
	                {   case 'e': 
	                    {	fputs("Else", fOutput);
						fputs("\tELSE", fOutput);
						fprintf(fOutput,"\t%d\n",ELSE);
						Val = ELSE;
					}
	                }
	           
				case 'i': switch (tempKey[3])
	                {    case 'f':
						{	fputs("Elif",fOutput);
							fputs("\tELIF", fOutput);
							fprintf(fOutput,"\t%d\n",ELIF);
							Val = ELIF;
						}
	                }
	            }
	        } 
	        
	        case 'T': switch (tempKey[1])
	        {   case 'r': switch (tempKey[2])
	            {   case 'u': switch (tempKey[3])
	                {   case 'e': 
	                    {	fputs("True", fOutput);
						fputs("\tTRUE", fOutput);
						fprintf(fOutput,"\t%d\n",TRUE);
						Val = TRUE;
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
						fputs("\tWHILE", fOutput);
						fprintf(fOutput,"\t%d\n",WHILE);
						Val = WHILE;
						} 
	                    }
	                }
	            }
	        } 
	   
	  	  case 'F': switch (tempKey[1])
	        {    case 'o': switch (tempKey[2])
	            {    case 'r': 
	                {	fputs("For", fOutput);
					fputs("\tFOR", fOutput);
					fprintf(fOutput,"\t%d\n",FOR);
					Val = FOR;
				}
	            }
	            case 'a': switch (tempKey[2])
	            {	case 'l': switch (tempKey[3])
	            	{ 	case 's': switch (tempKey[4])
	            	  	{	case 'e': 
	           	  		{	fputs("False", fOutput);
							fputs("\tFALSE", fOutput);
							fprintf(fOutput,"\t%d\n",FALSE);
							Val = FALSE;
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
									fputs("\tJUMPTO", fOutput);
									fprintf(fOutput,"\t%d\n",JUMPTO);
									Val = JUMPTO;
								}
							}     
				         }
	                    }
	                }
			 } 
			 
			 case 'M': switch (tempKey[1])
	        {   case 'a': switch (tempKey[2])
	            {   case 'i': switch (tempKey[3])
	                {   case 'n': 
	                    {	fputs("Main", fOutput);
						fputs("\tMAIN", fOutput);
						fprintf(fOutput,"\t%d\n",MAIN);
						
						Val = MAIN;
					}
	                }
	            }
	        } 
	    
	    }


		if(Val == 0){
			 Val = isDataType(tempKey, fOutput, fileSrc);
			 if(Val == 0){
			 	  	fprintf(fOutput,"%s\t ERROR Invalid Identifier",tempKey);
				fprintf(fOutput,"\t%d\n",ERROR);
				return ERROR;
			 }

		}
	 strcpy(currToken, tempKey);
	  return Val;				 
	}



int lexer() {

	char tempBuffer[MAX_SIZE];
	char ch = fgetc(fileSrc);
	int intCounter = 0;
	memset(tempBuffer, 0, sizeof tempBuffer);
	memset(currToken, 0, sizeof currToken);
	if(ch == EOF) return DONE;
	//END
	switch(ch){
		case '/': 
			ch = fgetc(fileSrc);
		if(ch == '/'){
				tempBuffer[intCounter] = '/';
				intCounter++;
				tempBuffer[intCounter] = '/';
				intCounter++;
				while((ch = fgetc(fileSrc)) != '\n' && ch!= EOF) {
					tempBuffer[intCounter] = ch;
					intCounter++;
				}
				fprintf(fOutput, "%s \tCOMMENT", tempBuffer);
				fprintf(fOutput," \t%d\n",COMMENT);
			lexer();
				//return COMMENT;
				
		}else if(ch == '*') {
				char msg[MAX_SIZE] = "COMMENT";
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

				fprintf(fOutput, "%s\tCOMMENT", tempBuffer);
				fprintf(fOutput,"\t%d\n",COMMENT);
				lexer();
				//return COMMENT;
				}else{
			tempBuffer[intCounter] = '/';
			intCounter++;
			fprintf(fOutput, "/  \tOPTR_DIV\n");
			strcpy(currToken, "/");
			fseek(fileSrc, -1, SEEK_CUR);
			return '/';
		}
			break;
		case '+':
			ch = fgetc(fileSrc);
			if(ch == '+'){
				fputs("++", fOutput);
				fputs("\tUNA_INC", fOutput);
				fprintf(fOutput,"\t%d\n",INCR);
				strcpy(currToken, "++");
				return INCR;
			}else{
				fputc('+', fOutput);
				fputs("\tOPTR_ADD", fOutput);
				fprintf(fOutput,"\t43\n");
				strcpy(currToken, "+");
				fseek(fileSrc, -1, SEEK_CUR);
				return '+';
			}
			break;
		case '-':
			ch = fgetc(fileSrc);
			if(ch == '-'){
				fputs("--",fOutput);
				fputs("\tUNA_DEC",fOutput );
				fprintf(fOutput,"\t%d\n",DECR);
				strcpy(currToken, "--");
				return DECR;
			}else{
				fputc('-', fOutput);
				fputs("\tOPTR_SUB", fOutput);
				fprintf(fOutput,"\t45\n");
				strcpy(currToken, "-");
				fseek(fileSrc, -1, SEEK_CUR);
				return '-';
			}
			break;
		case'*':
			fputc('*', fOutput);
			fputs("\tOPTR_MUL", fOutput);
			strcpy(currToken, "*");
			fprintf(fOutput,"\t42\n");
			return '*';	
			break;	
	
		case'%':
			fputc('%',fOutput );
			fputs("\tOPTR_MOD", fOutput);
			strcpy(currToken, "%");
			fprintf(fOutput," \t37\n");
			return '%';
			break;
			
		case '^':
			fputc('^',fOutput );
			fputs("\tOPTR_EXP", fOutput);
			strcpy(currToken, "^");
			fprintf(fOutput,"\t94\n");
			return '^';
			break;
	
		case'=':
			ch = fgetc(fileSrc);
			if(ch == '='){
				fputs("==", fOutput);
				fputs("\tCOND_EQUALS", fOutput);
				strcpy(currToken, "==");
				fprintf(fOutput,"\t%d\n",EQUAL);
				return EQUAL;
			}else{
				fputc('=',fOutput ); 
				fputs("\tEQUAL", fOutput);	
				fseek(fileSrc, -1, SEEK_CUR);
				strcpy(currToken, "=");
				fprintf(fOutput,"\t61\n");
				return '=';
			}
			break;
		
	
		case'>':
			ch = fgetc(fileSrc);
			if(ch =='='){
				fputs(">=",fOutput );
				fputs("\tGREAT_EQUAL", fOutput);
				strcpy(currToken, ">=");
				fprintf(fOutput,"\t%d\n",GE);
				return GE;
			}else{
				fputc('>',fOutput );
				fputs("  \tGREAT_THAN", fOutput);
				fprintf(fOutput,"\t62\n");
				strcpy(currToken, ">");
				fseek(fileSrc, -1, SEEK_CUR);
				return '>';
			}
			break;
	
	
		case'<':
			ch = fgetc(fileSrc);
			if(ch =='='){
				fputs("<=",fOutput );
				fputs("\tLESS_EQUAL", fOutput);
				strcpy(currToken, "<");
				fprintf(fOutput,"\t%d\n",LE);
				return LE;
			}else if(ch =='>'){
				fputs("<>",fOutput );
				fputs("\tNOT_EQUAL", fOutput);
				strcpy(currToken, "<>");
				fprintf(fOutput,"\t%d\n",NE);
				return NE;
			}else{
				fputc('<', fOutput );
				fputs("\tLESS_THAN", fOutput);
				strcpy(currToken, "<");
				fprintf(fOutput,"\t60\n");
				fseek(fileSrc, -1, SEEK_CUR);
				return '<';
			}
			break;
	
	
		case'{':
			fputc('{', fOutput);
			fputs("\tCURL_START", fOutput);
			strcpy(currToken, "{");
			fprintf(fOutput,"\t123\n");
			return '{';
			break;	
	
	
		case'}':
			fputc('}', fOutput);
			fputs("\tCURL_END", fOutput);
			strcpy(currToken, "}");
			fprintf(fOutput,"\t125\n");
			return '}';
			break;	
	
	
		case'(':
			fputc('(', fOutput);
			fputs("\tPAREN_START", fOutput);
			strcpy(currToken, "(");
			fprintf(fOutput,"\t40\n");
			return '(';
			break;
	
	
		case')':
			fputc(')', fOutput);
			fputs("\tPAREN_END", fOutput);
			strcpy(currToken, ")");
			fprintf(fOutput,"\t41\n");
			return ')';
			break;
	
	
		case'[':
			fputc('[', fOutput);
			fputs("\tSQR_START", fOutput);
			strcpy(currToken, "[");
			fprintf(fOutput,"\t91\n");
			return '[';
			break;
			
	
		case']':
			fputc(']', fOutput);
			fputs("\tSQR_END", fOutput);
			strcpy(currToken, "]");
			fprintf(fOutput,"\t93\n");
			return ']';
			break;
	
	
		case';':
			fputc(';', fOutput);
			fputs("\tTERMINATOR", fOutput);
			strcpy(currToken, ";");
			fprintf(fOutput,"\t59\n");
			return ';';
			break;
	
	
		case',':
			fputc(',', fOutput);
			fputs("\tSEPARATOR", fOutput);
			strcpy(currToken, ",");
			fprintf(fOutput,"\t44\n");
			return ',';
			break;
		
		case '\'':
		tempBuffer[0] = ch;
				intCounter = 1;
				
				while((ch=fgetc(fileSrc))!= '\'' && ch!='\n'){
					tempBuffer[intCounter] = ch;
					intCounter++;
				}
				if(ch == '\''){
				tempBuffer[intCounter] = ch;
				fprintf(fOutput, "%s\tCHARACTER\n", tempBuffer);
				strcpy(currToken, tempBuffer);
				return CHARLIT;
				}else{
				fprintf(fOutput, "%s\tCHARACTER (Missing Closing Quote)\n", tempBuffer);
				strcpy(currToken, tempBuffer);
				return CHARLIT;
			}
			break;
		
		case '\"':
			tempBuffer[0] = ch;
			intCounter = 1;
			
			while((ch=fgetc(fileSrc)) != '"' && ch!= '\n' && ch!=EOF){
				tempBuffer[intCounter] = ch;
				intCounter++;
			}
			
			if(ch == '"'){
				tempBuffer[intCounter] = ch;
				fprintf(fOutput, "%s\tSTRING\n", tempBuffer);
				strcpy(currToken, tempBuffer);
				return STRLIT;
			}else{
				fprintf(fOutput, "%s\tSTRING (Missing Closing Quote)\n", tempBuffer);
				strcpy(currToken, tempBuffer);
				return STRLIT;
			}
			
				break;
		  case '0':
		  case '1':
		  case '2':
		  case '3':
		  case '4':
		  case '5':
		  case '6':
		  case '7':
		  case '8':
		  case '9':
		  		tempBuffer[intCounter] = ch;
		  		intCounter++;
		  		return isInteger(fileSrc, fOutput, tempBuffer, intCounter); 
		  		break;
		  case '.':
		  		tempBuffer[intCounter] = ch;
		  		intCounter++;
		  		return isDecimal(fileSrc, fOutput, tempBuffer, intCounter);
		  		break;
		  case EOF:
		  		return DONE;
		  		break;
		  case ' ':
		  		lexer();
		  		break;
		  case '\n':
		  		lexer();
		  		break;
		  default:
		  	if(isalpha(ch)){
		  		
		  		tempBuffer[intCounter] = ch;
		  		intCounter++;
		  		while(isalnum(ch = fgetc(fileSrc)) && intCounter != 64){
		  			tempBuffer[intCounter] = ch;
		  			intCounter++;
		  		}
		  		if(ispunct(ch)){
		  			fseek(fileSrc, -1, SEEK_CUR);
		  		}
		  		else if(!isspace(ch) && ch!=EOF && (ch!=';')){
		  				
		  				while(!isspace(ch) && ch!=EOF && (ch!=';')){
		  						tempBuffer[intCounter] = ch;
		  						intCounter++;
		  				}
		  				fputs(tempBuffer, fOutput); 
      					fputs(" \tERROR Invalid Character",fOutput); 
      					strcpy(currToken, tempBuffer);
						fprintf(fOutput,"\t%d\n",ERROR);
						fseek(fileSrc, -1, SEEK_CUR);
						return ERROR;		
		  		}else if(intCounter >= 64){
		  				
		  				while(!isspace(ch) && (ch!=EOF) && (ch!=';')){
		  						tempBuffer[intCounter] = ch;
		  						intCounter++;
		  				}
		  				fseek(fileSrc, -1, SEEK_CUR);
		  				fputs(tempBuffer, fOutput); 
      					fputs(" \tERROR Too long",fOutput); 
      					strcpy(currToken, tempBuffer);
						fprintf(fOutput,"\t%d\n",ERROR);
						return ERROR;
		  		}
		  		
		  		
		  		if(isupper(tempBuffer[0])){	
				  	return isKeyword(tempBuffer, fOutput, fileSrc);
				}
				
				if(islower(tempBuffer[0])){	
				
					fputs(tempBuffer, fOutput); 
       				fputs(" \tIDENTIFIER",fOutput); 
       				strcpy(currToken, tempBuffer);
       				fprintf(fOutput," \t%d\n",ID);
		 			return ID; 
				}
				
		  	}else if(isspace(ch)){
		  		lexer();
		  	}else{
		  			fputs("\tNULL_TOKEN",fOutput);
					fprintf(fOutput,"\t%d\n",ERROR);
					return ERROR;
		  	}
		  	break;
	}  
		

	
}


void checkFile(FILE *filePtr)
{	
	if(filePtr == NULL) {
		printf("File does not exist!");
		exit(0);
	}
	fseek(filePtr, 0, SEEK_END);
	if (ftell(filePtr) == 0) {		
		printf("File is empty.\n");
	    exit(1);
	}
	else rewind(filePtr);
	
}

//checks if source code contains tokens
void checkSrc(FILE *filePtr) {	
	char ch;
	while(( ch = fgetc(filePtr)) != EOF) {	
		if(ch > ' ') return;
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
	if((strlen(strFileExt)) != (strlen(ret)))
		return 0;

	for(intCounter=0; intCounter < strlen(strFileExt); intCounter++){	
		if(strFileExt[intCounter] == ret[intCounter]){	
			intValid = 1;
		}
	}
	return intValid;

}


int main(void){	
	fflush(stdout);
	char file[64] = "example.pype";
	//printf("Enter filename: ");
	//scanf("%s", file);
	if(isValidFileExtension(file)){	
		fileSrc = fopen(file, "r");
		fOutput =fopen("symbol_table.txt","w");
		checkFile(fileSrc);
		checkSrc(fileSrc);
		rewind(fileSrc);
		fParser = fopen("parse.txt", "w");
		lookahead = lexer();
		parse_Pype();
		fclose(fileSrc);
		fclose(fOutput);
	}else{	
		printf("FILE NOT FOUND");
	}
	getch();
	return 0;
}

