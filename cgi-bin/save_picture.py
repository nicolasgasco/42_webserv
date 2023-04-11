import cgi
import os
import os.path
import sys

form = cgi.FieldStorage()

# Get filename here
fileitem = form['image']

# Test if the file was uploaded
if fileitem.filename:
    file_path = os.getcwd() + '/public/gallery/pictures/' + \
        os.path.basename(fileitem.filename)

    # If file already exists, return 409 Conflict
    if os.path.isfile(file_path):
        with open('./public/error/default.html', 'r') as file:
            error_page = file.read()
            error_page = error_page.replace("{{code}}", "409")
            error_page = error_page.replace("{{message}}", "Conflict")
            print(error_page)
        sys.exit(0)

    open(file_path, 'wb').write(fileitem.file.read())
    with open('./public/gallery/upload/success.html', 'r') as file:
        success_page = file.read()
        print(success_page)
