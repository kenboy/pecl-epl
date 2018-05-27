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
function compare ($arrValue, $othValue){
  return $arrValue === $othValue;
}

$array = ["key" => 2.1, 1.2, 5 => "value3"];
var_dump(
  \epl\differenceWith($array, [2.3, 1.4, 2.1], 'compare'),
  $array
);
?>
--EXPECT--
array(2) {
  [0]=>
  float(1.2)
  [5]=>
  string(6) "value3"
}
array(3) {
  ["key"]=>
  float(2.1)
  [0]=>
  float(1.2)
  [5]=>
  string(6) "value3"
}