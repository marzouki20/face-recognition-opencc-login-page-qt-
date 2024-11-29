import cv2
import cx_Oracle
import tkinter as tk
from tkinter import simpledialog
import numpy as np
import io

camera = cv2.VideoCapture(0)

def capture_image():
    ret, frame = camera.read()
    if not ret:
        print("Failed to grab frame")
        return None
    cv2.imshow("Captured Image", frame)
    return frame

def insert_image_to_db(image, name):
    try:
        _, img_encoded = cv2.imencode('.jpg', image)
        img_bytes = img_encoded.tobytes()
        con = cx_Oracle.connect('econnet/123@localhost:1521/XE')
        cursor = con.cursor()
        cursor.execute("""
            INSERT INTO image (name, image) 
            VALUES (:name, :image_data)""", 
            name=name, image_data=img_bytes)
        con.commit()
        print(f"Image '{name}' inserted")
    except cx_Oracle.DatabaseError as e:
        print("There is a problem with Oracle:", e)
    finally:
        if cursor:
            cursor.close()
        if con:
            con.close()

def input_image_name():
    root = tk.Tk()
    root.withdraw()
    name = simpledialog.askstring("Input", "Enter a name for the image:")
    return name

def main():
    image = capture_image()
    if image is None:
        return
    name = input_image_name()
    if name:
        insert_image_to_db(image, name)
    else:
        print("No name entered")
    camera.release()
    cv2.destroyAllWindows()

if __name__ == "__main__":
    main()
