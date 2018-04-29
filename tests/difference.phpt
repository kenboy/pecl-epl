--TEST--
chunk
--SKIPIF--
<?php
if (!extension_loaded('underscore')) {
	echo 'skip';
}
?>
--FILE--
<?php 
var_dump(\_\difference([2, 1], [2, 3]));
?>
--EXPECT--
array(1) {
  [1]=>
  int(1)
}