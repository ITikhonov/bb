from operator import div
from sys import stdin,stderr

c=stdin
s=[]
v={}

for l in c:
	l=l.split()

	if l[0]=='ident': s.append(l[1])
	elif l[0]=='num': s.append(float(l[1]))
	elif l[0]=='op+': s.append(s.pop()+s.pop())
	elif l[0]=='op-': s.append(s.pop(-2)-s.pop())
	elif l[0]=='op/': s.append(div(s.pop(-2),s.pop()))
	elif l[0]=='op*': s.append(s.pop()*s.pop())
	elif l[0]=='op=': v[s.pop()]=s.pop()
	elif l[0]=='get': s.append(v[l[1]])
	elif l[0]=='com': pass
	else: raise Exception(str(l))
	stderr.write(str((l,s))+'\n')

assert s==[],s
assert v=={'a': 13.5, 'c': 0.66666666666666663, 'b': 10.0, 'd': 1.0, 'e':14.666666666666666},v

