import os
from os import listdir
from os.path import isfile, join
import json

print('\n')

# Open template for showing directory content
path = os.getenv("PATH_INFO")

# Get href for each file
pictures_href_list = [
    f'{path.removeprefix("public/")}/{f}' for f in listdir(path) if isfile(join(path, f))]

# Format pictures list as HTML
formatted_pictures = map(
    lambda x: f'<li><a href="{x}"><img src="{x}"/></a><button type="button" class="delete-button" data-value="{x}">🗑️</button></li>', pictures_href_list)

# Add parent UL tag
formatted_pictures_list = f'<ul>{"".join(formatted_pictures)}</ul>'

# Open template for gallery
with open('./public/gallery/index.html', 'r') as file:
    gallery_template = file.read()

# Inject formatted pictures list
gallery_template = gallery_template.replace(
    '{{pictures}}', formatted_pictures_list)

print(gallery_template)
