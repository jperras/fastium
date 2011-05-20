--TEST--
Inflector::underscore()
--SKIPIF--
<?php if (!extension_loaded('fastium')) print "skip"; ?>
--FILE--
<?php
use fastium\Inflector;
echo Inflector::underscore('redBike') . "\n";
echo Inflector::underscore('aReallyLongWordToUnderscore') . "\n";
echo Inflector::underscore('some_word') . "\n";
echo Inflector::underscore('someWord') . "\n";
echo Inflector::underscore('someWord') . "\n"; // should make a cache hit, verifies same output
?>
--EXPECT--
red_bike
a_really_long_word_to_underscore
some_word
some_word
some_word
