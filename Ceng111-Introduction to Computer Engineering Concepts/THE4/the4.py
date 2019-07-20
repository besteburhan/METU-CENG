import operator
def query(tree,variable_value):
    ops={'<':operator.lt,'<=':operator.le,'==':operator.eq,'!=':operator.ne,'>':operator.gt,'>=':operator.ge,'and':operator.and_,'or':operator.or_}
    if type(tree)==str:
        return tree
    if tree[0][0]=='and':
        for i in range(1,len(tree[0])):
            if i<len(tree[0])-1 and true_false(tree[0][i],variable_value):
                continue
            if i<len(tree[0])-1 and not true_false(tree[0][i],variable_value):
                return query(tree[2],variable_value)
                break
            if i==len(tree[0])-1 and true_false(tree[0][i],variable_value):
                return query(tree[1],variable_value)
            if i==len(tree[0])-1 and not true_false(tree[0][i],variable_value):
                return query(tree[2],variable_value)
    if tree[0][0]=='or':
        for i in range(1,len(tree[0])):
            if i<len(tree[0])-1 and not true_false(tree[0][i],variable_value):
                continue
            if i<len(tree[0])-1 and  true_false(tree[0][i],variable_value):
                return query(tree[1],variable_value)
                break
            if i==len(tree[0])-1 and true_false(tree[0][i],variable_value):
                return query(tree[1],variable_value)
            if i==len(tree[0])-1 and not true_false(tree[0][i],variable_value):
                return query(tree[2],variable_value)
    if tree[0][0]=='not':
        if true_false(tree[0][1],variable_value):
            return query(tree[2],variable_value)
        else:
            return query(tree[1],variable_value)
    if tree[0][0]=='in':
        if true_false(tree[0],variable_value):
            return query(tree[1],variable_value)
        else:
            return query(tree[2],variable_value)
    if tree[0][0]=='<' or tree[0][0]=='>' or tree[0][0]=='<=' or tree[0][0]=='>=' or tree[0][0]=='==' or tree[0][0]=='!=':
        if true_false(tree[0],variable_value):
            return query(tree[1],variable_value)
        else:
            return query(tree[2],variable_value)
def true_false(bool_,variable_value):
    ops={'<':operator.lt,'<=':operator.le,'==':operator.eq,'!=':operator.ne,'>':operator.gt,'>=':operator.ge,'and':operator.and_,'or':operator.or_}
    variable=[]
    for i in range(0,len(variable_value)):
        variable.append(variable_value[i][0])
    if bool_[0]=='in':
        if type(bool_[1])==str:
            bool_[1]=help(bool_[1],variable_value)
            if bool_[1] in bool_[2]:
                return True
            else:
                return False
        if type(bool_[2])==str:
            bool_[2]=help(bool_[2],variable_value)
            if bool_[2] in bool_[1]:
                return True
            else:
                return False
        if type(bool_[2])==list and type(bool_[1])!=str :
            if bool_[1] in bool_[2]:
                return True
            else:
                return False
        if type(bool_[1])==list and type(bool_[2])!=str :
            if bool_[2] in bool_[1]:
                return True
            else:
                return False
    if bool_[0]=='not':
        if  true_false(bool_[1],variable_value):
            return False
        else:
            return True
    if bool_[0]=='<' or bool_[0]=='>' or bool_[0]=='==' or bool_[0]=='!=':
        if type(bool_[1])==str and type(bool_[2])!=str:
            bool_[1]=help(bool_[1],variable_value)
            if ops[bool_[0]](bool_[1],bool_[2]):
                return True
            else:
                return False
        if type(bool_[2])==str and type(bool_[1])!=str :
            bool_[2]=help(bool_[2],variable_value)
            if ops[bool_[0]](bool_[1],bool_[2]):
                return True
            else:
                return False
        if type(bool_[1])==str and type(bool_[2])==str:
            if bool_[1] in variable:
                bool_[1]=help(bool_[1],variable_value)
            if bool_[2] in variable:
                bool_[2]=help(bool_[2],variable_value)
            if ops[bool_[0]](bool_[1],bool_[2]):
                return True
            else:
                return False
        if type(bool_[1])!=str and type(bool_[2])!=str:
            if ops[bool_[0]](bool_[1],bool_[2]):
                return True
            else:
                return False
    if bool_[0]=='and':
        for i in range(1,len(bool_)):
            if i<len(bool_)-1 and true_false(bool_[i],variable_value):
                continue
            if i<len(bool_)-1 and not true_false(bool_[i],variable_value):
                return False
                break
            if i==len(bool_)-1 and true_false(bool_[i],variable_value):
                return True
            if i==len(bool_)-1 and not true_false(bool_[i],variable_value):
                return False
    if bool_[0]=='or':
        for i in range(1,len(bool_)):
            if i<len(bool_)-1 and not true_false(bool_[i],variable_value):
                continue
            if i<len(bool_)-1 and  true_false(bool_[i],variable_value):
                return True
                break
            if i==len(bool_)-1 and true_false(bool_[i],variable_value):
                return True
            if i==len(bool_)-1 and not true_false(bool_[i],variable_value):
                return False
def help(string,variable_value):
    for i in range(0,len(variable_value)):
        if variable_value[i][0]==string:
            return variable_value[i][1]
