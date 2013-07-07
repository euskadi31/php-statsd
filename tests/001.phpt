--TEST--
statsd
--FILE--
<?php

$statsd = statsd_connect('127.0.0.1');
echo "Done\n";
?>
--EXPECTF--
Done