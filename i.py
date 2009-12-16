from operator import div
from sys import stdin,stderr

prog=[]
clos={}

line=0
for l in stdin:
	l=l.split()
	if l[0] == 'end':
		clos[l[1]]=line+1;
	prog.append(l)
	line+=1

rs=[]
r=[[]]
s=r[0]
vs=[{'pow':pow}]
ip=0
args=[]

def getvar(v):
	for x in vs:
		if v in x:
			return x[v]

def setvar(v,z):
	for x in vs:
		if v in x:
			x[v]=z
			return
	vs[-1][v]=z

while ip<len(prog):
	l=prog[ip]
	ip+=1
	if l[0]=='ident': s.append(l[1])
	elif l[0]=='num': s.append(float(l[1]))
	elif l[0]=='op+': s.append(s.pop()+s.pop())
	elif l[0]=='op-': s.append(s.pop(-2)-s.pop())
	elif l[0]=='op/': s.append(div(s.pop(-2),s.pop()))
	elif l[0]=='op*': s.append(s.pop()*s.pop())
	elif l[0]=='op=': setvar(s.pop(-2),s[-1])
	elif l[0]=='op;': s.pop()
	elif l[0]=='def': s.append((ip,)); ip=clos[l[1]]
	elif l[0]=='end': ip=rs.pop(); s.append(None); args.pop(); vs.pop(0)
	elif l[0]=='args': vs.insert(0,{});
	elif l[0]=='arg': vs[0][l[1]]=args[-1].pop(0)
	elif l[0]=='op(':
		r.append([])
		s=r[-1]
	elif l[0]=='opf':
		arg=r.pop()
		s=r[-1]
		f=s.pop()
		if callable(f):
			s.append(f(*arg))
		else:
			args.append(arg)
			rs.append(ip)
			ip=f[0]
	elif l[0]=='get': s.append(getvar(l[1]))
	elif l[0]=='com': pass
	else: raise Exception(str(l))
	stderr.write(str((l,r,vs))+'\n')

assert s==[],s
v=vs[0]
del v['i']
del v['k']
smpl={'a': 13.5, 'c': 0.66666666666666663, 'b': 10.0, 'd': 1.0, 'e':14.666666666666666,'f0':531441.0,'f':31429.891510774083,'pow':pow,'g':3.0,'h':7.0,'j':8.0,'l':81.0}
assert v==smpl, ('\n\n\nhas:  '+str(sorted(v.items()))+'\n\nmust: '+str(sorted(smpl.items())))

