import cv2 as cv
import numpy as np
import pyautogui
import matplotlib.pyplot as plt
import matplotlib.image as mpimg
import math


def grayscale(img):

    return cv.cvtColor(img, cv.COLOR_RGB2GRAY)
    # Or use BGR2GRAY if you read an image with cv2.imread()
    # return cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)


def canny(img, low_threshold, high_threshold):
    """Applies the Canny transform"""
    return cv.Canny(img, low_threshold, high_threshold)


def gaussian_blur(img, kernel_size):
    """Applies a Gaussian Noise kernel"""
    return cv.GaussianBlur(img, (kernel_size, kernel_size), 0)


def region_of_interest(img, vertices):
    """
    Applies an image mask.

    Only keeps the region of the image defined by the polygon
    formed from `vertices`. The rest of the image is set to black.
    """
    # defining a blank mask to start with
    mask = np.zeros_like(img)

    # defining a 3 channel or 1 channel color to fill the mask with depending on the input image
    if len(img.shape) > 2:
        channel_count = img.shape[2]  # i.e. 3 or 4 depending on your image
        ignore_mask_color = (255,) * channel_count
    else:
        ignore_mask_color = 255

    # filling pixels inside the polygon defined by "vertices" with the fill color
    cv.fillPoly(mask, vertices, ignore_mask_color)

    # returning the image only where mask pixels are nonzero
    masked_image = cv.bitwise_and(img, mask)
    return masked_image


def draw_lines(img, lines, color=[0, 0, 255], thickness=2): # 선 그리기
    for line in lines:
        for x1,y1,x2,y2 in line:
            cv.line(img, (x1, y1), (x2, y2), color, thickness)

    # creating a new 2d array with means

    # Drawing the lines


def hough_lines(img, rho, theta, threshold, min_line_len, max_line_gap):
    lines = cv.HoughLinesP(img, rho, theta, threshold, np.array([]), minLineLength=min_line_len, maxLineGap=max_line_gap)
    line_img = np.zeros((img.shape[0], img.shape[1], 3), dtype=np.uint8)
    draw_lines(line_img, lines)
    return line_img


def weighted_img(img, initial_img, α=0.8, β=1., λ=0.):

    return cv.addWeighted(initial_img, α, img, β, λ)


rho = 3
theta = np.pi / 180
threshold = 15
min_line_len = 150
max_line_gap = 50

cv.namedWindow("result")
cv.moveWindow("result", 0, 500)


while 1:
    pic = pyautogui.screenshot(region=(0, 0, 960, 540))
    img_frame = np.array(pic)
    img_frame = cv.cvtColor(img_frame, cv.COLOR_RGB2BGR)
    meth = 'cv.TM_CCOEFF'
    method = eval(meth)


    gray = grayscale(img_frame)
    kernel_size = 5  # Kernel size
    blur_gray = gaussian_blur(gray, kernel_size)
    low_threshold = 50
    high_threshold = 150
    edges = canny(blur_gray, low_threshold, high_threshold)
    vertices = np.array([[(0, img_frame.shape[0]), (450, 315), (490, 315),
                          (img_frame.shape[1], img_frame.shape[0])]], dtype=np.int32)
    mask = region_of_interest(img_frame, vertices)
    masked_edges = region_of_interest(edges, vertices)
    lines = hough_lines(masked_edges, rho, theta, threshold, min_line_len, max_line_gap)
    lines_edges = weighted_img(lines, img_frame, α=0.8, β=1., λ=0.)


    cv.imshow('result',lines_edges)

    key = cv.waitKey(1)
    if key == 27:
        break

