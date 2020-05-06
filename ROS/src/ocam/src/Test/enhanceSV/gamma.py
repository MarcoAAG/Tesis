import cv2 as cv
import numpy as np

image = cv.imread("test.jpg")
gamma = 0.5

# corrected_image = np.power(image, gamma)
lookUpTable = np.empty((1, 256), np.uint8)
for i in range(256):
    lookUpTable[0, i] = np.clip(pow(i / 255.0, gamma) * 255.0, 0, 255)

res = cv.LUT(image, lookUpTable)

cv.imshow('Image', image)
cv.imshow('corrected image', res)
cv.waitKey()
cv.destroyAllWindows
