from cv2 import *

def calculateOrnt(dx, dy):
    ornt = dx
    rows,cols = dx.shape
    for i in range(0, rows):
        for j in range(0, cols):
            x = dx[i,j]
            y = dy[i,j]
            ornt[i,j] = fastAtan2(x, y)
    return ornt

def cvtToOrntImg(src):
    dx = Sobel(src, CV_32F, 1, 0, 3)
    dy = Sobel(src, CV_32F, 0, 1, 3)

    ornt_src = calculateOrnt(dx, dy)
    return ornt_src

def maxOf(a, b):
    if(a > b):
        return a
    else:
        return b

def minOf(a, b):
    if(a < b):
        return a
    else:
        return b
