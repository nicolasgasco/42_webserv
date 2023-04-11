import os
import json

print("\r\n")

path = os.getenv("PATH_TRANSLATED")

try:
    os.remove(path)
    result = {"result": "Success", "status": 200, "path": path}

    print("HTTP/1.1 200 OK\r")
    print("Content-Type: application/json\r")
    print("Content-Length: " + str(len(json.dumps(result, indent=4))) + "\r")
    print("\r")

    print(json.dumps(result, indent=4))

except OSError:
    result = {"result": "File does not exist", "status": 404, "path": path}
    print("HTTP/1.1 404 Not Found\r")
    print("Content-Type: application/json\r")
    print("Content-Length: " + str(len(json.dumps(result, indent=4))) + "\r")
    print("\r")
