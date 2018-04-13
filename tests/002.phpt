--TEST--
underscore_test1() Basic test
--SKIPIF--
<?php
if (!extension_loaded('underscore')) {
	echo 'skip';
}
?>
--FILE--
<?php 
$ret = underscore_test1();

var_dump($ret);
?>
--EXPECT--
The extension underscore is loaded and working!
NULL
