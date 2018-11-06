#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "stack.h"
#include "cstack.h"
/* pre-condition:
 *    arr[] is an arry of characters, 
 *    n is the maximum possible length of the array 
 * post-condition: 
 *    arr[] contains a 'string', max 'n' in length
 * 	  arr[] may contain white-space
 *	  arr[] gets its contents from user
 *	  function returns no. of characters read
 */
int readline(char *arr, int n) {
	char ch;
	int i = 0;
	while((ch = getchar()) != '\n'  && i < n)
		arr[i++] = ch;
	arr[i] = '\0';
	return i;
}

#define OPERAND	100
#define OPERATOR 200
#define END		300
#define ERROR	400
typedef struct token {
	int type; // can be OPERAND, OPERATOR, END, ERROR
	union data {
		int num;
		char op;	
	}data;
}token;

enum state {START, DIG, OP, SPC, STOP, ERR};
token getnext(char *str, int *restart) {
	static int i = 0;
	static int currstate = START;
	int nextstate;
	char currchar, currop;
	int sum = 0;
	token t;

	if(*restart == 1) {
		i = 0;
		currstate = START;
		*restart = 0;
	}
	while(1) {
		currchar = str[i];
		switch(currstate) {
			case START:
				switch(currchar) {
					case '0': case '1': case '2':
					case '4': case '5': case '6':
					case '7': case '8': case '9':
					case '3': 
						nextstate = DIG;
						sum = currchar - '0';
						break;
					case '+': case '-': case '*':
					case '/': case '%':
						nextstate = OP;
						currop = currchar;
						break;
					case ' ': case '\t':
						nextstate = SPC;
						break;
					case '\0':
						nextstate = STOP;
						break;
					default:
						nextstate = ERR;
						break;
				}
				break;
			case DIG:
				switch(currchar) {
					case '0': case '1': case '2':
					case '4': case '5': case '6':
					case '7': case '8': case '9':
					case '3': 
						nextstate = DIG;
						sum = sum * 10 + currchar - '0';
						break;
					case '+': case '-': case '*':
					case '/': case '%':
						nextstate = OP;
						t.type = OPERAND;
						t.data.num = sum;
						i++;
						currstate = nextstate;
						currop = currchar;
						return t;	
						break;
					case ' ': case '\t':
						nextstate = SPC;
						t.type = OPERAND;
						t.data.num = sum;
						i++;
						currstate = nextstate;
						return t;	
						break;
					case '\0':
						nextstate = STOP;
						t.type = OPERAND;
						t.data.num = sum;
						i++;
						currstate = nextstate;
						return t;	
						break;
					default:
						nextstate = ERR;
						t.type = OPERAND;
						t.data.num = sum;
						i++;
						currstate = nextstate;
						return t;	
						break;
				}

				break;
			case OP:
				switch(currchar) {
					case '0': case '1': case '2':
					case '4': case '5': case '6':
					case '7': case '8': case '9':
					case '3': 
						nextstate = DIG;
						sum =  currchar - '0';
						t.type = OPERATOR;
						t.data.op = currop;
						i++;
						currstate = nextstate;
						return t;
						break;
					case '+': case '-': case '*':
					case '/': case '%':
						nextstate = OP;
						t.type = OPERATOR;
						t.data.op = currop;
						i++;
						currstate = nextstate;
						currop = currchar;
						return t;
						break;
					case ' ': case '\t':
						nextstate = SPC;
						t.type = OPERATOR;
						t.data.op = currop;
						i++;
						currstate = nextstate;
						return t;
						break;
					case '\0':
						nextstate = STOP;
						t.type = OPERATOR;
						t.data.op = currop;
						i++;
						currstate = nextstate;
						return t;
						break;
					default:
						nextstate = ERR;
						t.type = OPERATOR;
						t.data.op = currop;
						i++;
						currstate = nextstate;
						return t;
						break;
				}

				break;
			case SPC:
				switch(currchar) {
					case '0': case '1': case '2':
					case '4': case '5': case '6':
					case '7': case '8': case '9':
					case '3': 
						nextstate = DIG;
						sum = currchar - '0';
						break;
					case '+': case '-': case '*':
					case '/': case '%':
						nextstate = OP;
						currop = currchar;
						break;
					case ' ': case '\t':
						nextstate = SPC;
						break;
					case '\0':
						nextstate = STOP;
						break;
					default:
						nextstate = ERR;
						break;
				}

				break;
			case STOP:
				t.type = END;
				return t;
				break;
			case ERR:
				t.type = ERROR;
				return t;
				break;
			default:
				break;
		}
		currstate = nextstate;
		i++;
	}
}

