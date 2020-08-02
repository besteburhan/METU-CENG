import math

def euclidean_distance(coord1, coord2):
    x1, y1, z1 = coord1
    x2, y2, z2 = coord2
    return math.sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2) + pow(z1 - z2, 2))

def is_interaction_hot_spot(first, second, it_first, it_second):
    if euclidean_distance(first['coord'], it_first['coord']) < 8 or euclidean_distance(first['coord'], it_second['coord']) < 8 or euclidean_distance(second['coord'], it_first['coord']) < 8 or euclidean_distance(second['coord'], it_second['coord']) < 8:
        return True

def group_interacting_pairs(interacting_amino_acids):
    group_counts = { 1 : 1,}
    interacting_amino_acids[0][0]['group'] = 1
    for (first, second) in interacting_amino_acids:
        try:
            group_number = first['group']
            for (it_first, it_second) in interacting_amino_acids:
                try:
                    it_group_number = it_first['group']
                    if it_group_number > group_number and is_interaction_hot_spot(first, second, it_first, it_second):
                        group_counts[it_first['group']] -=1
                        group_counts[first['group']] += 1
                        if group_counts[it_first['group']] == 1:
                            del group_counts[it_first['group']]
                        it_first['group'] = first['group']
                    elif it_group_number < group_number and is_interaction_hot_spot(first, second, it_first, it_second):
                        group_counts[it_group_number] += 1
                        group_counts[group_number] -= 1
                        if group_counts[group_number] == 0:
                            del group_counts[group_number]
                        group_number = it_group_number
                        first['group'] = group_number

                except:
                    if is_interaction_hot_spot(first, second, it_first, it_second):
                        group_counts[first['group']] += 1
                        it_first['group'] = first['group']
        except:
            group_number = len(group_counts)+1
            group_counts[group_number] = 1
            first['group'] = group_number
            for (it_first, it_second) in interacting_amino_acids:
                try:
                    it_group_number = it_first['group']
                    if it_group_number < group_number and is_interaction_hot_spot(first, second, it_first, it_second):
                        group_counts[group_number] -= 1
                        if group_counts[group_number] == 0:
                            del group_counts[group_number]
                        group_number = it_group_number
                        first['group'] = group_number
                        group_counts[first['group']] += 1
                    elif it_group_number > group_number and is_interaction_hot_spot(first, second, it_first, it_second):
                        group_counts[group_number] += 1
                        group_counts[it_group_number] -= 1
                        if group_counts[it_group_number] == 0:
                            del group_counts[it_group_number]
                        it_first['group'] = group_number
                        
                except:
                    if is_interaction_hot_spot(first, second, it_first, it_second):
                        group_counts[first['group']] += 1
                        it_first['group'] = first['group']
    return group_counts, interacting_amino_acids 

def find_interactions(chain_a, chain_b):
    interacting_amino_acids = []
    for aacid_a in chain_a:
        for aacid_b in chain_b:
            if euclidean_distance(aacid_a['coord'], aacid_b['coord']) < 8:
                interacting_amino_acids.append((aacid_a, aacid_b))
    return interacting_amino_acids

def parse_protein(file):
    chain_a = []  # like {id: 54 ,coord:(x,y,z), amino:"ABC"}}
    chain_b = []  # like {id: 99 ,coord:(x,y,z), amino:"DEF"}
    with open(file) as fd:
        for line in fd:
            if line[0:6] == "ATOM  " and ((line[17:20]=="GLY" and line[13:15]=="CA") or (line[17:20]!="GLY" and line[13:15]=="CB")):
                if line[21] == "A":
                    chain_a.append({
                        "id": int(line[23:26]), 
                        "coord": (float(line[30:38]), float(line[38:46]), float(line[46:54])), 
                        "amino":line[17:20]})
                elif line[21] == "B":
                    chain_b.append({
                        "id": int(line[23:26]), 
                        "coord": (float(line[30:38]), float(line[38:46]), float(line[46:54])), 
                        "amino":line[17:20]})
    return chain_a, chain_b

if __name__ == "__main__":
    protein_files = ["4uap.pdb", "2bti.pdb", "3r0a.pdb","6k62.pdb", "6k97.pdb", "6lac.pdb"]
    for file in protein_files:
        chain_a, chain_b = parse_protein(file)
        interacting_amino_acids = find_interactions(chain_a, chain_b)
        group_counts , interacting_amino_acids = group_interacting_pairs(interacting_amino_acids)

        with open("output_"+file[:-3]+  "txt", 'w') as f:
            f.write("There are " + str(len(interacting_amino_acids)) + " interacting pairs.\n")
            for a, b in interacting_amino_acids:
                f.write("Group "+ str(a['group']) + ": "+ a['amino']+ "(" + str(a['id']) +")-"+ b['amino']+ "(" + str(b['id']) + ")\n")
            f.write("Number of groups = "+ str(len(group_counts)))
        
        with open("report.txt", 'a') as f:
            f.write("\t" + file + "\n" + "-"*30 + "\n")
            f.write("There are " + str(len(interacting_amino_acids)) + " interacting pairs.\n")
            for a, b in interacting_amino_acids:
                f.write("Group "+ str(a['group']) + ": "+ a['amino']+ "(" + str(a['id']) +")-"+ b['amino']+ "(" + str(b['id']) + ")\n")
            f.write("Number of groups = "+ str(len(group_counts)) + "\n\n")