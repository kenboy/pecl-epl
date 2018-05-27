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
$array = ["key" => "value", "value2", 5 => "value3"];
var_dump(
  \epl\drop($array),
  \epl\drop($array, 2),
  \epl\drop($array, 5),
  \epl\drop($array, 0),
  $array
);
?>
--EXPECT--
array(2) {
  [0]=>
  string(6) "value2"
  [5]=>
  string(6) "value3"
}
array(1) {
  [5]=>
  string(6) "value3"
}
array(0) {
}
array(3) {
  ["key"]=>
  string(5) "value"
  [0]=>
  string(6) "value2"
  [5]=>
  string(6) "value3"
}
array(3) {
  ["key"]=>
  string(5) "value"
  [0]=>
  string(6) "value2"
  [5]=>
  string(6) "value3"
}