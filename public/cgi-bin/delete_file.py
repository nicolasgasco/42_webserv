import os
import json

print("\r\n")

path = os.getenv("PATH_TRANSLATED")

try:
    os.remove(path)
    result = {"result": "Success", "status": 200, "path": path}
except OSError:
    result = {"result": "File does not exist", "status": 404, "path": path}

print(json.dumps(result, indent=4))
