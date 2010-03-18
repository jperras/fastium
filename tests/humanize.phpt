--TEST--
Inflector::humanize()
--SKIPIF--
<?php if (!extension_loaded('inflector')) print "skip"; ?>
--FILE--
<?php
use lithium\util\Inflector;
echo Inflector::humanize('red_bike') . "\n";
echo Inflector::humanize('a_really_long_word') . "\n";
echo Inflector::humanize('some_word') . "\n";
echo Inflector::humanize('the-post-title', '-') . "\n";
?>
--EXPECT--
Red Bike
A Really Long Word
Some Word
The Post Title