# Webserv

## Overview
The project was coded by [dgerwig](https://github.com/diegogerwig) and [ngasco](https://github.com/nicolasgasco) in C++ (98 standard).

Quoting from the project's subject:

> This project is about writing your ow HTTP server. You will be able to test it with an actual browser. HTTP is one of the most used protocols on the internet. Knowing its arcane will be useful, even if you won’t be working on a website.

The server is able to render a static website where users can upload, see, and delete pictures:

<img src="https://github.com/nicolasgasco/42_webserv/assets/73175085/9d6ea6c9-03f5-46cc-800c-7bac6c6977dd" width="750px" />

## Features
- Supports GET, HEAD, POST, and DELETE requests
- Accepts a configuration file (loosely inspired by [nginx](https://www.nginx.com/)) to customize:
    - ports
    - server name
    - 404 error template
    - autoindex on (index template is shown for each route) or off (content of each route is shown instead)
    - maximum body size on POST request
    - accepted methods for each route, e.g. to protect a route from deletion
    - index template for each route
    - alternative name for routes
    - redirection to another template
 - Supports Python and Shell scripts for custom actions like:
    - rendering a Gallery page that adapts to the pictures stored on the server
    - uploading pictures using Python's CGI API
    - rendering a template with the content of a specific route
 - Supports both regular and chunked requests

## Run
You can run the server with the default configuration file, by simply running:
```
make run
```
