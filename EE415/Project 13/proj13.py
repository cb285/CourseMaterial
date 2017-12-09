#!/usr/bin/env python

# import libraries
import cv2
import numpy as np

# define pi
pi = 3.14159265359

# define function to set image values to range [0,1]
def norm(img):
    img_min = img.min()
    img_max = img.max()
    return (img - img_min)/(img_max - img_min)

# read original image
a = cv2.imread('Fig0646(a)(lenna_original_RGB).tif')

# get each color channel
a_red = a[:,:,2].astype("float")
a_green = a[:,:,1].astype("float")
a_blue = a[:,:,0].astype("float")

# define sobel masks
mx = np.array([[-1, -2, -1],
              [ 0,  0,  0],
              [ 1,  2,  1]], dtype="float");

my = np.array([[-1,  0,  1],
              [ -2,  0,  2],
              [ -1,  0,  1]], dtype="float");

# convolve with masks
red_dx = cv2.filter2D(a_red, -1, mx)
red_dy = cv2.filter2D(a_red, -1, my)
green_dx = cv2.filter2D(a_green, -1, my)
green_dy = cv2.filter2D(a_green, -1, my)
blue_dx = cv2.filter2D(a_blue, -1, my)
blue_dy = cv2.filter2D(a_blue, -1, my)

# finish algorithm
gxx = red_dx**2 + green_dx**2 + blue_dx**2
gyy = red_dy**2 + green_dy**2 + blue_dy**2
gxy = red_dx*red_dy + green_dx*green_dy + blue_dx*blue_dy

theta = 0.5*np.arctan2(2*gxy, (gxx-gyy));

f1 = (np.abs(0.5*((gxx + gyy) + (gxx - gyy)*np.cos(2*theta) + 2*gxy*np.sin(2*theta))))**0.5;
f2 = (np.abs(0.5*((gxx + gyy) + (gxx - gyy)*np.cos(2*(theta+pi/2)) + 2*gxy*np.sin(2*(theta+pi/2)))))**0.5;

b = np.maximum(f1, f2)
b_uint8 = (255*norm(b)).astype("uint8")

# show original
cv2.imshow('a', a)
# wait for key press to exit (required)
cv2.waitKey()
# show result
cv2.imshow('b', b_uint8)
# wait for key press to exit (required)
cv2.waitKey()

# write to files
cv2.imwrite("img_a.png", a)
cv2.imwrite("img_b.png", b_uint8)
