--TEST--
statsd_dec
--FILE--
<?php

$statsd = statsd_connect('127.0.0.1');
var_dump(statsd_dec($statsd, "count1", 1.0));
echo "Done\n";
?>
--EXPECTF--
bool(true)
Done