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
var_dump(\_\compact([0, 1, false, 2, '', 3]));
?>
--EXPECT--
array(3) {
  [1]=>
  int(1)
  [3]=>
  int(2)
  [5]=>
  int(3)
}