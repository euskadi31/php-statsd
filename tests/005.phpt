--TEST--
statsd_gauge
--FILE--
<?php

$statsd = statsd_connect('127.0.0.1');
var_dump(statsd_gauge($statsd, "speed", 10));
echo "Done\n";
?>
--EXPECTF--
bool(true)
Done