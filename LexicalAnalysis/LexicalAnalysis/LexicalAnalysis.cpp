
#include "pch.h"
#include <iostream>

//This is the chart of the KeyWords
static char keyWords[26][20] = {
      "break", "case", "char", "const", "continue",
      "default", "do", "double", "else", "enum",
      "float", "for", "goto", "if", "int", "long",
      "return", "short", "sizeof", "static", "struct",
	  "switch", "typedef", "union", "void", "while"
  };

//This is the chart of the SingleDeliniterWords
static char singleDelimiterWords[26][10] = {
	 "+", "-", "*", "/", "<", ">", "=",
	 ";", "(", ")", "^", ",", "\"", "\'", "#", "&",
	 "|", "%", "~", "[", "]", "{", "}", ".", ":", "!"
};

//This is the chart of the DoubleDeliniterWords
static char doubleDelimiterWords[10][10] = {
	 "<=", ">=", "==", "!=", "&&", "||", "<<", ">>", "\\", "\?"
};

//This is the chart of the Identifier
static  char IdentifierTbl[1000][50] = { "" };

//Method of seaching keyWords
int findKeyWord(char keyWords[][20], char s[]) {
	for (int i = 0; i <= 25; i++) {
		if (strcmp(keyWords[i], s) == 0) {
			return i + 1;
		}
	}
	return -1;
}

//Method of judging letter
bool isLetter(char a) {
	if ((a >= 'a' && a <= 'z') || (a >= 'A' && a <= 'z') || (a == '_')) {
		return true;
	}
	else {
		return false;
	}
}

//Method of judging digit
bool isDigit(char b) {
	if (b >= '0' && b <= '9') {
		return true;
	}
	else {
		return false;
	}
}

//Method of flit the useless char
void flitWords(char words[], int p) {
	char temp[10000];
	int count = 0;
	for (int i = 0; i <= p; i++) {
		if (words[i] == '/' && words[i + 1] == '/') {
			while (words[i] != '\n')
			{
				i++;
			}
		}
		if (words[i] == '/' && words[i+1]=='*') {
			i += 2; 
			while (words[i]!='*' || words[i+1]!='/') {
				i++;
				if (words[i] == '$') {
					printf("Comment Error!\n");
					exit(0);
				}
			}
			i += 2;
		}
		if (words[i] != '\n' && words[i] != '\t'&&words[i] != '\v' && words[i] != '\r') {
			temp[count] = words[i];
			count++;
		}
	}
	temp[count] = '\0';
	strcpy(words, temp);
}

