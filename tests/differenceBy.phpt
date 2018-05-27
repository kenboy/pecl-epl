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
$array = ["key" => 2.1, 1.2, 5 => "value3"];
var_dump(
  \epl\differenceBy($array, ["key" => 2.3, 3.4], 'floor'),
  (new \epl\collect($array))->differenceBy(["key" => 2.3, 3.4], 'floor'),
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
object(epl\collect)#1 (1) {
  ["value"]=>
  array(2) {
    [0]=>
    float(1.2)
    [5]=>
    string(6) "value3"
  }
}
array(3) {
  ["key"]=>
  float(2.1)
  [0]=>
  float(1.2)
  [5]=>
  string(6) "value3"
}