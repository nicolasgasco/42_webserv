import os
from os import listdir
from os.path import isfile, join
import json

print('\r\n')

# Open template for showing directory content
path = os.getenv("PATH_TRANSLATED")

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

print(gallery_template)
