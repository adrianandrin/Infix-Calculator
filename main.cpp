#include "Stack.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ASSUMING ALL NEGATIVE NUMBER WILL BE PUT ON A PARENTHESIS*/
void InfixToPostfix(char str[],char post[]);					//converts infix expression to postfix
bool postFix(char str[], float *result);						//posfix calculator
float stringToFloat(char a[],int index);						//converts string to float data type
bool operation(char a, Stack s);								//operates the given expression
void displayError(int result);									//display's the error
void displayFloat(int error, float result, char str[]);			//display's with decimal
void displayInt(int error, float result, char str[]);			//display's without decimal
bool floatORinteger(char a[]);									//helper function to determine if it is a float or integer
int precedence(char c);											//helper function to determine what operator is first or last
bool isOperator(char a);										//helper function to detect if it is an operator or not
void removeSpaces(char *a);										//helper function to remove spaces on the string

int main(int argc, char** argv) {
	char str[50],nstr[50];
	float result;
	int error;
	
	printf("If putting a Negative number put the number on a Parenthesis with the negative symbol ex.[(-1)] :D\n\n");
	printf("Input Infix Expression: ");
	gets(str);
	removeSpaces(str); 			//in case str have spaces
	InfixToPostfix(str,nstr);
	error=postFix(nstr,&result);
	if(floatORinteger(str)){
		displayFloat(error,result,str);
	}
	else{
		displayInt(error,result,str);
	} 
	
	/*
// For directly outputing the answer	
	strcpy(str,"((-205.13)+(15-5)) ");
	removeSpaces(str);
	InfixToPostfix(str,nstr);
	error=postFix(nstr,&result);
	if(floatORinteger(str)){
		displayFloat(error,result,str);
	}
	else{
		displayInt(error,result,str);
	}
	printf("\n");
	strcpy(str,"200 + 5");
	InfixToPostfix(str,nstr);
	error=postFix(nstr,&result);
	if(floatORinteger(str)){
		displayFloat(error,result,str);
	}
	else{
		displayInt(error,result,str);
	}  */
	return 0;
}

void InfixToPostfix(char str[],char post[]){  //converts infix expression to postfix expression
	Stack s = newStack();						
	int i=0, j=0,error=0;
	char x;
	push(s,'(');
	strcat(str,")");
	char item;
	item = str[i];
	while(item!='\0'){
		char previtem = str[i-1]; //for determining the the inputed string is a negative number string
		if(item=='('){
			push(s,item);
		}
		else if(item=='-'&&previtem=='('){
			post[j]=item;
			j++;
		}
		else if(item>='0'&&item<='9'){
			post[j]=item;
			j++;
		}
		else if(isOperator(item)){
			post[j]=' ';
			j++;
			x = peek(s);
			pop(s);
			while(isOperator(x)&& precedence(x)>=precedence(item)){
				post[j]=x;
				j++;
				x = peek(s);
				pop(s);
			}
			push(s,x);
			push(s,item);
		}
		else if(item==')'){
			post[j]=' ';
			j++;
			x = peek(s);
			pop(s);
			while(x!='('){
				post[j]=x;
				j++;
				x=peek(s);
				pop(s);
			}
		}
		else if(item=='.'){
			post[j]='.';
			j++;
		}
		else {
			error = 1;
		}
		i++;
		item =str[i];
	}
	str[--i]='\0';
	post[j]='\0';
	destroy(&s);
}

bool postFix(char str[], float *result){   //evaluates the postfix calculator
	Stack s= newStack();
	float fnum;
	bool flag=0;
	char *item=strtok(str," ");
	while(item!=NULL&&flag==0){
		if(item[0]=='-'&&item[1]>='0'&&item[1]<='9'){
//			fnum = stringToFloat(item,1); 				//for converting string to float numbers without using atof()
//			fnum = -fnum;
			fnum = atof(item);
			push(s,fnum);
		}
		else if(item[0]>='0'&&item[0]<='9'){
			//fnum = stringToFloat(item,0); 		//for converting string to float numbers without using atof()
			fnum = atof(item);
			push(s,fnum);
		}
		else if(!isEmpty(s)){
			flag=operation(item[0],s);  //function evaluates the program if it does not evaule it pushes the char operator or letter 'a' for determining an invalid expression
		}
		else {
			fnum = item[0];  //if there is no operator invalid
			push(s,fnum);
			flag = 1;
		}	
		item = strtok(NULL," ");
	}
	*result = peek(s);   //peeks the last item in the stack for its answer if flag == 0, else it peeks the last item in the stack to determine the error
	pop(s);
	if(!isEmpty(s)){ //check if the stack is still not empty or not 
		flag = 1;
	}
	destroy(&s);
	return flag;
}  

