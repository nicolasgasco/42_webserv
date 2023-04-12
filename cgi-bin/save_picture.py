import cgi
import os
import os.path
import sys

try:
    form = cgi.FieldStorage()

    # Get filename here
    fileitem = form['image']
except:
    with open('./public/error/default.html', 'r') as file:
        error_page = file.read()
    error_page = error_page.replace("{{code}}", "400")
    error_page = error_page.replace("{{message}}", "Bad Request")

    print("HTTP/1.1 400 Bad Request\r")
    print("Content-Length: " + str(len(error_page)) + "\r")
    print("Content-Type: text/html\r")
    print("\r")
    print(error_page)

try:
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

                print("HTTP/1.1 409 Conflict\r")
                print("Content-Length: " + str(len(error_page)) + "\r")
                print("Content-Type: text/html\r")
                print("\r\n")
                print(error_page)
            sys.exit(0)

        open(file_path, 'wb').write(fileitem.file.read())
        with open('./public/gallery/upload/success.html', 'r') as file:
            success_page = file.read()
            print("HTTP/1.1 200 OK\r")
            print("Content-Length: " + str(len(success_page)) + "\r")
            print("Content-Type: text/html\r")
            print("\r")
            print(success_page)
    else:
        with open('./public/error/default.html', 'r') as file:
            error_page = file.read()
        error_page = error_page.replace("{{code}}", "400")
        error_page = error_page.replace("{{message}}", "Bad Request")

        print("HTTP/1.1 400 Bad Request\r")
        print("Content-Length: " + str(len(error_page)) + "\r")
        print("Content-Type: text/html\r")
        print("\r")
        print(error_page)
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
