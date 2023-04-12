import os
from os import listdir
from os.path import isfile, join
import sys

try:
    # Open template for showing directory content
    path = os.getenv("PATH_TRANSLATED")

    if (not os.path.exists(path)):
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

    # Get href for each file
    pictures_href_list = [
        f'{path.replace("public/", "")}/{f}' for f in listdir(path) if isfile(join(path, f))]

    if len(pictures_href_list) == 0:
        formatted_pictures_list = "<div class='container centered'><img src='assets/error/empty.png' alt='Pictures list is empty' width='900' /></div>"
    else:
        # Format pictures list as HTML
        formatted_pictures = map(
            lambda x: f'<li><a href="{x}"><img src="{x}"/></a><button type="button" class="delete-button" data-value="{x}">🗑️</button></li>', pictures_href_list)

        # Add parent UL tag
        formatted_pictures_list = f'<div id="pictures-container"><ul>{"".join(formatted_pictures)}</ul></div>'

    # Open template for gallery
    with open('./public/gallery/index.html', 'r') as file:
        gallery_template = file.read()

    # Inject formatted pictures list
    gallery_template = gallery_template.replace(
        '{{pictures}}', formatted_pictures_list)

    print("HTTP/1.1 200 OK\r")
    # Content length is leading to content being truncated
    print("Content-Type: text/html\r")
    print("\r")
    print(gallery_template)

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
