--TEST--
statsd_timing
--FILE--
<?php

$statsd = statsd_connect('127.0.0.1');
var_dump(statsd_timing($statsd, "request", 2400));
echo "Done\n";
?>
--EXPECTF--
bool(true)
Done