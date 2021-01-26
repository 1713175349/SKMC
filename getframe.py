import numpy as np

atoms = []
with open("C_mp-990448_primitive.car", "r") as fp:
    for i in fp:
        a = i.split()
        if len(a) < 1 or a[0] == "end":
            break
        atoms.append([float(a[1]), float(a[2]), float(a[3])])

fp = open("out.dat", "w")
atoms = np.array(atoms)
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

fp.close()
