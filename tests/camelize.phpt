--TEST--
Inflector::camelize()
--SKIPIF--
<?php if (!extension_loaded('fastium')) print "skip"; ?>
--FILE--
<?php
use lithium\util\Inflector;
echo Inflector::camelize("lower cased word", false) . "\n";
echo Inflector::camelize('red_bike') . "\n";
echo Inflector::camelize('red bike') . "\n";
echo Inflector::camelize('some_word') . "\n";
echo Inflector::camelize("some other word", false) . "\n";
?>
--EXPECT--
lowerCasedWord
RedBike
RedBike
SomeWord
someOtherWord
