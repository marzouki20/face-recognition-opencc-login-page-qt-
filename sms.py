import pywhatkit as kit
import sys
import pyautogui
import time

# Receive phone number and message from Qt app
phone_number = sys.argv[1]
message = sys.argv[2]

# Send message through WhatsApp Web
try:
    # Use pywhatkit to open WhatsApp and type the message
    kit.sendwhatmsg_instantly(f"+{phone_number}", message, wait_time=10, tab_close=False)
    
    
    time.sleep(5)  

    # Locate the "Send" button and click it
    # You need to have the position of the "Send" button beforehand or use image recognition
    send_button = pyautogui.locateOnScreen('BUTTON.png', confidence=0.9)  # Image of the send button
    if send_button is not None:
        # Get the center of the button and click it
        button_center = pyautogui.center(send_button)
        pyautogui.click(button_center)
        print("Message sent successfully")
    else:
        print("Send button not found.")
        
except Exception as e:
    print(f"Failed to send message: {str(e)}")

