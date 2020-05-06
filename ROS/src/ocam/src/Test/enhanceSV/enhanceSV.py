import numpy as np
import cv2 as cv
from matplotlib import pyplot as plt

#! Read Image
img = cv.imread('test.jpg')
cv.imshow("Original", img)

#!Create new image
new_image = np.zeros(img.shape, img.dtype)

#!Set values
alpha = 1.0  # Simple contrast control
beta = 0    # Simple brightness control


#! Initialize values
print(' Basic Linear Transforms ')
print('-------------------------')
try:
    alpha = float(input('* Enter the alpha value [1.0-3.0]: '))
    beta = int(input('* Enter the beta value [0-100]: '))
except ValueError:
    print('Error, not a number')

# Do the operation new_image(i,j) = alpha*image(i,j) + beta
# Instead of these 'for' loops we could have used simply:
# new_image = cv.convertScaleAbs(image, alpha=alpha, beta=beta)
# but we wanted to show you how to access the pixels :)
for y in range(img.shape[0]):
    for x in range(img.shape[1]):
        for c in range(img.shape[2]):
            new_image[y, x, c] = np.clip(alpha*img[y, x, c] + beta, 0, 255)

gray = cv.cvtColor(new_image, cv.COLOR_BGR2GRAY)
img_gray = cv.cvtColor(img, cv.COLOR_BGR2GRAY)

#!print new image
cv.imshow('New Image', new_image)

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
plt.hist(gray.ravel(), 256, [0, 256], facecolor='g')
plt.xlabel('Bits (8bits)')
plt.ylabel('Pixels')
plt.grid(True)

plt.show()


# Wait until user press some key
cv.waitKey()
