import cv2 as cv
import numpy as np
import math
import time
import serial

ser = serial.Serial('/dev/ttyUSB0', 9600, timeout=1)

def control(middle_line):
    '''
    Interact between Raspberry and Arduino by middle_line variable.
    '''

    if (middle_line <= 335) & (middle_line >= 175):
        ser.write(b'a') #moveForward
        print("GO")
    elif middle_line < 175:
        ser.write(b'b') #turnLeft
        prev_dir = 'l'
        print("LEFT")
    elif middle_line > 335:
        ser.write(b'e') #turnRight
        print("RIGHT")
        prev_dir = 'r'
    else:
        ser.write(b'h') #stopped
        print("No line found")

def get_mid_coor(pt_in):
    pt1 = [pt[0] for pt in pt_in]
    pt2 = [pt[1] for pt in pt_in]

    pt1_out = np.average(pt1)
    pt2_out = np.average(pt2)

    return (int(pt1_out), int(pt2_out))

cap = cv.VideoCapture(0)
cap.set(cv.CAP_PROP_FRAME_WIDTH, 640)
cap.set(cv.CAP_PROP_FRAME_HEIGHT, 480)

while True:
    cap.grab()
    cap.grab()
    ret, frame = cap.retrieve()

    # FIND LINES
    blur = cv.GaussianBlur(frame, (5,5), 0)
    canny = cv.Canny(blur, 100, 200)
    hough = cv.HoughLines(canny, # input edges
                        1, # distance res in pixel
                        np.pi / 180, # angle res in rad
                        100, # Min number of votes for valid line
                        None, # Min allowed length of line
                        0, 
                        0
                        )

    pt1_list = []
    pt2_list = []
    
    # HOUGH
    if hough is not None:
        for i in range(0, len(hough)):
            rho = hough[i][0][0]
            theta = hough[i][0][1]

            a = math.cos(theta)
            b = math.sin(theta)

            x0 = a * rho
            y0 = b * rho
            
            pt1 = (int(x0 + 1000*(-b)), int(y0 + 1000*(a)))
            pt1_list.append(pt1)
            pt2 = (int(x0 - 1000*(-b)), int(y0 - 1000*(a)))
            pt2_list.append(pt2)

            # cv.line(frame, pt1, pt2, (0,0,255), 3, cv.LINE_AA)

        # mid point
        mid_pt1 = get_mid_coor(pt1_list)
        print('MID PT1: ', mid_pt1)
        mid_pt2 = get_mid_coor(pt2_list)
        print('MID PT2: ', mid_pt2)

        cv.line(frame, mid_pt1, mid_pt2, (0,255,0), 3, cv.LINE_AA)
    
        Y = (mid_pt1[1] + mid_pt2[1])//2 
        X = (mid_pt1[0] + mid_pt2[0])//2
        
        frame = cv.circle(frame, (X, Y), 20, (255, 0, 0), 10)
        
        print(f"X is: {X}")
        
        control(X)

    if cv.waitKey(1) & 0xFF == ord('q'):
        break

# When everything done, release the video capture object
cap.release()
 
# Closes all the frames
cv.destroyAllWindows()
