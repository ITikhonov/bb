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
	if(c=='-') return 1;
	return 0;
}

int istag(int c) {
	if(isident(c)) return 1;
	if(c=='/') return 1;
	return 0;
}

char *pe="\1b[01;3";

int main() {
	char buf[1024];
	int n=read(0,buf,1024);
	int i,state='X',q,pstate=0,cc;
	for(;;) {
		for(i=0;i<n;i++) {
			int c=buf[i];
			switch(state) {
			case 'X':
				state=c=='<'?'<':'X'; break;
			case '<':
				if(istag(c)) { state='T'; }
				else if(c=='!') { state='!'; }
				else { abort(); }
				break;
			case '!':
				if(c=='-') { state='C'; cc=0; }
				else { abort(); }
				break;
			case 'C':
				if(c=='-') { cc++; }
				else if(c=='>' && cc>1) { state='X'; }
				else { cc=0; }
				break;
			case 'T':
				if(istag(c)) { state='T'; }
				else if(c=='>') { state='X'; }
				else if(isspace(c)) { state=' '; }
				else { abort(); }
				break;
			case ' ':
				if(isspace(c)) { state=' '; }
				else if(c=='/') { state='/'; }
				else if(c=='>') { state='X'; }
				else if(isident(c)) { state='N'; }
				else { abort(); }
				break;
			case '/':
				if(c=='>') { state='X'; }
				else { abort(); }
				break;
			case 'N':
				if(isident(c)) { state='N'; }
				else if(c=='=') { state='='; }
				else if(c=='>') { state='X'; }
				else { abort(); }
				break;
			case '=':
				if(c=='"' || c=='\'') { state='V'; q=c; }
				else { abort(); }
				break;
			case 'V':
				if(c==q) { state='v'; }
				else { state='V'; }
				break;
			case 'v':
				if(isspace(c)) { state=' '; }
				else if(c=='>') { state='X'; }
				else if(c=='/') { state='/'; }
				else { abort(); }
				break;
			default:
				abort();
			}
			if(state!=pstate) {
				int clr;
				switch(state) {
					case 'X': clr=30;break;
					case '<': clr=31;break;
					case 'T': clr=32;break;
					case ' ': clr=33;break;
					case 'N': clr=34;break;
					case '=': clr=35;break;
					case 'V': clr=36;break;
					case 'C': clr=37;break;
				}
				printf("\x1b[01;%dm",clr);
				//putchar(state);
			}
			putchar(c);
			pstate=state;
		}
		n=read(0,buf,1024);
		if(n==0) break;
	}
}

