import os
import sys

print("\n")

path_info = os.getenv("PATH_INFO")
query_string = os.getenv("QUERY_STRING")

file_name = query_string[query_string.find("file=") + 5:query_string.find("&")]

file_path = path_info + "/" + file_name
if (not os.path.isfile(file_path)):
    with open('./public/error/404.html', 'r') as file:
        error_template = file.read() 
    print(error_template)
    sys.exit() 

new_name = query_string[query_string.find("name=") + 5:]
new_file_path = path_info + "/" + new_name

os.rename(file_path, new_file_path)

with open('./public/gallery/rename/success.html', 'r') as file:
    success_template = file.read() 
    print(success_template)
