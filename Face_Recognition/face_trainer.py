# import the required libraries
import cv2
import os
import numpy as np
from PIL import Image
import pickle


cascade = cv2.CascadeClassifier("haarcascade_frontalface_default.xml")

recognise = cv2.face.LBPHFaceRecognizer_create()

# Created a function 
def getdata():

    current_id = 50
    label_id = {} #dictionanary
    face_train = [] # list
    face_label = [] # list
    
    # Finding the path of the base directory i.e path were this file is placed
    BASE_DIR = os.path.dirname(os.path.abspath(__file__))

    # We have created "image_data" folder that contains the data so basically
    # we are appending its path to the base path
    my_face_dir = os.path.join(BASE_DIR,'image_data')

    # Finding all the folders and files inside the "image_data" folder
    for root, dirs, files in os.walk(my_face_dir):
        for file in files:

            # Checking if the file has extention ".png" or ".jpg"
            if file.endswith("png") or file.endswith("jpg"):

                # Adding the path of the file with the base path
                # so you basically have the path of the image 
                path = os.path.join(root, file)

                # Taking the name of the folder as label i.e his/her name
                label = os.path.basename(root).lower()

                # providing label ID as 1 or 2 and so on for different persons
                if not label in label_id:
                    label_id[label] = current_id
                    current_id += 1
                ID = label_id[label]

                # converting the image into gray scale image
                # you can also use cv2 library for this action
                pil_image = Image.open(path).convert("L")

                # converting the image data into numpy array
                image_array = np.array(pil_image, "uint8")
        
                # identifying the faces
                face = cascade.detectMultiScale(image_array)

                # finding the Region of Interest and appending the data
                for x,y,w,h in face:
                    img = image_array[y:y+h, x:x+w]
                #image_array = cv2.rectangle(image_array,(x,y),(x+w,y+h),(255,255,255),3)
                    cv2.imshow("Test",img)
                    cv2.waitKey(1)
                    face_train.append(img)
                    face_label.append(ID)

    # string the labels data into a file
    with open("labels.pickle", 'wb') as f:
        pickle.dump(label_id, f)
   

    return face_train,face_label

# creating ".yml" file
face,ids = getdata()
recognise.train(face, np.array(ids))
recognise.save("trainner.yml")
