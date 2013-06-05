from edgel_helper_func import *

def computeDocScore(skch, edgel_dic, edgel_counter):
    score = list()
    ornt_skch = cvtToOrntImg(skch)

    rows, cols = ornt_skch.shape
    TOL = 2
    
    for x in range(0, rows):
        for y in range(0, cols):
            val = ornt_skch[x, y]
            if(val != 0.0):
                for rx in range(maxOf(0, x - TOL), minOf(rows, x + TOL + 1)):
                    for ry in range(maxOf(0, y - TOL), minOf(cols, y + TOL + 1)):
                                    
                        
                                    
                
            
    
    return score
