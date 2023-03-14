import os
from os import listdir

print('\n')

# Open template for showing directory content
with open('./public/dir/index.html', 'r') as file:
    dir_content_template = file.read()

# Get all files in PATH_INFO directory
dir_path = os.getenv("PATH_INFO")
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


# Inject directory list in template
dir_content_template = dir_content_template.replace(
    '{{dir_content}}', '\n'.join(formatted_files_in_dir))

print(dir_content_template)
