import os
from os import listdir
from os.path import isfile, join
import json

print('\n')

# Open template for showing directory content
path = os.getenv("PATH_INFO")

files_in_dir = [
    f'{path.removeprefix("public/")}/{f}' for f in listdir(path) if isfile(join(path, f))]

result = {"value": files_in_dir}

print(json.dumps(result, indent=4))
