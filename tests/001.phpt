--TEST--
statsd_connect
--FILE--
<?php

$statsd = statsd_connect('127.0.0.1');
var_dump(is_resource($statsd));
echo "Done\n";
?>
--EXPECTF--
bool(true)
Done