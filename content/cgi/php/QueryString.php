#!/usr/bin/php-cgi
<?php
header("Content-type: text/html");

// Function to get the value of a query parameter
function getQueryParam($name) {
    return isset($_GET[$name]) ? $_GET[$name] : null;
}

// Get the value of the 'name' parameter from the query string
$name = getQueryParam('name');

echo "<html>
<head>
    <title>PHP CGI Script with Query String</title>
</head>
<body>";

if ($name) {
    echo "<h1>Hello, $name!</h1>";
} else {
    echo "<h1>Hello, CGI!</h1>";
}

echo "</body>
</html>";
?>