/* postfix evaluator function
 * pre-condition: 
 * 		str is a string.
 * 		str contains an expression entered by the user
 * post-confition:
 * 		function returns value of the postfix expression
 *		returns INT_MIN on error in input	
 */
int postfix(char *str) {
	token t;
	int x, y, result;
	stack a;
	int restart = 1;
	init(&a);
	while(1) {
		t = getnext(str, &restart);
		printf("type:%d num: %d op: %c\n", t.type, t.data.num, t.data.op);
		if(t.type == OPERAND) {
			if(!isfull(&a))
				push(&a, t.data.num);
			else
				return INT_MIN;
			//a[i++] = t.data.num;	
		} else if(t.type == OPERATOR) {
			if(!isempty(&a))
				x = pop(&a);
			else
				return INT_MIN;
			if(!isempty(&a))
				y = pop(&a);
			else
				return INT_MIN;
			//x = a[i - 1]; i--;
			//y = a[i - 1]; i--;
			switch(t.data.op) {
				case '+':
					result = y + x;
					break;
				case '-':
					result = y - x;
					break;
				case '*':
					result = y * x;
					break;
				case '/':
					result = y / x;
					break;
				case '%':
					result = y % x;
					break;
				default:
					return INT_MIN;	
			}
			//a[i++] = result;
			if(!isfull(&a))
				push(&a, result);
			else
				return INT_MIN;
			//i--;
		} else if(t.type == END) {
			break;
		} else if(t.type == ERROR) {
			return INT_MIN;
		}
	}
	if(!isempty(&a))
		result = pop(&a);
	else
		return INT_MIN;
	if(isempty(&a))
		return result;
	else
		return INT_MIN;
}
int precedence(char op) {
	if(op == '%')
		return 50;
	if(op == '*' || op == '/')
		return 30;
	if(op ==  '+' || op == '-')
		return 20;
	return 0;
}
char ctop(cstack *s) {
	char ch = cpop(s);
	cpush(s, ch);
	return ch;
}
/* 
 * Function returns malloced memory. THe caller should free it. 
 * function returns NULL on error in input 'str'
 */
char *intopost(char *str) {
	token t;
	cstack cs;
	char *result = malloc(1024);
	char temp[16];
	cinit(&cs);
	int restart = 1;
	int preccurr, prectop;
	char ch;

	strcpy(result, "");
	while(1) {
		t = getnext(str, &restart);
		if(t.type == OPERAND) {
			sprintf(temp, "%d", t.data.num);
			strcat(result, temp);
			strcat(result, " ");
		} else if(t.type == OPERATOR) {
			if(!cisempty(&cs)) {
				preccurr = precedence(t.data.op);
				prectop = precedence(ctop(&cs));
				while(!cisempty(&cs) && preccurr <= prectop) {
					ch = cpop(&cs);
					temp[0] = ch;
					temp[1] = '\0';
					strcat(result, temp);
					strcat(result, " ");
					prectop = precedence(ctop(&cs));
				}
			}
			//push the current operator
			cpush(&cs, t.data.op);
		} else if(t.type == END) {
			while(!cisempty(&cs)) { 
				ch = cpop(&cs);
				temp[0] = ch;
				temp[1] = '\0';
				strcat(result, temp);
			}
			break;
		} else if(t.type == ERROR) {
			free(result);
			return NULL;
		}	
	}
	return result;
}
/* postfix evaluator.
 * input: A string, postfix epxression. 
 * 		Contains operands and operators,,
 * 		operands are separted by one or more spaces.
 *		operands-operators are separated by zero or more 
 *		spaces.
 *		May contain errors
 *		e.g. 
 *			11  22 33  + + 
 *			112 123+
 *			11                  1   11 ++
 *			Incorrect, but possible inputs:
 *			11  a +
 *			1 + 2  OR		+ 1 2 
 *			1 2 3 +
 *			1 + +
 *			1 0 / 
 */
int main(int argc, char *argv[]) {
	char line[128], x, y;
	char *p;
	while(x = readline(line, 128)) {
		p = intopost(line);
		if(p) {
			y = postfix(p);
			if(y != INT_MIN)
				printf("%d\n", y);	
			else 
				fprintf(stderr, "Bad expression\n");
			free(p);
		}
	}
	return 0;
}
