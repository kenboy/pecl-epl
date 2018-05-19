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
var_dump(
  \epl\compact([0, 1, false, 2, '', 3]),
  (new \epl\collect([0, 1, false, 2, '', 3]))->compact()
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