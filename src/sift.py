import cv2
import numpy as np
from os import listdir
from os.path import isfile, join
mypath = './pic'
onlyfiles = [f for f in listdir(mypath) if isfile(join(mypath, f))]

i = 0
for o in onlyfiles:
    img = cv2.imread(mypath + '/' + o)
    gray= cv2.cvtColor(img,cv2.COLOR_BGR2GRAY)

    sift = cv2.xfeatures2d.SIFT_create()
    kp, des = sift.detectAndCompute(gray,None)
    m, d = des.shape
    with open(str(i) + '.txt', 'w') as f:
        f.write(str(m) + ' ' + str(d) + '\n')
        for e in des:
            for ee in e:
                f.write(str(ee) + ' ')
            f.write('\n')
    i = i + 1
