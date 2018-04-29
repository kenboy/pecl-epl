--TEST--
chunk
--SKIPIF--
<?php
if (!extension_loaded('underscore')) {
	echo 'skip';
}
?>
--FILE--
<?php 
var_dump(\_\chunk(["hello", "hello2"]));
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
