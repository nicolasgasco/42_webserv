import os
from os import listdir
import sys

try:
    # Get all files in PATH_INFO directory
    dir_path = os.getenv("PATH_TRANSLATED")
    if not os.path.exists(dir_path):
        with open('./public/error/default.html', 'r') as file:
            error_page = file.read()
            error_page = error_page.replace("{{code}}", "404")
            error_page = error_page.replace("{{message}}", "Not Found")

        print("HTTP/1.1 404 Not Found\r")
        print("Content-Length: " + str(len(error_page)) + "\r")
        print("Content-Type: text/html\r")
        print("\r")
        print(error_page)
        sys.exit(0)

    files_in_dir = [f for f in listdir(dir_path)]

    def format_directory(dir):
        '''For folders, / must be appended so that a correct request
        can be made when you click on it in the browser'''
        if dir.find(".") == -1:
            return f'{dir}/'
        else:
            return dir

    # Format each file in list for HTML template
    public_dir_path = dir_path.replace("./public", "")
    formatted_files_in_dir = list(
        map(lambda item: f'<li><a href="{public_dir_path}{format_directory(item)}">{format_directory(item)}</a></li>', files_in_dir))

    # Open template for showing directory content
    with open('./public/dir/index.html', 'r') as file:
        dir_content_template = file.read()

    # Inject directory list in template
    dir_content_template = dir_content_template.replace(
        '{{dir_content}}', '\n'.join(formatted_files_in_dir))

    print("HTTP/1.1 200 OK\r")
    print("Content-Type: text/html\r")
    print("Content-Length: " + str(len(dir_content_template)) + "\r")
    print("\r")
    print(dir_content_template)

except:
    with open('./public/error/default.html', 'r') as file:
        error_page = file.read()
        error_page = error_page.replace("{{code}}", "500")
        error_page = error_page.replace("{{message}}", "Internal Server Error")

    print("HTTP/1.1 500 Internal Server Error\r")
    print("Content-Length: " + str(len(error_page)) + "\r")
    print("Content-Type: text/html\r")
    print("\r")
    print(error_page)
