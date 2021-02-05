import numpy as np


def read_car(fname):
    atoms = []
    eles = []
    l = 1
    with open(fname, "r") as fp:
        for i in fp:
            if l <= 5:
                l += 1
                continue
            a = i.split()
            if len(a) < 1 or a[0] == "end":
                break
            atoms.append([float(a[1]), float(a[2]), float(a[3])])
            eles.append(a[7])
    return np.array(atoms), eles


def get_state_sequence(fname, atoms, eles):
    myatoms, myeles = read_car(fname)
    er = 0.01
    A = np.zeros(len(myeles), dtype=np.int)
    for i in range(len(eles)):
        for j in range(len(eles)):   # 遍历myatoms
            if np.sqrt(np.sum((atoms[i]-myatoms[j])**2)) < er:
                if myeles[j] == "H":
                    A[i] = 0
                elif myeles[j] == "C":
                    A[i] = 1
                elif myeles[j] == "F":
                    A[i] = -1
                break
    return A


atoms1, eles = read_car("C_mp-990448_primitive.car")
print(eles)
fp = open("out.dat", "w")
fp.write("#结构框架 id Position(绝对坐标) type neigborsNumber neigbors  第一个坐标必须是0,0,0\n")
fp.write(str(len(eles))+"\n")
atoms = np.array(atoms1)
cent = atoms[0]
for i in range(atoms.shape[0]):
    neb = []
    for j in range(atoms.shape[0]):
        if (i == j):
            continue
        if np.sqrt(np.sum((atoms[i]-atoms[j])**2)) < 1.45:
            neb.append(j)
    now = atoms[i] - cent
    fp.write("{id:d} {x:f} {y:f} {z:f} 0 {nebnum:d} ".format(id=i, x=now[0], y=now[1], z=now[2], nebnum=len(neb))+" ".join(list(map(lambda x:str(x),neb)))+"\n")


def event_g(s,t):    
    s1=get_state_sequence("./source/"+s, atoms1, eles)
    t1=get_state_sequence("./target/"+t, atoms1, eles)
    string1 = " ".join(list(map(lambda x:str(x),s1.tolist())))
    string2 = " ".join(list(map(lambda x:str(x),t1.tolist())))
    return string1+" | "+string2+" #"+s

def diff_event_g(s,t):    
    s1=get_state_sequence("./source/"+s, atoms1, eles)
    t1=get_state_sequence("./target/"+t, atoms1, eles)
    sn=""
    for i in range(len(s1)):
        if s1[i] != t1[i]:
            sn += (str(i)+" ")
    return sn+" #"+s+""

s='''zigzag-kink-propagrators1.car
amarchair-kinks1.car
amarchair-kinks2.car
amarchair-kinks3.car
amarchair-kinks4.car
amarchair-s1.car
amarchair-s2.car
danjinlinxifus1.car
zigzag-kinks2.car
zigzag-kinks3.car
zigzag-kinks-pros2.car
zigzag-s1.car'''

t='''zigzag-kink-propagratort1.car
amarchair-kinkt1.car
amarchair-kinkt2.car
amarchair-kinkt3.car
amarchair-kinkt4.car
amarchair-t1.car
amarchair-t2.car
danjinlinxifut1.car
zigzag-kinkt2.car
zigzag-kinkt3.car
zigzag-kink-prot2.car
zigzag-t1.car'''

st=list(zip(s.split(),t.split()))

fp.write("\n")
index=0
for i,v in st:
    fp.write(event_g(i,v)+"  "+str(index)+"\n")
    print(diff_event_g(i,v)+"  "+str(index)+"\n")
    index += 1
fp.close()
