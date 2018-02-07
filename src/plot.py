import matplotlib.pyplot as plt

def read_data(fileName):
    with open(fileName) as f:
        content = f.readlines()
    content = [x.strip() for x in content]
    return content

def get_coordinates(data, x, y):
    x = []
    y = []
    for k in  data:
        ll =  k.split()
        x.append(ll[0])
        y.append(ll[1])
    return x, y

#rkct = 'rkdct.txt'
#kdct = 'kdct.txt'
#pkct = 'pkct.txt'
#rk = read_data(rkct)
#kd = read_data(kdct)
#pk = read_data(pkct)

#xkd, ykd = get_coordinates(kd)
#xrk, yrk = get_coordinates(rk)
#xpk, ypk = get_coordinates(pk)

k_param = "pk_result_sift_imax_performance.txt"
data = read_data(k_param)
x , y = get_coordinates(data, 0, 1)

kmean_time = "pk_sift_imax_precision.txt"
data = read_data(kmean_time)
xx, yy = get_coordinates(data, 0, 1)

plt.plot(x, y, 'r-')
#plt.plot(xrk, yrk, 'b-', xkd, ykd, 'r-', xpk, ypk, 'g-')

plt.ylabel('time');
plt.xlabel('number of iterations')


plt1 = plt.twinx()
plt1.plot(xx, yy, 'y-');
plt.ylabel('precision')

plt.show()
