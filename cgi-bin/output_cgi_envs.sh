#!/bin/bash

vars=""

# HTML-formatted output of CGI environment variables
vars+="<li><b>CONTENT_LENGTH</b>=$CONTENT_LENGTH</li>"
vars+="<li><b>CONTENT_TYPE</b>=$CONTENT_TYPE</li>"
vars+="<li><b>GATEWAY_INTERFACE</b>=$GATEWAY_INTERFACE</li>"
vars+="<li><b>PATH_INFO</b>=$PATH_INFO</li>"
vars+="<li><b>PATH_TRANSLATED</b>=$PATH_TRANSLATED</li>"
vars+="<li><b>QUERY_STRING</b>=$QUERY_STRING</li>"
vars+="<li><b>REMOTE_ADDR</b>=$REMOTE_ADDR</li>"
vars+="<li><b>REQUEST_METHOD</b>=$REQUEST_METHOD</li>"
vars+="<li><b>SCRIPT_NAME</b>=$SCRIPT_NAME</li>"
if [[ $SERVER_NAME ]]; then
    vars+="<li><b>SERVER_NAME</b>=$SERVER_NAME</li>"
fi
vars+="<li><b>SERVER_PORT</b>=$SERVER_PORT</li>"
vars+="<li><b>SERVER_PROTOCOL</b>=$SERVER_PROTOCOL</li>"
vars+="<li><b>SERVER_SOFTWARE</b>=$SERVER_SOFTWARE</li>"

# Read in index.html and replace {{vars}} with $vars
page=""
while IFS= read -r line; do
    if [[ $line == *"{{vars}}"* ]]; then
        page+=$vars
    else
        page+=$line
    fi
done < public/env/index.html

# Output page
printf "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n%s" "$page"