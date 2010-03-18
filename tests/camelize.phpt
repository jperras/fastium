--TEST--
Inflector::camelize()
--SKIPIF--
<?php if (!extension_loaded('inflector')) print "skip"; ?>
--FILE--
<?php
use lithium\util\Inflector;
echo Inflector::camelize('red_bike') . "\n";
echo Inflector::camelize('red bike') . "\n";
echo Inflector::camelize('some_word') . "\n";
?>
--EXPECT--
RedBike
RedBike
SomeWord