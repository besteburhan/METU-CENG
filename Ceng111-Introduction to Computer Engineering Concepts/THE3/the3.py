import random
def hyphenate(k):
    pivot=[]
    vowel=['a','e','i','I','u','U','o','O']
    i=0
    while i>=0:
        if k[i]==" ":
            pivot=pivot+[k[0:i]]
            pivot=pivot +[" "]
            k=k[i+1:]
            i=0
            continue
        if k[i]=="." and len(k[i:])!=1:
            pivot=pivot+[k[0:i]]
            pivot=pivot +["."]
            k=k[i+1:]
            i=0
            continue
        if k[i]=="." and len(k[i:])==1:
            pivot=pivot+[k[0:i]]
            pivot=pivot +["."]
            break
        if k[i] in vowel and i!=len(k)-1:
            i=i+1
            if k[i] in vowel:
                pivot=pivot+[k[0:i]]
                k=k[i:]
                i=0
                continue
            if k[i]==" ":
                pivot=pivot+[k[0:i]]
                pivot=pivot +[" "]
                k=k[i+1:]
                i=0
                continue
            if k[i]=="." and len(k[i:])!=1:
                pivot=pivot+[k[0:i]]
                pivot=pivot +["."]
                k=k[i+1:]
                i=0
                continue
            if k[i]=="." and len(k[i:])==1:
                pivot=pivot+[k[0:i]]
                pivot=pivot +["."]
                break
            else:
                while i>=0:
                    if i==len(k)-1:
                        break
                    else:
                        i=i+1
                        if k[i] in vowel :
                            pivot=pivot+[k[0:i-1]]
                            k=k[i-1:]
                            i=0
                            break
                        if k[i]==" ":
                            pivot=pivot+[k[0:i]]
                            pivot=pivot +[" "]
                            k=k[i+1:]
                            i=0
                            break
                        if k[i]=="." and len(k[i:])==1:
                            pivot=pivot+[k[0:i]]
                            pivot=pivot +["."]
                            i=-1
                            break
                        if k[i]==".":
                            pivot=pivot+[k[0:i]]
                            pivot=pivot +["."]
                            k=k[i+1:]
                            i=0
                            break
                        else:
                            continue        
        else:
            if i==len(k)-1:
                pivot=pivot+[k[0:i+1]]
                break    
            else:
                i=i+1
                continue
    return pivot
def execute():
    lineliste=[]
    while True:
        inp=raw_input()
        if inp == '=':
            break
        else:
            lineliste.append(inp)
    syl=[]
    syl = syl + lineliste[0].split() + [" "]
    i=1    	
    while i<len(lineliste):
        if lineliste[i][-1] !=".":
            syl= syl + hyphenate(lineliste[i]) +[" "]
            i=i+1
        else:
            syl=syl + hyphenate(lineliste[i])
            i=i+1
    n_m=syl[0:2]
    syl=syl[2:]
    
    syls_d={}
    for i in range(len(syl)-1):
        syls_d.setdefault(syl[i],helper(syl,syl[i]))
    if syls_d.has_key("."):
        d=random.choice([" ","."])
        a=random.choice(syls_d[d].keys())
    else:
        d=" "
        a=random.choice(syls_d[d].keys())
    output=a
    n=int(n_m[0])
    m =int(n_m[1])
    while  len(out_split(output))<= n and len(output)<m:
        L=syls_d[a].values()
        max_1=max(L)
        max_2=0
        L.remove(max_1)
        if len(L)>1:
            max_2=max(L)
        rc=[]
        for i in syls_d[a].keys():
            if syls_d[a][i]==max_1 or syls_d[a][i]==max_2:
                rc =rc + [i]
        if len(rc)>0:
            while True:
                a=random.choice(rc)
                if syls_d.has_key(a):
                    output+=a
                    break
    print output
def out_split(o):
    if " " in o:
        out=o.split(" ")
        for i in range(len(out)-1):
            if out[i].find(".")>0:
                out.insert(i+1,out[i][".".find(out[i])+1:])
                out[i]=out[i][0:".".find(out[i])]
            else:
                break
    else:
        out=["o"]
    return out
def helper(u,y):
    lis={}
    for j in range(len(u)-1):
        if y==u[j]:
            if lis.has_key(u[j+1]):
                lis[u[j+1]]=[lis[u[j+1]][0]+1]
            else:
                lis.setdefault(u[j+1],[1])
    return lis
print execute()
