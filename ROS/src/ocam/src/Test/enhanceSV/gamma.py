import cv2 as cv
import numpy as np
from matplotlib import pyplot as plt

image = cv.imread("test.jpg")
gamma = 1.5

# corrected_image = np.power(image, gamma)
lookUpTable = np.empty((1, 256), np.uint8)
for i in range(256):
    lookUpTable[0, i] = np.clip(pow(i / 255.0, gamma) * 255.0, 0, 255)

res = cv.LUT(image, lookUpTable)

res_gray = cv.cvtColor(res, cv.COLOR_BGR2GRAY)
img_gray = cv.cvtColor(image, cv.COLOR_BGR2GRAY)

cv.imshow('Image', image)
cv.imshow('corrected image', res)

#!Print histrogram
plt.figure()
plt.subplot(211)

plt.title('Histograma de imagen original')
plt.hist(img_gray.ravel(), 256, [0, 256])
plt.xlabel('Bits (8bits)')
plt.ylabel('Pixels')
plt.grid(True)
# plt.show()

plt.subplot(212)
plt.title('Histograma de imagen corregida')
plt.hist(res_gray.ravel(), 256, [0, 256], facecolor='g')
plt.xlabel('Bits (8bits)')
plt.ylabel('Pixels')
plt.grid(True)

plt.show()


cv.waitKey()
cv.destroyAllWindows
