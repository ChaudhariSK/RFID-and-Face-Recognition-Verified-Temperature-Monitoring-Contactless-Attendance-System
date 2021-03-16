import cv2

video = cv2.VideoCapture(0)

# load "haarcascade_frontalface_default.xml" by creating a CascadeClassifier
# object as cascade
cascade = cv2.CascadeClassifier("haarcascade_frontalface_default.xml")

while True:
    check,frame = video.read()
    
    # Image from webacm is in the format of BGR i.e combination of 3 colours
    # which will basicall require more amount of computation.
    # so we convert it into a gray scale image which is only single colour
    # and requires less computation.
    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    
    # Now we use detectMultiScale method to detect the faces in the video
    # stream. Which will return x,y,w,h which are basically the positions
    # with which we create a rectangle box.
    face = cascade.detectMultiScale(gray, scaleFactor = 1.1, minNeighbors = 6)

    # using for loop to go through the locations x,y,w,h and drow a rectangle
    for x,y,w,h in face:
        frame = cv2.rectangle(frame, (x,y), (x+w,y+h), (0,255,255), 3)
        
    cv2.imshow("Video",frame)
    key = cv2.waitKey(1)
    if(key == ord('q')):
        break

video.release()
cv2.destroyAllWindows()

