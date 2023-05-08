import datetime
import os
from os import listdir
from os.path import isfile, join
import sys
from cgi_utils import print_error_page, get_formatted_date

try:
    # Open template for showing directory content
    path = os.getenv("PATH_TRANSLATED")

    # If PATH_TRANSLATED is not a valid path, return 404
    if (not os.path.exists(path)):
        print_error_page("404", "Not Found")
        sys.exit(0)

    # Compute list with href for each file
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

    # Open gallery template file
    with open('./public/gallery/index.html', 'r') as file:
        gallery_template = file.read()

    # Inject formatted pictures list
    gallery_template = gallery_template.replace(
        '{{pictures}}', formatted_pictures_list)

    print("HTTP/1.1 200 OK\r")

    print("Content-Type: text/html\r")
    # Content length does not work in this case
    # Some extra whitespaces are being appended automatically in the response
    # len(gallery_template) is actually less than the actual length of the response
    # using it will cause the response to be truncated
    print("Date: " + str(get_formatted_date()) + "\r")
    print("Server: " + os.getenv("SERVER_SOFTWARE") + "\r")

    print("\r")
    print(gallery_template)
except:
    print_error_page("500", "Internal Server Error")
