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
  \epl\chunk($array),
  (new \epl\collect($array))->chunk(2),
  $array
);
?>
--EXPECT--
array(3) {
  [0]=>
  array(1) {
    ["key"]=>
    string(5) "value"
  }
  [1]=>
  array(1) {
    [0]=>
    string(6) "value2"
  }
  [2]=>
  array(1) {
    [5]=>
    string(6) "value3"
  }
}
object(epl\collect)#1 (1) {
  ["value"]=>
  array(2) {
    [0]=>
    array(2) {
      ["key"]=>
      string(5) "value"
      [0]=>
      string(6) "value2"
    }
    [1]=>
    array(1) {
      [5]=>
      string(6) "value3"
    }
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
