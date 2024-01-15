#!/usr/bin/php-cgi
<?php
header("Content-Length: 100");
header("Content-Line: 10");
header("Content-Type: text/javascript");

echo "<html>
<head>
    <title>Simple PHP CGI Script</title>
</head>
<body>
    <h1>Hello, CGI!</h1>
</body>
</html>";
?>