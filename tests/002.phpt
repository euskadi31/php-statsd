--TEST--
statsd_count
--FILE--
<?php

$statsd = statsd_connect('127.0.0.1');
var_dump(statsd_count($statsd, "count1", 123, 1.0));
echo "Done\n";
?>
--EXPECTF--
bool(true)
Done