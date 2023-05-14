import os
from os import listdir
import sys
from cgi_utils import print_error_page, print_template


def format_directory(dir):
    '''For folders, / must be appended so that a correct request
    can be made when you click on it in the browser'''
    if dir.find(".") == -1:
        return f'{dir}/'
    else:
        return dir


try:
    # Get all files in PATH_INFO directory
    dir_path = os.getenv("PATH_TRANSLATED")
    public_dir_path = dir_path.replace("./public", "")
    if not public_dir_path.endswith("/"):
        public_dir_path += "/"

    if not os.path.exists(dir_path):
        print_error_page("404", "Not Found")
        sys.exit(0)

    files_in_dir = [f for f in listdir(dir_path)]

    # Format each file in list for HTML template
    formatted_files_in_dir = list(
        map(lambda item: f'<li><a href="{public_dir_path}{format_directory(item)}">{format_directory(item)}</a></li>', files_in_dir))

    # Open template for showing directory content
    with open('./public/dir/index.html', 'r') as file:
        dir_content_template = file.read()

    # Inject directory list in template
    dir_content_template = dir_content_template.replace(
        '{{dir_content}}', '\n'.join(formatted_files_in_dir))

    print_template(dir_content_template)

except:
    print_error_page("500", "Internal Server Error")