bool operation(char a,Stack s){ //function evaluates the given character if stack is empty it will not proceed on evaluating but pushing the character operator
	float val1, val2,num;		// for determining if the given string is missing an operand
	bool flag = 0;
	if(isOperator(a)){
		switch(a){
			case '+':
				val1=peek(s);
				pop(s);
				if(!isEmpty(s)){
					val2=peek(s);
					pop(s);
					num = val2+val1;
					push(s,num);
				}
				else {
					num = '+';
					push(s,num);
					flag = 1;
				}
				break;
			case '-':
				val1=peek(s);
				pop(s);
				if(!isEmpty(s)){
					val2=peek(s);
					pop(s);
					num = val2-val1;
					push(s,num);
				}
				else {
					num = '-';
					push(s,num);
					flag=1;
				}
				break;
			case '*':
				val1=peek(s);
				pop(s);
				if(!isEmpty(s)){
					val2=peek(s);
					pop(s);
					num = val2*val1;
					push(s,num);
				}
				else {
					num = '*';
					push(s,num);
					flag=1;
				}
				break;
			case '/':
				val1=peek(s);
				pop(s);
				if(!isEmpty(s)){
					val2=peek(s);
					pop(s);
					num = val2/val1;
					push(s,num);
				}
				else {
					num = '/';
					push(s,num);
					flag=1;
				}
				break;	
		}
	}
	else{
		num = 'a';			//if it is not one of the character operator then it will push 'a' for condition purposese for the invalid error
		push(s,num);					// 'a' can be change into somthing else but not numbers 1 to 9 and character operators
		flag = 1;
	}
	return flag;
}


float stringToFloat(char a[],int index){   //converts string numbers to float numbers
	int i=index,point,y=1,num1=0,num2=0;
	float fnum;
	while(a[i]!='\0'){
		if(a[i]=='.'){
			point=i;
			break;
		}
		i++;
	}
	for(i=index;a[i]!='\0'&& a[i]!=' ';i++){
		if(i<point){
			num1 = num1 * 10 + ( a[i] - '0' );
		}
		else if(i == point){
			continue;
		}
		else {
			num2 =	num2 * 10 +( a[i] - '0' );
			y = y * 10;  
		}
	}
	fnum = num2 / (float) y;
	fnum+=num1;
	index = i;
	return fnum;
}

void displayError(int result){  //display's the error messages with the parameter result that was peeked on the top of the stack 
	if(result >=0 && result <= 9)  // to determine what type of error message to be used
		printf("Missing Operator");
	else if(result=='+'||result=='-'||result=='*'||result=='/'){
		printf("Missing Operand");
	}
	else {
		printf("Invalid Expression"); 
	}
}

void displayFloat(int error, float result, char str[]){ //display's with decimal value
	if(!error){
		printf("%s = %.2f",str,result);
	}
	else {
		displayError(result);
	}
}

void displayInt(int error, float result, char str[]){   //displays's without decimal value
	if(!error){
		printf("%s = %.f",str,result);
	}
	else {
		displayError(result);
	}
}
bool floatORinteger(char a[]){ 	// detects if the string number has a decimal or a devision operator for decimal prinitng  
	int i = 0;		// this is to determine what type of %f should be used.
	bool flag = false;
	while(a[i]!='\0'){
		if(a[i]=='.' || a[i]=='/'){
			flag = true;
			break;
		}
		i++;
	}
	return flag;
}

int precedence(char c) { //function that returns the precedence of the operator
	int flag=-1; 
    switch (c) { 
    	case '+': 
    	case '-': 
        flag = 1; 
  		break;
    	case '*': 
    	case '/': 
        flag = 2; 
  		break;
    } 
    return flag; 
}

bool isOperator(char a){ //check if the given character is an operator or not;
	bool flag = false;
	if(a=='+'||a=='-'||a=='*'||a=='/'){
		flag = true;
	}
	return flag;
}

void removeSpaces(char *a){  //removes the spaces of the inputed string

    int count = 0; 
    for (int i = 0; a[i]; i++){
        if (a[i] != ' ') {
            a[count++] = a[i]; 
		}
	} 
    a[count] = '\0'; 
} 
