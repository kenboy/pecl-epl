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
var_dump(\epl\differenceWith([2.1, 1.2], [2.3, 3.4, 2.1], function ($arrValue, $othValue){
  return $arrValue === $othValue;
}));
?>
--EXPECT--
array(1) {
  [1]=>
  float(1.2)
}