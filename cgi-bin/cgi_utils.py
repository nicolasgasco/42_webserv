import datetime
import os


def print_error_page(error_code, error_message):
    """Prints error page"""
    with open('./public/error/default.html', 'r') as file:
        error_page = file.read()
        error_page = error_page.replace("{{code}}", error_code)
        error_page = error_page.replace("{{message}}", error_message)

    print(f"HTTP/1.1 {error_code} {error_message}\r")

    print("Content-Type: text/html\r")
    print("Content-Length: " + str(len(error_page)) + "\r")
    print("Date: " + str(get_formatted_date()) + "\r")
    print("Server: " + os.getenv("SERVER_SOFTWARE") + "\r")

    print("\r")
    print(error_page)


def get_formatted_date():
    """Returns formatted date"""
    return datetime.datetime.now().strftime("%a, %d %b %G %T GMT")


def print_template(page_template):
    """Prints a specific template"""
    print("HTTP/1.1 200 OK\r")

    # TODO Content length is leading to content being truncated
    print("Content-Type: text/html\r")
    print("Content-Length: " + str(len(page_template)) + "\r")
    print("Date: " + str(get_formatted_date()) + "\r")
    print("Server: " + os.getenv("SERVER_SOFTWARE") + "\r")

    print("\r")
    print(page_template)
