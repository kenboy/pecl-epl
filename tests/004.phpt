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
var_dump(\_\differenceWith([2.1, 1.2], [2.3, 3.4, 2.1], function ($arrValue, $othValue){
  return $arrValue === $othValue;
}));
?>
--EXPECT--
array(1) {
  [1]=>
  float(1.2)
}