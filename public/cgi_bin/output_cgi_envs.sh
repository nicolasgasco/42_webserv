#!/bin/bash

# HTML-formatted output of CGI environment variables
vars="<li><b>SERVER_PROTOCOL</b>=$SERVER_PROTOCOL</li>"
vars+="<li><b>SERVER_PORT</b>=$SERVER_PORT</li>"
vars+="<li><b>REQUEST_METHOD</b>=$REQUEST_METHOD</li>"
vars+="<li><b>PATH_INFO</b>=$PATH_INFO</li>"
vars+="<li><b>REMOTE_HOST</b>=$REMOTE_HOST</li>"
if [[ $CONTENT_TYPE ]]; then
    vars+="<li><b>CONTENT_TYPE</b>=$CONTENT_TYPE</li>"
fi
if [[ $CONTENT_LENGTH ]]; then
    vars+="<li><b>CONTENT_LENGTH</b>=$CONTENT_LENGTH</li>"
fi

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
printf "\r\n%s" "$page"