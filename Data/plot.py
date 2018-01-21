import matplotlib.pyplot as plt

def read_data(fileName):
    with open(fileName) as f:
        content = f.readlines()
    content = [x.strip() for x in content]
    return content

def get_coordinates(data, f, s):
    x = []
    y = []
    for k in  data:
        ll =  k.split()
        x.append(ll[f])
        y.append(ll[s])
    return x, y

rksearch = 'rkd_result_sift_e07_98p.txt'
kdsearch = 'kd_result_sift.txt'
linear = 'naive_sift.txt'
pksearch = 'pk_result_sift_r05_20p.txt'

rk = read_data(rksearch)
kd = read_data(kdsearch)
pk = read_data(pksearch)
ln = read_data(linear);

xkd, ykd = get_coordinates(kd, 0, 3)
xrk, yrk = get_coordinates(rk, 0, 3)
xpk, ypk = get_coordinates(pk, 0, 3)
xl , yl  = get_coordinates(ln, 0, 3)
plt.plot(xrk, yrk, 'b-', xkd, ykd, 'r-', xpk, ypk, 'g-', xl, yl, 'y-')

plt.ylabel('time in seconds');
plt.xlabel('number of points')
plt.show()
