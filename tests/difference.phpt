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
  \epl\difference($array, ["value2", "key"=>"value4"]),
  (new \epl\collect($array))->difference(["value3"]),
  $array
);
?>
--EXPECT--
array(2) {
  ["key"]=>
  string(5) "value"
  [5]=>
  string(6) "value3"
}
object(epl\collect)#1 (1) {
  ["value":protected]=>
  array(2) {
    ["key"]=>
    string(5) "value"
    [0]=>
    string(6) "value2"
  }
}
array(3) {
  ["key"]=>
  string(5) "value"
  [0]=>
  string(6) "value2"
  [5]=>
  string(6) "value3"
}