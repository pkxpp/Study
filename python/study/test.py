import cv2
import numpy as np

# Load image
img = cv2.imread('VCG41N1210205351.jpg')

# Add text to image
font = cv2.FONT_HERSHEY_SIMPLEX
cv2.putText(img, 'Hello, World!', (50, 50), font, 1, (255, 255, 255), 2, cv2.LINE_AA)

# Display image
cv2.imshow('image', img)
cv2.waitKey(0)
cv2.destroyAllWindows()