//Method of analysing program
void analyProgram(int &n, char resource[], char token[], int &p) {
	int i = 0;
	int count = 0;
	char judgeChar;
	judgeChar = resource[p];

	while (judgeChar == ' '){
		p++;
		judgeChar = resource[p];
	}

	for (i = 0; i < 20; i++) {
		token[i] = '\0';
	}
	if (isLetter(resource[p])) {
		token[count] = resource[p];
		p++;
		count++;
		while (isLetter(resource[p]) || isDigit(resource[p]))
		{
			token[count] = resource[p];
			count++;
			p++;
		}
		token[count] = '\0';
		n = findKeyWord(keyWords, token);
		if (n == -1) {
			n = 1000;//To mark this is a identifier
		}
		return;
	}
	else if (isDigit(resource[p])) {
		while (isDigit(resource[p]))
		{
			token[count] = resource[p];
			count++;
			p++;
		}
		token[count] = '\0';
		n = 999;//To mark this is a number
	}
	else if (judgeChar == '+' || judgeChar == '-' || judgeChar == '*' || judgeChar == '/'
			|| judgeChar == ';' || judgeChar == '(' || judgeChar == ')' || judgeChar == '^'
			|| judgeChar == ',' || judgeChar == '\"' || judgeChar == '\'' || judgeChar == '~'
			|| judgeChar == '#' || judgeChar == '%' || judgeChar == '[' || judgeChar == ']' 
			|| judgeChar == '{' || judgeChar == '}' || judgeChar == '\\' || judgeChar == '.'
			|| judgeChar == '\?' || judgeChar == ':') {
		token[0] = resource[p];
		token[1] = '\0';
		for (i = 0; i <= 25; i++) {
			if (strcmp(token, singleDelimiterWords[i]) == 0) {
				n = 333 + i;//To mark this is a single operator
				break;
			}
		}
		for (i = 0; i <= 9; i++) {
			if (strcmp(token, doubleDelimiterWords[i]) == 0) {
				n = 366 + i;//To mark this is a single operator
				break;
			}
		}
		p++;
		return; 
	}
	else if (resource[p] == '<') {
		p++;
		if (resource[p] == '=') {
			n = 388;
		}
		else if (resource[p] == '<') {
			p--;
			n = 588;
		}
		else {
			p--;
			n = 377;
		}
		p++;
		return;
	}
	else if (resource[p] == '>') {
		p++;
		if (resource[p] == '=') {
			n = 400;
		}
		else if (resource[p] == '>') {
			n = 599;
		}
		else {
			p--;
			n = 399;
		}
		p++;
		return;
	}
	else if (resource[p] == '=') {
		p++;
		if (resource[p] == '=') {
			n = 422;
		}
		else {
			p--;
			n = 411;
		}
		p++;
		return;
	}
	else if (resource[p] == '!') {
		p++;
		if (resource[p] == '=') {
			n = 433;
		}
		else {
			n = 688;
			p--;
		}
		p++;
		return;
	}
	else if (resource[p] == '&') {
		p++;
		if (resource[p] == '&') {
			n = 533;
		}
		else {
			n = 522;
			p--;
		}
		p++;
		return;
	}
	else if (resource[p] == '|') {
		p++;
		if (resource[p] == '|') {
			n = 555;
		}
		else {
			n = 544;
			p--;
		}
		p++;
		return;
	}
	else if (resource[p] == '$') {
		n = 0;
	}
	else {
		printf("Error: There is a %c that can't be recognized.\n" , judgeChar);
		exit(0);
	}
}

int main()
{
	char resource[10000];
	char token[20] = {0};
	int n = -1;
	int i;
	int pointer = 0;
	printf("1");
	FILE *fp, *fp2;
	if ((fp = fopen("input.txt", "r")) == NULL) {
		std::cout << "Can not open the source file";
		exit(0);
	}
	printf("2");
	resource[pointer] = fgetc(fp);
	while (resource[pointer]!=EOF) {
		pointer++;
		resource[pointer] = fgetc(fp);
	}
	pointer++;
	resource[pointer] = '\0';
	fclose(fp);
	printf("3");
	flitWords(resource, pointer);
	pointer = 0;

	if ((fp2 = fopen("output.txt", "w+")) == NULL) {
		std::cout << "Cannot open the output file\n";
		exit(0);
	}
	printf("4");
	while (n!=0) {
		analyProgram(n, resource, token, pointer);
		if (n == 1000) {
			for (i = 0; i <= 1000; i++) {
				if (strcmp(IdentifierTbl[i], token) == 0) {
					break;
				}
				if (strcmp(IdentifierTbl[i], "") == 0) {
					strcpy(IdentifierTbl[i], token);
					break;
				}
			}
			fprintf(fp2, "(Identifier, %s)\n", token);
		}
		else if (n >0 && n < 333) {
			fprintf(fp2, "(KeyWord, %s)\n", keyWords[n-1]);
		}
		else if (n == 999)
		{   
			fprintf(fp2, "(Const Number, %s)\n", token);
		}
		else if (n >= 333 && n <= 366)
		{
			fprintf(fp2, "(singleDelimiterWords, %s)\n",singleDelimiterWords[n - 333]);
		}
		else if (n >= 366 && n <= 688)
		{
			fprintf(fp2, "(doubleDelimiterWords, %s)\n", doubleDelimiterWords[n - 366]);
		}
	}
	printf("5");
	for (i = 0; i <= 100; i++) {
		if (strcmp(IdentifierTbl[i], "") == 0) {

		}
		fprintf(fp2, "(Identifier: %s)\n", i + 1, IdentifierTbl[i]);
	}
	printf("6");
	fclose(fp2);
	return 0;
}