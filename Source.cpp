#include <iostream>
#include <Stdio.h>
#include <string>
#include <queue>
#include <Windows.h>
using namespace std;

struct Token {
	string Word;
	int Length;
	int Column;
	int Line;
	string Type;
			 };//Token

HANDLE H;
queue<Token>LookAhead;
Token Temp;

int isKeyword(string buffer) {
		if ((buffer != "if") || (buffer != "else")|| (buffer != "while") || (buffer != "do")
			|| (buffer!= "break") || (buffer!= "continue")|| (buffer !="double") || (buffer != "float")
			|| (buffer!= "return") || (buffer!= "char")|| (buffer!= "case") || (buffer!= "int")
			|| (buffer!= "sizeof") || (buffer!= "long")|| (buffer!= "short") || (buffer!= "typedef")
			|| (buffer!= "switch") || (buffer!= "unsigned")|| (buffer!= "void") || (buffer!= "static")
			|| (buffer!= "struct") || (buffer!= "goto")|| (buffer!= "const") || (buffer!= "class")
			|| (buffer!= "new") || (buffer!= "delete")|| (buffer!= "and") || (buffer!= "bool")
			|| (buffer!= "default") || (buffer!= "false")|| (buffer!= "true") || (buffer!= "for")
			|| (buffer!= "friend") || (buffer!= "iLineCounterine")|| (buffer!= "regester") || (buffer!= "namespace")
			|| (buffer!= "or") || (buffer!= "public")|| (buffer!= "private") || (buffer!= "protected")
			|| (buffer!= "signed") || (buffer!= "template")|| (buffer!= "this") || (buffer!= "try")
			|| (buffer!= "catch") || (buffer!= "using")|| (buffer!= "virtual") || (buffer!= "void")
			|| (buffer!= "xor") || (buffer!="include"))
			return 0;
		return 1;
								}//isKeyword

int Operator(string A) {
	if (A == "!" || A == "<" || A == ">" || A == "==" || A == "<=" || A == ">=" || A == "!=" || A == "||" || A == "&&")
		return 1;
	return 0;
					   }//Operator

class Scanner {
private:

	FILE *Source, *Destination;
	char ch, c;
	char buffer[50]; char *ope;
	int LineCounter , Length, ColumnCounter;

public:

