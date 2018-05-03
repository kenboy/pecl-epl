--TEST--
chunk
--SKIPIF--
<?php
if (!extension_loaded('epl')) {
	echo 'skip';
}
?>
--FILE--
<?php 
$hello = \epl\before(1, function($user) { return $user.' hello'; });
var_dump($hello('hello'));
?>
--EXPECT--
string(11) "hello hello"