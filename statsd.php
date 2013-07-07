<?php
$br = (php_sapi_name() == "cli")? "":"<br>";

if(!extension_loaded('statsd')) {
    dl('statsd.' . PHP_SHLIB_SUFFIX);
}
$module = 'statsd';
$functions = get_extension_funcs($module);
echo "Functions available in the test extension:$br\n";
foreach($functions as $func) {
    echo $func."$br\n";
}
echo "$br\n";
if (extension_loaded($module)) {
    echo "Done";
} else {
    echo "Module $module is not compiled into PHP";
}