	Scanner() : Source(NULL),Destination(NULL),ope(NULL) {

		char Temp[500];
		ch = c = NULL;
		ope = new char[2];
		Length = ColumnCounter= 0;
		LineCounter = 1;
		cout << " Please Enter Source Code Path : ";
		cin.getline(Temp, 500);
		Source = fopen(Temp, "r");
		if (Source == NULL) {
			H = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleTextAttribute(H, FOREGROUND_RED | FOREGROUND_INTENSITY);
			cout << " Can't Open The File !" << endl;
			system("pause");
			exit(-1);
							}//if statment
		
		else {
			H = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleTextAttribute(H, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
			cout << " File Opened Successfuly !" << endl;
			 }//else
		
		cout << endl << endl;

		H = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(H, 7);
		cout << " Please Enter Destinationination File Path : ";
		cin.getline(Temp, 500);
		Destination = fopen(Temp, "w");

		if (Destination == NULL) {
			H = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleTextAttribute(H, FOREGROUND_RED | FOREGROUND_INTENSITY);
			cout << " Can't Create The File !" << endl;
			system("pause");
			//exit(-1);
								 }//if statment
	
		else {
			H = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleTextAttribute(H, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
			cout << " Destination File Created Successfuly ! " << endl;
			 }//else

		cout << endl << endl;
		

															}//Scanner

	void tokenLex() {
		fprintf(Destination,"%s                %s      %s      %s      %s \n" , "TYPE", "SPELLING" , "LENGTH", "LINE", "COLUMN");
		while (!feof(Source)) {
			ch = getc(Source);
			Length = 0;
			Buffer(ch);
			Operator();
			if (ch == '\n') { 
				LineCounter++;
				ColumnCounter = 0;
							}//if statment

							 }//while loop
		fclose(Source);
					}//tokenlex

	void Buffer(char ch) {

		while (isalnum(ch) || ch == '.') {
			buffer[Length++] = ch;
			ch = getc(Source);
										 }//while loop
		buffer[Length] = '\0';

		if (isKeyword(buffer) == 1) {
			ColumnCounter++;
			fprintf(Destination, "%s          %s          %d          %d          %d\n" , "Keyword   ", buffer,Length, LineCounter, ColumnCounter);
			Temp.Column = ColumnCounter;
			Temp.Length = Length;
			Temp.Word = buffer;
			Temp.Line = LineCounter;
			Temp.Type = "Keyword";
			LookAhead.push(Temp);
			ungetc(ch, Source);
									}//if statment
		else if ('a' <= buffer[0] && 'z' >= buffer[0] || 'A' <= buffer[0] && 'Z' >= buffer[0]) {
			ColumnCounter++;
			fprintf(Destination, "%s          %s          %d          %d          %d\n" , "Identifier", buffer,Length, LineCounter, ColumnCounter);
			Temp.Column = ColumnCounter;
			Temp.Length = Length;
			Temp.Word = buffer;
			Temp.Line = LineCounter;
			Temp.Type = "Identifier";
			LookAhead.push(Temp);
			ungetc(ch, Source);
																							   }//else if
		else  if ('0' <= buffer[0] && buffer[0] <= '9') {
			ColumnCounter++;
			Number(buffer, Length, ColumnCounter, LineCounter);
			ungetc(ch, Source);
														}//else if

						 }//Buffer

	void Operator() {
		if (punc_arr(&ch) == 1 || other_op(&ch) == 1 || basic_op(ch) == 1) {
			ope[0] = ch;
			char c = ch;
			ch = getc(Source);
			ope[1] = NULL;
			if (punc_arr(&ch) == 1 || other_op(&ch) == 1) {
				ope[1] = ch;
				ope[2] = NULL;
				if (other_op(ope) == 1 || punc_arr(ope) == 1) {
					if (other_op(ope) == 1)   {
						ColumnCounter++;
						fprintf(Destination, "%s          %s          %d          %d          %d\n" , "Other Operation", ope, 2, LineCounter, ColumnCounter);
						Temp.Column = ColumnCounter;
						Temp.Length = Length;
						Temp.Word = ope;
						Temp.Line = LineCounter;
						Temp.Type = "Other Operation";
						LookAhead.push(Temp);
											  }//if statment
					if (punc_arr(ope) == 1) {
						ColumnCounter++;
						fprintf(Destination, "%s          %s          %d          %d          %d\n" , "Punctuation    ", ope, 2, LineCounter, ColumnCounter);
						Temp.Column = ColumnCounter;
						Temp.Length = Length;
						Temp.Word = ope;
						Temp.Line = LineCounter;
						Temp.Type = "Punctuation";
						LookAhead.push(Temp);
											}//if statment
				
															  }//if statment
				else {
					if (other_op1(c) == 1) {
						ColumnCounter++;
						fprintf(Destination, "%s          %c          %d          %d          %d\n" , "Other Operation", c, 1, LineCounter, ColumnCounter);
						Temp.Column = ColumnCounter;
						Temp.Length = Length;
						Temp.Word = c;
						Temp.Line = LineCounter;
						Temp.Type = "Other Operation";
						LookAhead.push(Temp);
										   }//if statment

					else
						if (punc_arr1(c) == 1) {
							ColumnCounter++;
							fprintf(Destination, "%s          %c          %d          %d          %d\n" , "Punctuation    ", c, 1, LineCounter, ColumnCounter);
							Temp.Column = ColumnCounter;
							Temp.Length = Length;
							Temp.Word = c;
							Temp.Line = LineCounter;
							Temp.Type = "Punctuation";
							LookAhead.push(Temp);
											   }//if statment


					if (other_op(&ope[1]) == 1) {
						ColumnCounter++;
						fprintf(Destination, "%s          %c          %d          %d          %d\n" , "Other Operation", ope[1], 1, LineCounter, ColumnCounter);
						Temp.Column = ColumnCounter;
						Temp.Length = Length;
						Temp.Word = ope[1];
						Temp.Line = LineCounter;
						Temp.Type = "Other Operation";
						LookAhead.push(Temp);
												}//if statment
					else {
						if (punc_arr(&ope[1]) == 1) {
							ColumnCounter++;
							fprintf(Destination, "%s          %c          %d          %d          %d\n" , "Punctuation    ", ope[1], 2, LineCounter, ColumnCounter);
							Temp.Column = ColumnCounter;
							Temp.Length = Length;
							Temp.Word = ope[1];
							Temp.Line = LineCounter;
							Temp.Type = "Punctuation";
							LookAhead.push(Temp);
												    }//if statment
						 }//else
						}//else


														  }//else
			else {
				if (other_op1(c) == 1) {
					ColumnCounter++;
					fprintf(Destination, "%s          %c          %d          %d          %d\n" , "Other Operation", c, 1, LineCounter, ColumnCounter);
					Temp.Column = ColumnCounter;
					Temp.Length = Length;
					Temp.Word = c;
					Temp.Line = LineCounter;
					Temp.Type = "Other Operation";
					LookAhead.push(Temp);
									   }//if statment

				else if (basic_op(c) == 1) {
					ColumnCounter++;
					fprintf(Destination, "%s          %c          %d          %d          %d\n" , "Basic Operation", c, 1, LineCounter, ColumnCounter);
					Temp.Column = ColumnCounter;
					Temp.Length = Length;
					Temp.Word = c;
					Temp.Line = LineCounter;
					Temp.Type = "Basic Operation";
					LookAhead.push(Temp);
									  }//if statment

				else if (punc_arr1(c) == 1) {
					ColumnCounter++;
					fprintf(Destination, "%s          %c          %d          %d          %d\n" , "Punctuation    ", c, 1, LineCounter, ColumnCounter);
					Temp.Column = ColumnCounter;
					Temp.Length = Length;
					Temp.Word = c;
					Temp.Line = LineCounter;
					Temp.Type = "Basic Operation";
					LookAhead.push(Temp);
											}//else if

				if (ch != '\n')
				ungetc(ch, Source);
				 }//else
																			}//if statment
					}//Operator

	int isKeyword(char buffer[]) {
		if (!strcmp(buffer, "if") || !strcmp(buffer, "else")|| !strcmp(buffer, "while") || !strcmp(buffer, "do")
			|| !strcmp(buffer, "break") || !strcmp(buffer, "continue")|| !strcmp(buffer, "double") || !strcmp(buffer, "float")
			|| !strcmp(buffer, "return") || !strcmp(buffer, "char")|| !strcmp(buffer, "case") || !strcmp(buffer, "int")
			|| !strcmp(buffer, "sizeof") || !strcmp(buffer, "long")|| !strcmp(buffer, "short") || !strcmp(buffer, "typedef")
			|| !strcmp(buffer, "switch") || !strcmp(buffer, "unsigned")|| !strcmp(buffer, "void") || !strcmp(buffer, "static")
			|| !strcmp(buffer, "struct") || !strcmp(buffer, "goto")|| !strcmp(buffer, "const") || !strcmp(buffer, "class")
			|| !strcmp(buffer, "new") || !strcmp(buffer, "delete")|| !strcmp(buffer, "and") || !strcmp(buffer, "bool")
			|| !strcmp(buffer, "default") || !strcmp(buffer, "false")|| !strcmp(buffer, "true") || !strcmp(buffer, "for")
			|| !strcmp(buffer, "friend") || !strcmp(buffer, "iLineCounterine")|| !strcmp(buffer, "regester") || !strcmp(buffer, "namespace")
			|| !strcmp(buffer, "or") || !strcmp(buffer, "public")|| !strcmp(buffer, "private") || !strcmp(buffer, "protected")
			|| !strcmp(buffer, "signed") || !strcmp(buffer, "template")|| !strcmp(buffer, "this") || !strcmp(buffer, "try")
			|| !strcmp(buffer, "catch") || !strcmp(buffer, "using")|| !strcmp(buffer, "virtual") || !strcmp(buffer, "void")
			|| !strcmp(buffer, "xor") || !strcmp(buffer,"include"))
			return 1;
		return 0;
								}//isKeyword

	void Number(char buffer[], int Length, int ColumnCounter, int LineCounter) {
		int  in = 0; int flo = 0;
		int i = 0;
		for (; i < Length; i++) {
			if ('0' <= buffer[i] && buffer[i] <= '9') { in++; }
			else
				if (buffer[i] == '.') {
					in--;
					fprintf(Destination, "%s          %s          %d          %d          %d\n" , "Float Number   ", buffer, Length, LineCounter, ColumnCounter);
					Temp.Column = ColumnCounter;
					Temp.Length = Length;
					Temp.Word = buffer;
					Temp.Line = LineCounter;
					Temp.Type = "Number";
					LookAhead.push(Temp);
									  }//if statment
			if (buffer[i] != '.'&&'a' <= buffer[i] && 'z' >= buffer[i] || 'A' <= buffer[i] && 'Z' >= buffer[i]) {
				fprintf(Destination, "%s          %s          %d          %d          %d\n" , "Invalid Number ", buffer, Length, LineCounter, ColumnCounter);
				Temp.Column = ColumnCounter;
				Temp.Length = Length;
				Temp.Word = buffer;
				Temp.Line = LineCounter;
				Temp.Type = "Number";
				LookAhead.push(Temp);
																												}//if statment				
						   }//for loop	


		if (i == in) {
			fprintf(Destination, "%s          %s          %d          %d          %d\n", "Integer Number ", buffer, Length, LineCounter, ColumnCounter);
			Temp.Column = ColumnCounter;
			Temp.Length = Length;
			Temp.Word = buffer;
			Temp.Line = LineCounter;
			Temp.Type = "Number";
			LookAhead.push(Temp);
					 }//if statment
													   }//Number

	bool basic_op(char ch) {
		if (ch == '+' || ch == '-' || ch == '*') {
			char temp = getc(Source);
			if (ch == '-' && temp == '-') {
				Temp.Word = "--";
				Temp.Column = ColumnCounter;
				Temp.Length = Length;
				Temp.Line = LineCounter;
				Temp.Type = "Punctuation";
				LookAhead.push(Temp);
				return 0;
										  }//if statment

			if (ch == '+' && temp == '+') {
				Temp.Word = "++";
				Temp.Column = ColumnCounter;
				Temp.Length = Length;
				Temp.Line = LineCounter;
				Temp.Type = "Punctuation";
				LookAhead.push(Temp);
				return 0;
										  }//if statment

			ungetc(temp, Source);
			return 1;
												 }//if statment
			
			char temp;
			if (ch == '/') {
				temp = getc(Source);
				if (temp == '/') {
				for (;;) {
				temp = getc(Source);
				if (temp == '\n') {
					LineCounter++;
					ColumnCounter = 0;
					break;
								}//if statemnt
						  }//for loop*/
								  }//if statment

				else if (temp == '*') {
					for (;;) {
					temp = getc(Source);
					if (temp == '\n') {
						LineCounter++;
						ColumnCounter = 0;
									  }//if statment
					if (temp == '*' && getc(Source) == '/') {
						break;
															}//if statment
							 }//for loop
									   }//else if
				else if ((temp != '/' || temp != '*')) {
					ungetc(temp, Source);
					return 1;
													   }//else if

				else ungetc(ch, Source);
				
				
							  }//if statment

			else if (ch == '\"') {
				fprintf(Destination, "%s          %c          %d          %d          %d\n" , "Punctuation    ", ch,1, LineCounter, ColumnCounter);
				Temp.Column = ColumnCounter;
				Temp.Length = Length;
				Temp.Word = ch;
				Temp.Line = LineCounter;
				Temp.Type = "Punctuation";
				LookAhead.push(Temp);
				int i = 0;
				char BUFFER[500];
				temp = getc(Source);
				for (; temp != '\"'; i++) {
					BUFFER[i] = temp;
					ColumnCounter++;
					temp = getc(Source);
										 }//for loop
				BUFFER[i] = NULL;
				Temp.Column = ColumnCounter;
				Temp.Length = strlen(BUFFER);
				Temp.Word = buffer;
				Temp.Line = LineCounter;
				Temp.Type = "string";
				LookAhead.push(Temp);
				fprintf(Destination, "%s          %s          %d          %d          %d\n" , "String         ", BUFFER, strlen(BUFFER), LineCounter, ColumnCounter);
				fprintf(Destination, "%s          %c          %d          %d          %d\n" , "Punctuation    ", '"', 1, LineCounter, ColumnCounter);
							}
			 
			

		return 0;
						   }//basic_op

	bool punc_arr(string  c) {

		if (c == "\'" || c == "\\" || c == "<" || c == ">" || c == "<<" || c == ">>" ||
			c == "," || c == "#" || c == ";" || c == ":" || c == "(" || c == ")"
			|| c == "{" || c == "}" || c == "@" || c == "~" || c == "&" || c == "|" || c == "+=" 
			|| c == "-=" || c == "/=" || c == "*=" || c == "++" || c == "--"
			/*|| c == "\""*/) 
			return 1;

															 
		return 0;
							  }//punc_arr

	bool other_op(string ch) {
		if (ch == "<" || ch == ">" || ch == "=" || ch == "==" || ch == "<=" || ch == ">=" || ch == "!=" || ch == "!" || ch == "++" || ch == "--")
			return 1;
		return 0;
							 }//other_op

	bool punc_arr1(char  c) {

		if (c == '<' || c == '>' || c == '<<' || c == '>>' || '\\' || '|' || '&'
			|| c == ',' || c == '#' || c == ';' || c == ':' || c == '(' || c == ')' || "\'"
			|| c == '{' || c == '}' || c == '@' || c == '~' /*|| c == '\"' || c == '"'*/)
			return 1;

		return 0;
							}//punc_arr1

	bool other_op1(char ch) {
		if (ch == '<' || ch == '>' || ch == '=' || ch == '==' || ch == '<=' || ch == '>=' || ch == '!=' || ch == '!') 
			return 1;																								  
		return 0;
							}//other_op1

			  };//Lex

class Parser {

public : 

	Token LA;
	int Error;
	int LabelNumber;
	FILE * STDERROR, *STDOUT;
	char Temp[500];


	int Lex() {
		if (LookAhead.empty() || LookAhead.size() == 1)
			return -1;

		LookAhead.pop();
		LA = LookAhead.front();
		return 1;
			   }//Lex

	Parser() : STDERROR(NULL),STDOUT(NULL) {
		LabelNumber = 0;
		Error = 0;
		LA = LookAhead.front();

		H = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(H, 7);
		cout << " Please Enter STDERROR File Path : ";
		cin.getline(Temp, 500);
		STDERROR = fopen(Temp, "w");

		if (STDERROR == NULL) {
			H = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleTextAttribute(H, FOREGROUND_RED | FOREGROUND_INTENSITY);
			cout << " Can't Create The File !" << endl;
			system("pause");
			//exit(-1);
							   }//if statment
	
		else {
			H = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleTextAttribute(H, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
			cout << " STDERROR File Created Successfuly ! " << endl;
			 }//else

		cout << endl << endl;


		H = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(H, 7);
		cout << " Please Enter STDOUT File Path : ";
		cin.getline(Temp, 500);
		STDOUT = fopen(Temp, "w");

		if (STDOUT == NULL) {
			H = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleTextAttribute(H, FOREGROUND_RED | FOREGROUND_INTENSITY);
			cout << " Can't Create The File !" << endl;
			system("pause");
			//exit(-1);
							   }//if statment
	
		else {
			H = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleTextAttribute(H, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
			cout << " STDOUT File Created Successfuly ! " << endl;
			 }//else
		
		cout << endl << endl;

		MainFunction();
										   }//Parser

	void LabelUpdater() {
		fprintf(STDOUT, "%s%d\n", " Label ", ++LabelNumber);
		cout << " Label " << LabelNumber << endl;
					    }//LabelUpdater

	void MainFunction() {

			if (LA.Word == "main") {
				Lex();
				if (LA.Word == "(") {
					Lex();
					if (LA.Word == ")") {
						Lex();

						CompoundStatmentFunction();

						if (Lex() != -1) {
							//fprintf(STDERROR, "%s\n", " Not Valid ");
							cout << " Not Valid" << endl;
							//Sleep(3000);
							//exit(-1);
										 }//if statment
										  }//if statment
					else {
						H = GetStdHandle(STD_OUTPUT_HANDLE);
						SetConsoleTextAttribute(H, FOREGROUND_RED | FOREGROUND_INTENSITY);
						fprintf(STDERROR, "%s%d%s\n", " In Line Number : ",LA.Line," , Missing )");
						//fprintf(STDERROR, "%s\n", " Not Valid ");
						cout << " In Line Number : " << LA.Line << " , Missing )" << endl; 
						cout << " Not Valid" << endl; 
						//Sleep(3000);
						//exit(-1); 
						 }//else 
									 }//if statment
				else {
					H = GetStdHandle(STD_OUTPUT_HANDLE);
					SetConsoleTextAttribute(H, FOREGROUND_RED | FOREGROUND_INTENSITY);
					fprintf(STDERROR, "%s%d%s\n", " In Line Number : ", LA.Line, " , Missing (");
					//fprintf(STDERROR, "%s\n", " Not Valid ");
					cout << " In Line Number : " << LA.Line << " , Missing (" << endl;
					cout << " Not Valid" << endl;
					//Sleep(3000);
					//exit(-1);
					 }//else
									}//if statment

			else {
				H = GetStdHandle(STD_OUTPUT_HANDLE);
				SetConsoleTextAttribute(H, FOREGROUND_RED | FOREGROUND_INTENSITY);
				fprintf(STDERROR, "%s%d%s\n", " In Line Number : ", LA.Line, " , Missing main Function");
				//fprintf(STDERROR, "%s\n", " Not Valid ");
				cout << " In Line Number :  " << LA.Line << " , Missing main Function" << endl;
				cout << " Not Valid" << endl;
				//Sleep(3000);
				//exit(-1);
				 }//if statment

						}//MainFunction

	void CompoundStatmentFunction() {
		if (LA.Word == "{") {
			Lex();
			MoreStatmentFunction();
			if (LA.Word == "}")
				Lex();
			
			else {
				H = GetStdHandle(STD_OUTPUT_HANDLE);
				SetConsoleTextAttribute(H, FOREGROUND_RED | FOREGROUND_INTENSITY);
				fprintf(STDERROR, "%s%d%s\n", " In Line Number : ", LA.Line, " , Missing }");
				//fprintf(STDERROR, "%s\n", " Not Valid ");
				cout << " In Line Number : " << LA.Line << " , Missing }" << endl;
				cout << " Not Valid" << endl;
				//Sleep(3000);
				//exit(-1);
				 }//else
							 }//if statment
		else { 
			H = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleTextAttribute(H, FOREGROUND_RED | FOREGROUND_INTENSITY);
			fprintf(STDERROR, "%s%d%s\n", " In Line Number : ", LA.Line, " , Missing {");
			//fprintf(STDERROR, "%s\n", " Not Valid ");
			cout << " In Line Number : " << LA.Line << " , Missing {"  << endl;
			cout << " Not Valid" << endl;
			//Sleep(3000);
			//exit(-1);
			 }//else
									 }//CompoundStatmentFunction

	void ExpresionFunction() {
		string TempOfID;
		int Flag = -1;
		if (isKeyword(LA.Word) || LA.Type == "Identifier" || LA.Type == "Number") {
			if (isKeyword(LA.Word))
				Lex();

			if (LA.Type == "Identifier") {
				Flag = 0;
				TempOfID = LA.Word;
										 }//if statment

			else {
				fprintf(STDOUT, "%s%s", " Push ", LA.Word.c_str());
				cout << " Push " << LA.Word << endl;
				 }//else

			if (LA.Type == "Identifier" || LA.Type == "Number") {
				TempOfID = LA.Word;
				Lex();
				if (Flag == 0 && (LA.Word != "="&&LA.Word != "++"&&LA.Word != "+="&&LA.Word != "*="&&LA.Word != "-="&&LA.Word != "--"&&LA.Word != "/=")) {
					fprintf(STDOUT, "%s%s\n", " Rvalue ", TempOfID.c_str());
					cout << " Rvalue " << TempOfID << endl;
																																						 }//if statment
				else if (Flag == 0) {
					fprintf(STDOUT, "%s%s\n", " Lvalue ", TempOfID.c_str());
					cout << " Lvalue " << TempOfID << endl;
									}//else if

				 if (LA.Word == "+") {
					Lex();
					ExpresionFunction();
					fprintf(STDOUT, "\n%s\n", "  +  ");
					cout << " + " << endl;
									}//if statment

				else if (LA.Word == "-") {
						Lex();
						ExpresionFunction();
						fprintf(STDOUT, "\n%s\n", "  -  ");
						cout << " - " << endl;
										 }//else if

					else if (LA.Word == "*") {
							Lex();
							ExpresionFunction();
							fprintf(STDOUT, "\n%s\n", "  *  ");
							cout << " * " << endl;
											 }//else if

						else if (LA.Word == "/") {
								Lex();
								ExpresionFunction();
								fprintf(STDOUT, "\n%s\n", "  /  ");
								cout << " / " << endl;
												 }//else if

							else if (LA.Word == "%") {
									Lex();
									ExpresionFunction();
									fprintf(STDOUT, "\n%s\n", "  %  ");
									cout << " % " << endl;
													 }//else if

								else {
							
									if (LA.Word == "=") {
										Lex();
										ExpresionFunction();
										fprintf(STDOUT, "\n%s\n", "  :=  ");
										cout << " := " << endl;
														}//if statment

									else if (LA.Word == "++") {
										Lex();
										fprintf(STDOUT, "%s%s\n", " Rvalue ", TempOfID.c_str());
										fprintf(STDOUT, "%s\n%s\n%s\n", " Push 1 ", "  +  " , "  :=  ");
										cout << " Rvalue " << TempOfID << endl;
										cout << " Push 1 " << endl << " + " << endl << " := " << endl;
															  }//else if
									else if (LA.Word == "--") {
										Lex();
										fprintf(STDOUT, "%s%s\n", " Rvalue ", TempOfID.c_str());
										fprintf(STDOUT, "%s\n%s\n%s\n", " Push 1 ", "  -  ", "  :=  ");
										cout << " Rvalue " << TempOfID << endl;
										cout << " Push 1 " << endl << " - " << endl << " := " << endl;
															  }

											else if (LA.Word == "*=") {
													fprintf(STDOUT, "%s%s\n", " Rvalue ", TempOfID.c_str());
													cout << " Rvalue " << TempOfID << endl;
													Lex();
													ExpresionFunction();
													fprintf(STDOUT, "%s\n%s\n",  "  *  ", "  :=  ");
													cout << " * " << endl << " := " << endl;
																	  }//else if

												else if (LA.Word == "/=") {
														fprintf(STDOUT, "%s%s\n", " Rvalue ", TempOfID.c_str());
														cout << " Rvalue " << TempOfID << endl;
														Lex();
														ExpresionFunction();
														fprintf(STDOUT, "%s\n%s\n", "  /  ", "  :=  ");
														cout << " / " << endl << " := " << endl;
																		  }//else if

													else if (LA.Word == "+=") {
															fprintf(STDOUT, "%s%s\n", " Rvalue ", TempOfID.c_str());
															cout << " Rvalue " << TempOfID << endl;
															Lex();
															ExpresionFunction();
															fprintf(STDOUT, "%s\n%s\n", "  +  ", "  :=  ");
															cout <<  " + " << " := " << endl;
																				 }//else if
														
													else if (LA.Word == "-=") {
																fprintf(STDOUT, "%s%s\n", " Rvalue ", TempOfID.c_str());
																cout << " Rvalue " << TempOfID << endl;
																Lex();
																ExpresionFunction();
																fprintf(STDOUT, "%s\n%s\n", "  -  ", "  :=  ");
																cout << endl << " - " << endl << " := " << endl;
																			  }//else if
									  }//else
				return;
																 }//if statment

			else {
				H = GetStdHandle(STD_OUTPUT_HANDLE);
				SetConsoleTextAttribute(H, FOREGROUND_RED | FOREGROUND_INTENSITY);
				fprintf(STDERROR, "%s%d%s\n", " In Line Number : ", LA.Line, " , Missing  Identifier");
				//fprintf(STDERROR, "%s\n", " Not Valid ");
				cout << " In Line Number : " << LA.Line << " , Missing  Identifier" << endl;
				cout << " Not Valid" << endl;
				//Sleep(3000);
				//exit(-1);
				 }//else
			/**/
			if (LA.Word == "+") {
				Lex();
				ExpresionFunction();
								}//if statment

			else if (LA.Word == "Identifier") {
				Lex();
				ExpresionFunction();
											  }//else if

			else if (LA.Word == "-") {
					Lex();
					ExpresionFunction();
									 }//else if

				else if (LA.Word == "*") {
						Lex();
						ExpresionFunction();
										 }//else if

					else if (LA.Word == "/") {
							Lex();
							ExpresionFunction();
											 }//else if

						else if (LA.Word == "%") {
								Lex();
								ExpresionFunction();
												 }//else if
			return;
																				  }//if statment

		if (LA.Word == "(") {
			Lex();
			ExpresionFunction();

			if (LA.Word == ")")
				Lex();

			else {
				H = GetStdHandle(STD_OUTPUT_HANDLE);
				SetConsoleTextAttribute(H, FOREGROUND_RED | FOREGROUND_INTENSITY);
				fprintf(STDERROR, "%s%d%s\n", " In Line Number : ", LA.Line, " , Missing  )");
				//fprintf(STDERROR, "%s\n", " Not Valid ");
				cout << " In Line Number :  " << LA.Line << ", Missing ) "<< endl;
				cout << " Not Valid" << endl;
				//Sleep(3000);
				//exit(-1);
				 }//else

			if (LA.Word == "+") {
				Lex();
				ExpresionFunction();
								}//if statment

			else if (LA.Word == "-") {
					Lex();
					ExpresionFunction();
									 }//else if

				else if (LA.Word == "*") {
						Lex();
						ExpresionFunction();
										 }//else if

					else if (LA.Word == "/") {
							Lex();
							ExpresionFunction();
									 		 }//else if

						else if (LA.Word == "%") {
								Lex();
								ExpresionFunction();
												 }//else if
			return;
							}//if statment

							 }//CompoundStatmentFunction

	void BreakStatmentFunction(){
		if (LA.Word == "break"){
			fprintf(STDOUT, "%s%d\n", " Goto Label ", LabelNumber + 1);
			cout << " Goto Label " << LabelNumber + 1 << endl;
			Lex();
			if (LA.Word == ";")
				Lex();
			else {
				H = GetStdHandle(STD_OUTPUT_HANDLE);
				SetConsoleTextAttribute(H, FOREGROUND_RED | FOREGROUND_INTENSITY);
				fprintf(STDERROR, "%s%d%s\n", " In Line Number : ", LA.Line, " , Missing  ;");
				//fprintf(STDERROR, "%s\n", " Not Valid ");
				cout << " In Line Number :  " << LA.Line << ", Missing ; " << endl;
				cout << " Not Valid" << endl;
				//Sleep(3000);
				//exit(-1);
				 }//else 
							   }//if statment
		else {
			H = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleTextAttribute(H, FOREGROUND_RED | FOREGROUND_INTENSITY);
			fprintf(STDERROR, "%s%d%s\n", " In Line Number : ", LA.Line, " , Error");
			//fprintf(STDERROR, "%s\n", " Not Valid ");
			cout << " In Line Number :  " << LA.Line << ", Error " << endl;
			cout << " Not Valid" << endl;
			//Sleep(3000);
			//exit(-1);
			 }//else

								}//BreakStatmentFunction

	void ContinueStatmentFunction() {
		if (LA.Word == "continue"){
			fprintf(STDOUT, "%s%d\n", " Goto Label ", LabelNumber);
			cout << " Goto Label " << LabelNumber  << endl;
			Lex();
			if (LA.Word == ";")
				Lex();
			else {
				H = GetStdHandle(STD_OUTPUT_HANDLE);
				SetConsoleTextAttribute(H, FOREGROUND_RED | FOREGROUND_INTENSITY);
				fprintf(STDERROR, "%s%d%s\n", " In Line Number : ", LA.Line, " , Missing  ;");
				//fprintf(STDERROR, "%s\n", " Not Valid ");
				cout << " In Line Number :  " << LA.Line << ", Missing ; " << endl;
				cout << " Not Valid" << endl;
				//Sleep(3000);
				//exit(-1);
				 }//else 
							   }//if statment
		else {
			H = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleTextAttribute(H, FOREGROUND_RED | FOREGROUND_INTENSITY);
			fprintf(STDERROR, "%s%d%s\n", " In Line Number : ", LA.Line, " , Error");
			//fprintf(STDERROR, "%s\n", " Not Valid ");
			cout << " In Line Number :  " << LA.Line << ", Error " << endl;
			cout << " Not Valid" << endl;
			//Sleep(3000);
			//exit(-1);
			 }//else
									}//ContinueStatmentFunction();

	void StatmentFunction() {
		if (LA.Word == "if")
			IfStatmentFunction();

		else if (LA.Word == "while")
				WhileloopStatmentFunction();

			else if (LA.Word == "switch")
				SwitchStatmentFunction();

				else if (LA.Word == "do")
					DoStatmentFunction();

					else if (LA.Word == "for")
						ForLoopStatmentFunction();
						
					  else if(LA.Word == "break")
						  BreakStatmentFunction();

					  else if (LA.Word == "continue")
						  ContinueStatmentFunction();

						else if (LA.Type == "Identifier") {
							ExpresionFunction();
								if (LA.Word == ";")
									Lex();
								else {
									H = GetStdHandle(STD_OUTPUT_HANDLE);
									SetConsoleTextAttribute(H, FOREGROUND_RED | FOREGROUND_INTENSITY);
									fprintf(STDERROR, "%s%d%s\n", " In Line Number : ", LA.Line, " , Missing  ;");
									//fprintf(STDERROR, "%s\n", " Not Valid ");
									cout << " In Line Number : " << LA.Line << " , Missing ;" << endl;
									cout << " Not Valid"<< endl;
									//Sleep(3000);
									//exit(-1);
									 }//else
														  }//else if

						else if (LA.Word == ";")
									Lex();

						else if (LA.Word == "{")
							CompoundStatmentFunction();
									else {
										H = GetStdHandle(STD_OUTPUT_HANDLE);
										SetConsoleTextAttribute(H, FOREGROUND_RED | FOREGROUND_INTENSITY);
										fprintf(STDERROR, "%s%d%s\n", " In Line Number : ", LA.Line, " , Missing Statment(s)");
										//fprintf(STDERROR, "%s\n", " Not Valid ");
										cout << " In Line Number : " << LA.Line << " , Missing Statment(s)" << endl;
										cout << " Not Valid" << endl;
										//Sleep(3000);
										//exit(-1);
										 }//else

							  }//StatmentFunction

	void PreListStatmentFunction() {
		if (LA.Word == ";")
			return;
		if (LA.Type == "Identifier") {
			fprintf(STDOUT, "%s%d\n", " Lvalue ", LA.Word.c_str());
			cout << " Lvalue " << LA.Word << endl;
			Lex();


			if (LA.Word == "=") {
				Lex();
				ExpresionFunction();
				fprintf(STDOUT, "%d\n", " := ");
				cout << " := " << endl;

				if (LA.Word == ",") {
					Lex();

									}//if statmanet
								  }//if statment
										}//if statment
		
		else {
			H = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleTextAttribute(H, FOREGROUND_RED | FOREGROUND_INTENSITY);
			fprintf(STDERROR, "%s%d%s\n", " In Line Number : ", LA.Line, " , Missing Identifier(s)");
			//fprintf(STDERROR, "%s\n", " Not Valid ");
			cout << " In Line Number : " << LA.Line << " , Missing Identifier" << endl;
			cout << " Not Valid" << endl;
			//Sleep(3000); 
			//exit(-1);
			 }//else
								   }//PreListStatmentFunction

	void PostListStatmentFunction() {
		ExpresionFunction();
		if (LA.Word == ",") {
			Lex();
			PostListStatmentFunction();
							}//if statment
									}//PostListStatmentFunction

	void ForLoopStatmentFunction() {
		if (LA.Word == "for") {
			cout << " for loop Function " << endl;
			fprintf(STDOUT, "%s\n", " for loop Function ");
			Lex();

			if (LA.Word == "(") {
				Lex();
				PreListStatmentFunction();

					if (LA.Word == ";") {
						Lex();
						LabelUpdater();
						int TopLabelNumber = LabelNumber;

						ConditionFunction();
						fprintf(STDOUT, "%s", " Go False ");
						cout << " Go False "; 
						LabelUpdater();
						int FalseLabelNumber = LabelNumber;

						if (LA.Word == ";") {
							Lex();
							fprintf(STDOUT, "%s", " Go False ");
							cout << " Goto ";
							LabelUpdater();
							int Begining = LabelNumber;
							LabelUpdater();
							int PostList = LabelNumber;
							PostListStatmentFunction();
							fprintf(STDOUT, "%s%d\n", " Goto Label ", TopLabelNumber);
							cout << " Goto Label " << TopLabelNumber << endl;


							if (LA.Word == ")") {
								Lex();
								fprintf(STDOUT, "%s%d\n", " Label ", Begining);
								cout << " Label " << Begining << endl;
								StatmentFunction();
								fprintf(STDOUT, "%s%d\n", " Goto Label ", PostList);
								fprintf(STDOUT, "%s%d\n", " Label ", FalseLabelNumber);
								cout << " Goto Label " << PostList << endl;
								cout << " Label " << FalseLabelNumber << endl;
												}//if statment

							else {
								H = GetStdHandle(STD_OUTPUT_HANDLE);
								SetConsoleTextAttribute(H, FOREGROUND_RED | FOREGROUND_INTENSITY);
								fprintf(STDERROR, "%s%d%s\n", " In Line Number : ", LA.Line, " , Missing  (");
								//fprintf(STDERROR, "%s\n", " Not Valid ");
								cout << " In Line Number : " << LA.Line << " , Missing (" << endl;
								cout << " Not Valid" << endl;
								//Sleep(3000);
								//exit(-1);
								 }//else
											 }//if statment
						
						else {
							H = GetStdHandle(STD_OUTPUT_HANDLE);
							SetConsoleTextAttribute(H, FOREGROUND_RED | FOREGROUND_INTENSITY);
							fprintf(STDERROR, "%s%d%s\n", " In Line Number : ", LA.Line, " , Missing  ;");
							//fprintf(STDERROR, "%s\n", " Not Valid ");
							cout << " In Line Number : " << LA.Line << " , Missing ;" << endl; 
							cout << " Not Valid" << endl;
							//Sleep(3000); 
							//exit(-1);
							 }//else
											}//if statment

					else {
						H = GetStdHandle(STD_OUTPUT_HANDLE);
						SetConsoleTextAttribute(H, FOREGROUND_RED | FOREGROUND_INTENSITY);
						fprintf(STDERROR, "%s%d%s\n", " In Line Number : ", LA.Line, " , Missing  ;");
						//fprintf(STDERROR, "%s\n", " Not Valid ");
						cout << " In Line Number : " << LA.Line << " , Missing ;" << endl;
						cout << " Not Valid" << endl;
						//Sleep(3000);
						//exit(-1);
						 }//else
									  }//if statment

			else {
				H = GetStdHandle(STD_OUTPUT_HANDLE);
				SetConsoleTextAttribute(H, FOREGROUND_RED | FOREGROUND_INTENSITY);
				fprintf(STDERROR, "%s%d%s\n", " In Line Number : ", LA.Line, " , Missing  )");
				//fprintf(STDERROR, "%s\n", " Not Valid ");
				cout << " In Line Number : " << LA.Line << " , Missing )" << endl;
				cout << " Not Valid" << endl;
				//Sleep(3000);
				//exit(-1);
				 }//else
							  }//if statment
		
		else {
			H = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleTextAttribute(H, FOREGROUND_RED | FOREGROUND_INTENSITY);
			fprintf(STDERROR, "%s%d%s\n", " In Line Number : ", LA.Line, " , Missing  for loop");
			//fprintf(STDERROR, "%s\n", " Not Valid ");
			cout << " In Line Number : " << LA.Line << " , Missing for loop" << endl;
			cout << " Not Valid" << endl;
			//Sleep(3000); 
			//exit(-1);
			 }//else

								   }//ForLoopStatmentFunction

	void DoStatmentFunction() {
		
		
		LabelUpdater();
		int DoLable = LabelNumber;

		if (LA.Word == "do") {
			cout << " do loop Function " << endl;
			fprintf(STDOUT, "%s\n", " do loop Function ");
			Lex();

			StatmentFunction();
			if (LA.Word == "while") {
				cout << " while Function " << endl;
				fprintf(STDOUT, "%s\n", " while Function ");
				Lex();

				if (LA.Word == "(") {
					Lex();
					ConditionFunction();

					if (LA.Word == ")") {
						fprintf(STDOUT, "%s%d\n", " Go True Label ", DoLable);
						cout << " Go True Label " << DoLable << endl;
						Lex();

						if (LA.Word == ";")
							Lex();

						else {
							H = GetStdHandle(STD_OUTPUT_HANDLE);
							SetConsoleTextAttribute(H, FOREGROUND_RED | FOREGROUND_INTENSITY);
							fprintf(STDERROR, "%s%d%s\n", " In Line Number : ", LA.Line, " , Missing  ;");
							//fprintf(STDERROR, "%s\n", " Not Valid ");
							cout << " In Line Number : " << LA.Line << " , Missing ;" << endl;
							cout << " Not Valid" << endl;
							//Sleep(3000);
							//exit(-1);
							 }//else
										}//if statment

					else {
						fprintf(STDERROR, "%s%d%s\n", " In Line Number : ", LA.Line, " , Missing  )");
						//fprintf(STDERROR, "%s\n", " Not Valid ");
						cout << " In Line Number : " << LA.Line << " , Missing )" << endl;
						cout << " Not Valid" << endl;//Sleep(3000);
						//exit(-1);
						 }//else

									}//if statment
			
				else {
					H = GetStdHandle(STD_OUTPUT_HANDLE);
					SetConsoleTextAttribute(H, FOREGROUND_RED | FOREGROUND_INTENSITY);
					fprintf(STDERROR, "%s%d%s\n", " In Line Number : ", LA.Line, " , Missing  (");
					//fprintf(STDERROR, "%s\n", " Not Valid ");
					cout << " In Line Number : " << LA.Line << " , Missing (" << endl; 
					cout << " Not Valid" << endl;
					//Sleep(3000); 
					//exit(-1);
					 }//else
									}//if statment
			
			else {
				H = GetStdHandle(STD_OUTPUT_HANDLE);
				SetConsoleTextAttribute(H, FOREGROUND_RED | FOREGROUND_INTENSITY);
				fprintf(STDERROR, "%s%d%s\n", " In Line Number : ", LA.Line, " , Missing  while loop");
				//fprintf(STDERROR, "%s\n", " Not Valid ");
				cout << " In Line Number : " << LA.Line << " , Missing while loop" << endl;
				cout << " Not Valid" << endl;
				//Sleep(3000);
				//exit(-1);
				 }//else
							 }//if statment

			else {
			H = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleTextAttribute(H, FOREGROUND_RED | FOREGROUND_INTENSITY);
			fprintf(STDERROR, "%s%d%s\n", " In Line Number : ", LA.Line, " , Missing  do");
			//fprintf(STDERROR, "%s\n", " Not Valid ");
			cout << " In Line Number : " << LA.Line << " , Missing do" << endl; 
			cout << " Not Valid"<< endl;
			//Sleep(3000);
			//exit(-1);
			 }//else
			
							   }//doStatmentFunction

	void SwitchStatmentFunction() {
		if (LA.Word == "switch") {
			cout << " switch statment Function " << endl;
			fprintf(STDOUT, "%s\n", " switch statment Function ");
			Lex();

			if (LA.Word == "(") {
				Lex();

				ExpresionFunction();

				if (LA.Word == ")") {
					Lex();
					BracedCasesFunction();
									}//if statment
				
				else {
					H = GetStdHandle(STD_OUTPUT_HANDLE);
					SetConsoleTextAttribute(H, FOREGROUND_RED | FOREGROUND_INTENSITY);
					fprintf(STDERROR, "%s%d%s\n", " In Line Number : ", LA.Line, " , Missing  )");
					//fprintf(STDERROR, "%s\n", " Not Valid ");
					cout << " In Line Number : " << LA.Line << " , Missing )" << endl; 
					cout << " Not Valid" << endl;
					//Sleep(3000);    
					//exit(-1);
					 }//else
								}//if statment
			
			else {
				H = GetStdHandle(STD_OUTPUT_HANDLE);
				SetConsoleTextAttribute(H, FOREGROUND_RED | FOREGROUND_INTENSITY);
				fprintf(STDERROR, "%s%d%s\n", " In Line Number : ", LA.Line, " , Missing  (");
				//fprintf(STDERROR, "%s\n", " Not Valid ");
				cout << " In Line Number : " << LA.Line << " , missing (" << endl;
				cout << " Not Valid" << endl;
				//Sleep(3000);
				//exit(-1);
				 }//else
								 }//if statment
		
		else {
			H = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleTextAttribute(H, FOREGROUND_RED | FOREGROUND_INTENSITY);
			fprintf(STDERROR, "%s%d%s\n", " In Line Number : ", LA.Line, " , Missing  switch");
			//fprintf(STDERROR, "%s\n", " Not Valid ");
			cout << " In Line Number : " << LA.Line << " , Missing switch" << endl;
			cout << " Not Valid" << endl;
			//Sleep(3000);    
			//exit(-1);
			 }//else
								  }//SwitchStatmentFunction

	void BracedCasesFunction() {
		if (LA.Word == "{") {
			Lex();
			CasesFunction();

			if (LA.Word == "}")
				Lex();
				
			else {
				H = GetStdHandle(STD_OUTPUT_HANDLE);
				SetConsoleTextAttribute(H, FOREGROUND_RED | FOREGROUND_INTENSITY);
				fprintf(STDERROR, "%s%d%s\n", " In Line Number : ", LA.Line, " , Missing  }");
				//fprintf(STDERROR, "%s\n", " Not Valid ");
				cout << " In Line Number : " << LA.Line << " , Missing }" << endl; 
				cout << " Not Valid" << endl;
				//Sleep(3000);
				//exit(-1);
				 }//else
							}//if statment
		
		else {
			H = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleTextAttribute(H, FOREGROUND_RED | FOREGROUND_INTENSITY);
			cout << " In Line Number : " << LA.Line << " , Missing {" << endl; 
			cout << " Not Valid" << endl;
			//Sleep(3000);     
			//exit(-1);
			 }//else
							   }//BracedCasesFunction

	void CasesFunction() {

		if (LA.Word == "case") {
			CaseFunction();
			CasesFunction();
							   }//if statment

		if (LA.Word == "default") {
			DefaultFunction();
			CasesFunction();
								  }//if statment
						 }//CasesFunction

	void DefaultFunction() {

		if (LA.Word == "default") {
			Lex();
			if (LA.Word == ":") {
				Lex();
				if (LA.Word != "}" && LA.Word != "case")
					StatmentFunction();
								}//if statment
			
			else {
				H = GetStdHandle(STD_OUTPUT_HANDLE);
				SetConsoleTextAttribute(H, FOREGROUND_RED | FOREGROUND_INTENSITY);
				fprintf(STDERROR, "%s%d%s\n", " In Line Number : ", LA.Line, " , Missing  :");
				//fprintf(STDERROR, "%s\n", " Not Valid ");
				cout << " In Line Number : " << LA.Line << " , Missing :" << endl;
				cout << " Not Valid" << endl;
				//Sleep(3000);    
				//exit(-1);
				 }//else
								  }//if statment
		
		else {
			H = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleTextAttribute(H, FOREGROUND_RED | FOREGROUND_INTENSITY);
			fprintf(STDERROR, "%s%d%s\n", " In Line Number : ", LA.Line, " , Error");
			//fprintf(STDERROR, "%s\n", " Not Valid ");
			cout << " In Line Number : " << LA.Line << " , Error";
			cout << " Not Valid" << endl;
			//Sleep(3000);      
			//exit(-1);
			 }//else
						   }//DefaultFunction

	void OrderedStatmentFunction() {
		if (LA.Word != "}" && LA.Word != "case" && LA.Word != "default") {
			StatmentFunction();
			OrderedStatmentFunction();
																		 }//if statment
								   }//OrderedStatmentFunction

	void CaseFunction() {

		if (LA.Word == "case") {
			Lex();
			ExpresionFunction();
			fprintf(STDOUT, "\n%s\n", " == ");
			fprintf(STDOUT, "%s\n", " Go False ");
			cout << " == " << endl;
			cout << " Go False "; 
			LabelUpdater();
			int NextCaseLabelNumber = LabelNumber;


			if (LA.Word == ":") {
				Lex();
				OrderedStatmentFunction();
				fprintf(STDOUT, "%s%d\n", " Label ",NextCaseLabelNumber);
				cout << " Label " << NextCaseLabelNumber << endl;
								}//if statment
			
			else {
				H = GetStdHandle(STD_OUTPUT_HANDLE);
				SetConsoleTextAttribute(H, FOREGROUND_RED | FOREGROUND_INTENSITY);
				fprintf(STDERROR, "%s%d%s\n", " In Line Number : ", LA.Line, " , Missing  :");
				//fprintf(STDERROR, "%s\n", " Not Valid ");
				cout << " In Line Number : " << LA.Line << " , Missing :" << endl;
				cout << " Not Valid" << endl;
				//Sleep(3000);   
				//exit(-1);
				 }//else
							   }//if statment

		else {
			fprintf(STDERROR, "%s%d%s\n", " In Line Number : ", LA.Line, " , Error");
			//fprintf(STDERROR, "%s\n", " Not Valid ");
			cout << " In Line Number : " << LA.Line << " , Error" << endl; 
			cout << " Not Valid" << endl;
			//Sleep(3000);
			//exit(-1);
			 }//else
				   }//CaseFunction

	void MoreStatmentFunction() {
		if (LA.Word == "}")
			return;

		StatmentFunction();
		MoreStatmentFunction();

					   }//MoreStatmentFunction			

	void ConditionFunction() {
		if (LA.Word == ";" || LA.Word == ")")
			return;
		if (LA.Type == "Identifier" || LA.Type == "Number") {
			Token Current = LA;

			if (LA.Type == "Identifier") {
				fprintf(STDOUT, "%s%s\n", " Rvalue ", LA.Word.c_str());
				cout << " Rvalue " << LA.Word << endl;
										 }//if statment
			else {
				fprintf(STDOUT, "%s%s\n", " Lvalue ", LA.Word.c_str());
				cout << " Push " << LA.Word << endl;
				 }//else

			Lex();
			SPFunction(Current);

																					  }//if statment
		else {
			H = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleTextAttribute(H, FOREGROUND_RED | FOREGROUND_INTENSITY);
			fprintf(STDERROR, "%s%d%s\n", " In Line Number : ", LA.Line," Missing Identifier");
			fprintf(STDERROR, "%s\n", " Not Valid");
			cout << " In Line Number : " << LA.Line << " , Missing Identifier" << endl;
			cout << " Not Valid" << endl;
			//Sleep(3000);  
			//exit(-1);
			 }//else
							   }//ConditionFunction

	void SPFunction(Token &LP) {
		if (Operator(LA.Word)) {
			string LO = LA.Word;
			Lex();
			if (LA.Type == "Identifier" || LA.Type == "Number") {
				if (LA.Type == "Identifier") {
					fprintf(STDERROR, "%s%s\n", " Rvalue ",LA.Word.c_str());
					cout << " Rvalue " << LA.Word << endl;
											 }//if statment
				else {
					fprintf(STDOUT, "%s%s\n", " Push ",LA.Word.c_str());
					cout << " Push " << LA.Word << endl;
					 }//else

				fprintf(STDOUT, " %s\n", LO.c_str());
				cout << " " << LO << endl;

				Lex();
																}//if statment
							   }//if statment


		else {
			H = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleTextAttribute(H, FOREGROUND_RED | FOREGROUND_INTENSITY);
			fprintf(STDERROR, "%s%d%s\n", " In Line Number : ", LA.Line, " Missing Operator");
			fprintf(STDERROR, "%s\n", " Not Valid");
			cout << " In Line Number : " << LA.Line << " , Missing Operator" << endl;
			cout << " Not Valid" << endl;
			//Sleep(3000); 
			//exit(-1);
			 }//else

							}//SPFunction

	void WhileloopStatmentFunction() {
		LabelUpdater();
		int LabelNumerBeforCallingWhileLoop = LabelNumber;


		if (LA.Word == "while") {
			cout << " while loop Function " << endl;
			fprintf(STDOUT, "%s\n", " while loop Function ");
			Lex();

			if (LA.Word == "(") {
				Lex();
				ConditionFunction();

				if (LA.Word == ")") {
					Lex();
					cout << " Go False ";
					LabelUpdater();
					int GoFalseToLabelNumber = LabelNumber;
					StatmentFunction();
					fprintf(STDOUT, "%s%d\n%s%d\n", " Goto ", LabelNumerBeforCallingWhileLoop, " Label ",GoFalseToLabelNumber);
					cout << " Goto " << LabelNumerBeforCallingWhileLoop << endl << " Label " << GoFalseToLabelNumber << endl;
							 	    }//if statment

				else {
					H = GetStdHandle(STD_OUTPUT_HANDLE);
					SetConsoleTextAttribute(H, FOREGROUND_RED | FOREGROUND_INTENSITY);
					fprintf(STDERROR, "%s%d%s\n", " In Line Number : ", LA.Line, " Missing )");
					fprintf(STDERROR, "%s\n", " Not Valid");
					cout << " In Line Number : " << LA.Line << " , Missing )" << endl;
					cout << " Not Valid" << endl;
					//Sleep(3000);
					//exit(-1);
					 }//else	
								}//if statment

			else {
				fprintf(STDERROR, "%s%d%s\n", " In Line Number : ", LA.Line, " Missing (");
				fprintf(STDERROR, "%s\n", " Not Valid");
				cout << " In Line Number : " << LA.Line << " , Missing (" << endl; 
				cout << " Not Valid" << endl;//Sleep(3000); 
				//exit(-1);
				 }//else
								}//if statment
		
		else {
			fprintf(STDERROR, "%s%d%s\n", " In Line Number : ", LA.Line, " Error");
			fprintf(STDERROR, "%s\n", " Not Valid");
			cout << " In Line Number : " << LA.Line << " , Error !" << endl;
			cout << " Not Valid" << endl;//Sleep(3000);
			//exit(-1);
			 }//else
									 }//WhileloopStatmentFunction

	void IfStatmentFunction() {

		if (LA.Word == "if") {
			cout << " else if Function " << endl;
			fprintf(STDOUT, "%s\n", " if statment Function ");
			Lex();

			if (LA.Word == "(") {
				Lex();
				ConditionFunction();

				if (LA.Word == ")") {
					Lex();
					fprintf(STDOUT, "%s", " Go False");
					cout << " Go False ";
					LabelUpdater();
					int GoFalseToLabelNumber = LabelNumber;
					StatmentFunction();
					fprintf(STDOUT, "%s", " Goto ");
					cout << " Goto ";
					LabelUpdater();
					int AfterElseLabelNumber = LabelNumber;
					fprintf(STDOUT, "%s%d\n", " Label ",GoFalseToLabelNumber);
					cout << " Label " << GoFalseToLabelNumber << endl;

					if (LA.Word == "else") {
						ElseStatmentFunction();
										   }//if statment
					fprintf(STDOUT, "%s%d\n", " Label ", AfterElseLabelNumber);
					cout << " Label " << AfterElseLabelNumber << endl;
									}//if statment

				else {
					H = GetStdHandle(STD_OUTPUT_HANDLE);
					SetConsoleTextAttribute(H, FOREGROUND_RED | FOREGROUND_INTENSITY);
					fprintf(STDERROR, "%s%d%s\n", " In Line Number : ", LA.Line, " Missing )");
					fprintf(STDERROR, "%s\n", " Not Valid");
					cout << " In Line Number : " << LA.Line << " , Missing )" << endl;
					cout << " Not Valid" << endl;
					//Sleep(3000); 
					//exit(-1);
					 }//else
								}//if statment
			
			else {
				H = GetStdHandle(STD_OUTPUT_HANDLE);
				SetConsoleTextAttribute(H, FOREGROUND_RED | FOREGROUND_INTENSITY);
				fprintf(STDERROR, "%s%d%s\n", " In Line Number : ", LA.Line, " Missing (");
				fprintf(STDERROR, "%s\n", " Not Valid");
				cout << " In Line Number : " << LA.Line << " , Missing (" << endl;
				cout << " Not Valid" << endl;
				//Sleep(3000); 
				//exit(-1);
				 }//else
							 }//if statment

		else {
			H = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleTextAttribute(H, FOREGROUND_RED | FOREGROUND_INTENSITY);
			fprintf(STDERROR, "%s%d%s\n", " In Line Number : ", LA.Line, " Error !");
			fprintf(STDERROR, "%s\n", " Not Valid");
			cout << " In Line Number : " << LA.Line << " , Error !" << endl;
			cout << " Not Valid" << endl;
			//Sleep(3000);
			//exit(-1);
			 }//else

							  }//IfStatmentFunction

	void ElseStatmentFunction() {
		if (LA.Word == "else") {
			fprintf(STDOUT, "%s\n", " else statment Function ");
			cout << " else statment Function " << endl;
			Lex();
			StatmentFunction();
							   }//if statment
		else {
			H = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleTextAttribute(H, FOREGROUND_RED | FOREGROUND_INTENSITY);
			fprintf(STDERROR, "%s%d%s\n", " In Line Number : ", LA.Line, " Error !");
			fprintf(STDERROR, "%s\n", " Not Valid");
			cout << " In Line Number : " << LA.Line << " , Error !" << endl;
			cout << " Not Valid" << endl;
			//Sleep(3000);
			//exit(-1);
			 }//else

								 }//ElseStatmentFunction

	FILE * GetSTDERORR() {
		return STDERROR;
						 }//GetSTDERORR

	FILE * GetSTDOUT() {
		return STDOUT;
					   }//GetSTDOUT

			 };//Parser

void main(void) {
	Scanner a;
	a.tokenLex();
	Token T;
	Parser K;
	H = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(H, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	float Size;
	fseek(K.GetSTDERORR(), 0, SEEK_END);
	Size = ftell(K.GetSTDERORR());

	if (Size == 0) {
		fprintf(K.GetSTDOUT(), "%s\n", " Halt");
		cout << " Halt" << endl << endl;
		cout << " This Code Is Valid !" << endl;
				   }//if statment

	else if (Size != 0) {
		fprintf(K.GetSTDOUT(), "%s\n", " Halt");
		cout << " Halt" << endl;
		H = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(H, FOREGROUND_RED | FOREGROUND_INTENSITY);
		cout << " Not Valid !" << endl;
		fprintf(K.GetSTDERORR(), "%s\n", " Not Valid !");
					    }//else if

	//Sleep(50000);
	H = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(H,7);
	cout << " ";
	system("pause");
	exit(0);
				 }//main
				 
				 
/*By : Abd Alrahman Hamdan*/
