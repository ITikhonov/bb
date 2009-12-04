#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

char b_ident[128], *ident=0;

// Punctuation, Identifier, Number, String, RegEx

char b_stack[1024]={0},*stack=b_stack;

#define CL "\x1b[01;35m["
#define RS "]\x1b[00m"

int priority(char c) {
	switch(c) {
	case '/': return 5;
	case '+': return 6;
	case '=': return 16;
	case ';': return 100;
	case '\0': return 120;
	}
}

void out1(char *i) { printf(CL"%s %.*s"RS,i,(int)(ident-b_ident),b_ident);
		     fprintf(stderr,"%s %.*s\n",i,(int)(ident-b_ident),b_ident); }
void outo(char i) { printf(CL"op%c"RS,i); fprintf(stderr,"op%c\n",i); }

int state='X',state1='0';
void parse(int c) {
	printf("\x1b[01;31m%c%c:%c(%.*s)\x1b[00m",state,state1,c,(int)(stack-b_stack),b_stack);
	switch(state) {
	case 'X':
		if(c=='I') {
			out1("ident");
			state='2';
		} else { goto abort; }
		break;
	case '2':
		if(c=='=') { *stack++='='; state='3'; }
		else { goto abort; }
		break;
	case '3':
		if(c=='N') { out1("num"); }
		else if(c=='+') {
			if(stack[-1]=='+') { outo(stack[-1]); stack[-1]=c; }
			else { goto unstack; }
		} else if(c=='/') { if(stack[-1]=='/') { outo(stack[-1]); stack[-1]=c; } else {*stack++=c;} }
		else if(c==';') { goto unstack; }
		else { goto abort; }
		break;
	default:
		goto abort;
	}
	return;

unstack: while(priority(stack[-1])<=priority(c)) {
		outo(stack[-1]);
		stack--;
	}
	*stack++=c;

	return;

abort:	printf("\n Parse abort: char '%c' state '%c'\n",c,state);
	abort();
}

/* ============================================= */

int isspace(int c) {
	if(c==' ') return 1;
	if(c=='\t') return 1;
	if(c=='\n') return 1;
	if(c=='\r') return 1;
	return 0;
}

int isident(int c) {
        if(c>='A'&&c<='Z') return 1;
        if(c>='a'&&c<='z') return 1;
        if(c=='$') return 1;
        if(c=='_') return 1;
        return isdigit(c);
}

int ispunctuation(int c) {
	char *p=".{}[],.;*+-=!&/|():?<>%^";
	for(;*p;p++) { if(c==*p) return 1; }
	return 0;
}

int isdigit(int c) {
        if(c>='0'&&c<='9') return 1;
}

/*
Punctuation
Whitespace
Identifier
String
Number
Comment
RegEx
*/

int main() {
	int fd=open("test.js",O_RDONLY);

	char buf[1024];
	int n=read(fd,buf,1024);
	int c,i,state='X',q,esc, last='X',pstate=0;
	for(;;) {
		for(i=0;i<n;i++) {
			c=buf[i];
			putchar(c);
			switch(state) {
			case 'X':
			state_X:
				state='X'; 
				if(c=='/') { state='/'; }
				else if(ispunctuation(c)) { parse(c); if(c==')') { last='E'; } else { last='P'; } }
				else if(isspace(c)) { state='W'; }
				else if(c=='\''||c=='"') { state='S'; q=c; esc=0; last='S'; }
				else if(isdigit(c)) { state='N'; last='N'; ident=b_ident; *ident++=c; }
				else if(isident(c)) { state='I'; last='I'; ident=b_ident; *ident++=c; }
				else { goto abort; }
				break;
			case 'W':
				if(isspace(c)) { /* nothing */ }
				else { goto state_X; }
				break;
			case 'I':
				if(isident(c)) { *ident++=c; }
				else { parse('I'); goto state_X; }
				break;
			case 'S':
				if(esc) { esc=0; }
				else if(c==q) { parse('S'); state='X'; }
				else if(c=='\\') { esc=1; }
				else { /* nothing */ }
				break;
			case 'N':
				if(isdigit(c) || c=='.') { *ident++=c; }
				else { parse('N'); goto state_X; }
				break;
			case '/':
				printf("\x1b[01;31m%c\x1b[00m" "\x1b[01;33m%.*s\x1b[00m",last,(int)(ident-b_ident),b_ident);
				if(c=='*') { state='C'; esc=0; }
				else if(c=='/') { state='c'; }
				else if(last!='N'&&last!='I'&&last!='E') { esc=0; state='R'; goto state_R; }
				else if(last=='I'&&strncmp(b_ident,"return",6)==0) { esc=0; state='R'; goto state_R; }
				else { parse('/'); goto state_X; }
				break;
			case 'R':
			state_R:
				if(esc) { esc=0; }
				else if(c=='\\') { esc=1; }
				else if(c=='/') { parse('R'); state='X'; }
				else { /* nothing */ }
				break;
			case 'C':
				if(esc) { if(c=='/') { state='X'; } else { esc=0; } }
				else if(c=='*') { esc=1; }
				else { /* nothing */ }
				break;
			case 'c':
				if(c=='\n'||c=='\r') { state='X'; }
				else { /* nothing */ }
				break;
			default:
				abort();
			}

			if(pstate!=state) { printf("\x1b[01;32m%c\x1b[00m",state); }
			pstate=state;
		}
		n=read(fd,buf,1024);
		if(n==0) break;
	}

	return 0;

	abort:
		printf("\n Abort: char '%c' state '%c'\n",c,state);
	return 1;
}

