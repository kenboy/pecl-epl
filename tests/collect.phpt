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
$collect = new \epl\collect([0, 1, false, 2, '', 3]);
var_dump($collect, $collect->all());
?>
--EXPECT--
object(epl\collect)#1 (1) {
  ["value"]=>
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