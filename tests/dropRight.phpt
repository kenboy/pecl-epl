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
  \epl\dropRight($array),
  \epl\dropRight($array, 2),
  \epl\dropRight($array, 5),
  \epl\dropRight($array, 0),
  $array
);
?>
--EXPECT--
array(2) {
  ["key"]=>
  string(5) "value"
  [0]=>
  string(6) "value2"
}
array(1) {
  ["key"]=>
  string(5) "value"
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