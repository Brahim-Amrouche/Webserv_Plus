#!/usr/bin/env python3

import os

# Get the query string parameters from the environment
query_string = os.environ.get("QUERY_STRING", "")
query_params = {}

# Parse the query string into key-value pairs
for qc in query_string.split("&"):
    parts = qc.split("=")
    key = parts[0]
    value = parts[1] if len(parts) > 1 else ''
    query_params[key] = value

# Extract values from the query parameters
name = query_params.get('name', '')
age = query_params.get('age', '')

# Print the content type and an empty line to signal the end of headers
print("Content-type: text/html\r")
print("\r")

# Print HTML content
print("<html>")
print("<head><title>CGI Script with QueryString</title></head>")
print("<body>")
print("<h1>Query String Example</h1>")
print("<p>Name: " + name + "</p>")
print("<p>Age: " + age + "</p>")
print("</body>")
print("</html>")