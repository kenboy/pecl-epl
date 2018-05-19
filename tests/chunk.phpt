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
  \epl\chunk(["hello", "hello2"]),
  (new \epl\collect(["hello", "hello2", "hello3"]))->chunk(2)
);
?>
--EXPECT--
array(2) {
  [0]=>
  array(1) {
    [0]=>
    string(5) "hello"
  }
  [1]=>
  array(1) {
    [1]=>
    string(6) "hello2"
  }
}
object(epl\collect)#1 (1) {
  ["value"]=>
  array(2) {
    [0]=>
    array(2) {
      [0]=>
      string(5) "hello"
      [1]=>
      string(6) "hello2"
    }
    [1]=>
    array(1) {
      [2]=>
      string(6) "hello3"
    }
  }
}
