import cgi
import os
import os.path
import sys
from cgi_utils import print_error_page, print_template

try:
    form = cgi.FieldStorage()

    # Get filename here
    fileitem = form['image']
except:
    print_error_page("400", "Bad Request")

try:
    if fileitem.filename.find("png") and fileitem.filename.find("jpg") and fileitem.filename.find("gif"):
        print_error_page("400", "Bad Request")
        sys.exit(0)

    # Test if the file was uploaded
    if fileitem.filename:
        file_path = os.getcwd() + '/public/pictures/' + \
            os.path.basename(fileitem.filename)

        # If file already exists, return 409 Conflict
        if os.path.isfile(file_path):
            print_error_page("409", "Conflict")
            sys.exit(0)

        open(file_path, 'wb').write(fileitem.file.read())

        with open('./public/gallery/upload/success.html', 'r') as file:
            success_page = file.read()
        print_template(success_page)

    else:
        print_error_page("400", "Bad Request")
except:
    print_error_page("500", "Internal Server Error")
