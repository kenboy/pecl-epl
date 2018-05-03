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
var_dump(\epl\differenceBy([2.1, 1.2], [2.3, 3.4], 'floor'));
?>
--EXPECT--
array(1) {
  [1]=>
  float(1.2)
}