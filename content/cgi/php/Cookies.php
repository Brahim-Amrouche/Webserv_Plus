#!/usr/bin/php-cgi
<?php
header("Content-type: text/html");

// Function to get the current value of the counter from the cookie
function getCounterValue() {
    if (isset($_COOKIE['counter'])) {
        return $_COOKIE['counter'];
    } else {
        return 0;
    }
}

// Function to set a new value for the counter in the cookie
function setCounterValue($value) {
    setcookie('counter', $value, time() + 3600); // Cookie expires in 1 hour (adjust as needed)
}

// Check if the button is clicked
if (getCounterValue() != NULL) {
    $counterValue = getCounterValue() + 1;
    setCounterValue($counterValue);
} else {
    $counterValue = getCounterValue();
    setCounterValue($counterValue);
}

echo "<html>
<head>
    <title>PHP CGI Counter</title>
</head>
<body>
    <h1>Counter Value: $counterValue</h1>
    <form method=\"post\">
        <input type=\"submit\" name=\"increment\" value=\"Increment\">
    </form>
</body>
</html>";
?>