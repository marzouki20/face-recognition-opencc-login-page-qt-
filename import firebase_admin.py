import bcrypt
import firebase_admin
from firebase_admin import credentials, auth
import cx_Oracle
import cloudinary
import cloudinary.api
import cloudinary.uploader
import requests

# Configure Cloudinary
cloudinary.config(
    cloud_name = 'dkzpvbajq',  # Replace with your Cloudinary cloud name
    api_key = '496337663431744',        # Replace with your Cloudinary API key
    api_secret = '3joBpbTf1gByo0KfXoy1r84zHvU'  # Replace with your Cloudinary API secret
)

# Hash the password
def hash_password(password):
    salt = bcrypt.gensalt()
    hashed = bcrypt.hashpw(password.encode('utf-8'), salt)
    return hashed.decode('utf-8')

# Insert user and image into the Oracle database
def insert_user_and_image_to_oracle(email, hashed_password, image_blob, image_name):
    try:
        con = cx_Oracle.connect('econnet/123@localhost:1521/XE')
        cursor = con.cursor()
        cursor.execute("""
            INSERT INTO etulisateur (adress, mot_de_pass, image, name) 
            VALUES (:email, :password, :image_blob, :image_name)""",
            email=email, password=hashed_password, image_blob=image_blob, image_name=image_name)
        con.commit()
        print(f"User '{email}' with image '{image_name}' inserted successfully.")
    except cx_Oracle.DatabaseError as e:
        print("There is a problem with Oracle:", e)
    finally:
        if cursor:
            cursor.close()
        if con:
            con.close()

# Download image from Cloudinary by Public ID
def download_image_by_public_id(public_id):
    try:
        url = cloudinary.CloudinaryImage(public_id).build_url()
        response = requests.get(url)
        if response.status_code == 200:
            return response.content  # Image content as bytes
        else:
            print(f"Failed to download image '{public_id}': {response.status_code}")
            return None
    except Exception as e:
        print(f"Error downloading image: {e}")
        return None

# Sync Firebase users and Cloudinary images to Oracle
def sync_firebase_users_and_images_to_oracle():
    # Initialize Firebase
    cred = credentials.Certificate("C:/Users/Admin/Downloads/demo1-esprit-firebase-adminsdk-zs77d-d1817a344d.json")  # Update with your file path
    firebase_admin.initialize_app(cred)

    # Get all Firebase users
    users = auth.list_users().iterate_all()
    for user in users:
        email = user.email
        hashed_password = hash_password("placeholder_password")  # Generate a hashed placeholder password
        
        # Extract the first name from the email
        first_name = email.split('@')[0]

        # Use the first name as the Public ID to fetch the image
        image_blob = download_image_by_public_id(first_name)

        if image_blob:
            # Insert user with the image into Oracle
            insert_user_and_image_to_oracle(email, hashed_password, image_blob, first_name)
        else:
            print(f"Image for Public ID '{first_name}' not found or could not be downloaded.")

if __name__ == "__main__":
    sync_firebase_users_and_images_to_oracle()
