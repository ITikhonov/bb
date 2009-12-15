from operator import div
from sys import stdin,stderr

prog=[]

for l in stdin:
	prog.append(l.split())

r=[[]]
s=r[0]
v={'pow':pow}
ip=0

while ip<len(prog):
	l=prog[ip]
	if l[0]=='ident': s.append(l[1])
	elif l[0]=='num': s.append(float(l[1]))
	elif l[0]=='op+': s.append(s.pop()+s.pop())
	elif l[0]=='op-': s.append(s.pop(-2)-s.pop())
	elif l[0]=='op/': s.append(div(s.pop(-2),s.pop()))
	elif l[0]=='op*': s.append(s.pop()*s.pop())
	elif l[0]=='op=': v[s.pop(-2)]=s[-1]
	elif l[0]=='op;': s.pop()
	elif l[0]=='op(':
		r.append([])
		s=r[-1]
	elif l[0]=='opf':
		arg=r.pop()
		s=r[-1]
		s.append(s.pop()(*arg))
	elif l[0]=='get': s.append(v[l[1]])
	elif l[0]=='com': pass
	else: raise Exception(str(l))
	stderr.write(str((l,r,v))+'\n')
	ip+=1

assert s==[],s
assert v=={'a': 13.5, 'c': 0.66666666666666663, 'b': 10.0, 'd': 1.0, 'e':14.666666666666666,'f0':531441.0,'f':31429.891510774083,'pow':pow,'g':3.0,'h':7.0},v

