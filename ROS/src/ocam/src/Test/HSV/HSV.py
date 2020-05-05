import numpy as np
import cv2

print("Hola perros")
cap = cv2.VideoCapture(2)

while(True):
    # Capture frame-by-frame
    ret, frame = cap.read()

    #! Covert RGB -> HSV
    hsvImage = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)

    #! Display original image
    cv2.imshow('Original', frame)
    #!Display HSV image
    cv2.imshow('Imagen HSV', hsvImage)

    #! Display HSV in 3 channels
    cv2.imshow('Hue', hsvImage[:, :, 0])
    cv2.imshow('Saturation', hsvImage[:, :, 1])
    cv2.imshow('Value', hsvImage[:, :, 2])

    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

# When everything done, release the capture
cap.release()
cv2.destroyAllWindows()
