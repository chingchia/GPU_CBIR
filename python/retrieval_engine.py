from cv2 import *
import sys
import numpy as np
import os
from edgel_algorithm import *



print "Reading edgel_dic.txt"

input = open('edgel_dic_simple.txt', 'r')
edgel_dic = dict()
for columns in ( row.strip().split() for row in input ):  
    key = int(columns[0])
    docList = list()
    for docID in columns[1:]:
        docList.append(int(docID))
    edgel_dic[key] = docList
    
edgel_counter = dict()

print "done!"

flag = True
while(flag):
    flag = False
    print "Waiting for sketch"
    skch = imread('skch.png', CV_LOAD_IMAGE_GRAYSCALE)
    namedWindow('sketch')
    imshow('sketch', skch)
    waitKey(0)
    
    print "Start computing scores"
    score = computeDocScore(skch, edgel_dic, edgel_counter)
    

destroyAllWindows()  
