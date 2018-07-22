--TEST--
collectCreate
--SKIPIF--
<?php
if (!extension_loaded('epl')) {
	echo 'skip';
}
?>
--FILE--
<?php
$collect = \epl\collect([0, 1, false, 2, '', 3]);
var_dump($collect, $collect->all(), \epl\collect());
?>
--EXPECT--
object(epl\collect)#1 (1) {
  ["value":protected]=>
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
object(epl\collect)#2 (1) {
  ["value":protected]=>
  array(0) {
  }
}