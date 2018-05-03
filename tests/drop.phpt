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
var_dump(\epl\drop([1, 2, 3]));
var_dump(\epl\drop([1, 2, 3], 2));
var_dump(\epl\drop([1, 2, 3], 5));
var_dump(\epl\drop([1, 2, 3], 0));
?>
--EXPECT--
array(2) {
  [1]=>
  int(2)
  [2]=>
  int(3)
}
array(1) {
  [2]=>
  int(3)
}
array(0) {
}
array(3) {
  [0]=>
  int(1)
  [1]=>
  int(2)
  [2]=>
  int(3)
}