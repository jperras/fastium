<?php

/**
 * Fastium performance benchmark. Export classes under 'fastium\util' namespace and run from the
 * command line using `php speedtest.php`.
 */

if (!class_exists('fastium\util\Inflector')) {
	die("\nChange extension namespace to 'fastium' before running me.\n\n");
}

require '/Library/WebServer/Documents/lithium/core/libraries/lithium/util/Inflector.php';

$loopCount = 99999;

$tests = array(
	'Underscore' => array(
		'method' => 'underscore',
		'value'  => 'FooBarBaz'
	),
	'Humanize'   => array(
		'method' => 'humanize',
		'value'  => 'foo_bar_baz'
	),
	'Camelize'   => array(
		'method' => 'camelize',
		'value'  => 'foo_bar_baz'
	),
	'Camelize, $cased = false'   => array(
		'method' => 'camelize',
		'value'  => 'foo_bar_baz',
		'arg' => false
	)
);

foreach ($tests as $title => $params) {
	echo "\n{$title} test:\n";
	$method = $params['method'];
	$value = $params['value'];

	if (isset($params['arg'])) {

		$start = microtime(true);
		for ($i = 0; $i <= $loopCount; $i++) {
			$result = lithium\util\Inflector::$method($value, $params['arg']);
		}
		$phpTime = (microtime(true) - $start);
		echo "PHP:  {$phpTime}\n";

		$start = microtime(true);
		for ($i = 0; $i <= $loopCount; $i++) {
			$result = fastium\util\Inflector::$method($value, $params['arg']);
		}
		$cTime = (microtime(true) - $start);
		echo "C:    {$cTime}\n";

	} else {

		$start = microtime(true);
		for ($i = 0; $i <= $loopCount; $i++) {
			$result = lithium\util\Inflector::$method($value);
		}
		$phpTime = (microtime(true) - $start);
		echo "PHP:  {$phpTime}\n";

		$start = microtime(true);
		for ($i = 0; $i <= $loopCount; $i++) {
			$result = fastium\util\Inflector::underscore('FooBarBaz');
		}
		$cTime = (microtime(true) - $start);
		echo "C:    {$cTime}\n";

	}
	printf("Diff: %01.2f%%\n", ($phpTime / $cTime) * 100);
}

die("\n* higher % are better\n\n");

?>