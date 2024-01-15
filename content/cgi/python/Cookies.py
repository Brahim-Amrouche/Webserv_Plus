import http.cookies
import os

# Get the stored cookies from the user's browser
stored_cookies = http.cookies.SimpleCookie(os.environ.get("HTTP_COOKIE"))

# Check if a 'test_cookie' is already set
if 'test_cookie' in stored_cookies:
    cookie_value = stored_cookies['test_cookie'].value
    message = f"Cookie Value: {cookie_value}"
else:
    # Set a new 'test_cookie' with a value
    cookie_value = "HelloCookie"
    message = "Cookie has been set"

    # Create a new cookie
    new_cookie = http.cookies.SimpleCookie()
    new_cookie['test_cookie'] = cookie_value

    # Print the 'Set-Cookie' header
    print(new_cookie.output() + "\r")

# Print the content type and an empty line to signal the end of headers
print("Content-type: text/html\r")
print("\r")

# Print HTML content
print("<html>")
print("<head><title>CGI Script with Cookie</title></head>")
print("<body>")
print("<h1>Cookie Example</h1>")
print("<p>" + message + "</p>")
print("</body>")
print("</html>")