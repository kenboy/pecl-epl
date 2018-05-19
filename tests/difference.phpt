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
  \epl\difference([2, 1], [2, 3]),
  (new \epl\collect([2, 1]))->difference([2, 3])
);
?>
--EXPECT--
array(1) {
  [1]=>
  int(1)
}
object(epl\collect)#1 (1) {
  ["value"]=>
  array(1) {
    [1]=>
    int(1)
  }
}