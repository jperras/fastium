--TEST--
Inflector::underscore()
--SKIPIF--
<?php if (!extension_loaded('inflector')) print "skip"; ?>
--FILE--
<?php
use lithium\util\Inflector;
echo Inflector::underscore('redBike') . "\n";
echo Inflector::underscore('aReallyLongWordToUnderscore') . "\n";
echo Inflector::underscore('some_word') . "\n";
?>
--EXPECT--
red_bike
a_really_long_word_to_underscore
some_word