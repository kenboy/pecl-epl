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
$array = [0, 1, false, 2, '', 3];
var_dump(
  \epl\compact($array),
  (new \epl\collect($array))->compact(),
  $array
);
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
object(epl\collect)#1 (1) {
  ["value"]=>
  array(3) {
    [1]=>
    int(1)
    [3]=>
    int(2)
    [5]=>
    int(3)
  }
}
array(6) {
  [0]=>
  int(0)
  [1]=>
  int(1)
  [2]=>
  bool(false)
  [3]=>
  int(2)
  [4]=>
  string(0) ""
  [5]=>
  int(3)
}