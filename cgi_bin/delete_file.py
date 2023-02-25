import os
import json

print("\n")

path = os.getenv("PATH_INFO")

try:
    os.remove(path)
    result = {"result": "Success", "status": 200}
except OSError:
    result = {"result": "File does not exist", "status": 404}

print(json.dumps(result, indent=4))
