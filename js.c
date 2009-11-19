#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int isspace(int c) {
	if(c==' ') return 1;
	if(c=='\t') return 1;
	if(c=='\n') return 1;
	if(c=='\r') return 1;
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
	char buf[1024];
	int n=read(0,buf,1024);
	int c,i,state='X',q,esc, last='X',pstate=0;
	char b_ident[128], *ident=0;
	for(;;) {
		for(i=0;i<n;i++) {
			c=buf[i];
			putchar(c);
			switch(state) {
			case 'X':
			state_X:
				state='X'; 
				if(c=='/') { state='/'; }
				else if(ispunctuation(c)) { if(c==')') { last='E'; } else { last='P'; } }
				else if(isspace(c)) { state='W'; }
				else if(c=='\''||c=='"') { state='S'; q=c; esc=0; last='S'; }
				else if(isdigit(c)) { state='N'; last='N'; }
				else if(isident(c)) { state='I'; last='I'; ident=b_ident; *ident++=c; }
				else { goto abort; }
				break;
			case 'W':
				if(isspace(c)) { /* nothing */ }
				else { goto state_X; }
				break;
			case 'I':
				if(isident(c)) { *ident++=c; }
				else { goto state_X; }
				break;
			case 'S':
				if(esc) { esc=0; }
				else if(c==q) { state='X'; }
				else if(c=='\\') { esc=1; }
				else { /* nothing */ }
				break;
			case 'N':
				if(isdigit(c)) { /* nothing */ }
				else if(c=='.') { /* nothing */ }
				else {goto state_X; }
				break;
			case '/':
				printf("\x1b[01;31m%c\x1b[00m" "\x1b[01;33m%.*s\x1b[00m",last,(int)(ident-b_ident),b_ident);
				if(c=='*') { state='C'; esc=0; }
				else if(c=='/') { state='c'; }
				else if(last!='N'&&last!='I'&&last!='E') { esc=0; state='R'; goto state_R; }
				else if(last=='I'&&strncmp(b_ident,"return",6)==0) { esc=0; state='R'; goto state_R; }
				else { goto state_X; }
				break;
			case 'R':
			state_R:
				if(esc) { esc=0; }
				else if(c=='\\') { esc=1; }
				else if(c=='/') { state='X'; }
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
		n=read(0,buf,1024);
		if(n==0) break;
	}

	return 0;

	abort:
		printf("\n Abort: char '%c' state '%c'\n",c,state);
	return 1;
}

