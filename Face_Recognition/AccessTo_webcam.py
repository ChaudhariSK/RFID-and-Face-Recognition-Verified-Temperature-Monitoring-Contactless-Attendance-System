# import opencv library
import cv2

# Create a VideoCapture() object as "video"
video = cv2.VideoCapture(0)

# Enter into an infinite while loop
while True:
    # Data of 1 frame is read every time when this while loop is repeated
    # into the variable frame
    # check contains the boolean value i.e True or False if its True then
    # webcam is active
    check,frame = video.read()

    # imshow is a method of cv2 library which will basically show the image
    # or frame on a new window
    cv2.imshow("Video",frame)

    # Creates a delay of 1 mili-second and stores the value to variable key
    # if any key is pressed on keyboard
    key = cv2.waitKey(1)

    # check if key is equal to 'q' if it is then break out of the loop
    if(key == ord('q')):
        break

# Release the webcam. in other words turn it of
video.release()

# Destroys all the windows which were created
cv2.destroyAllWindows()
