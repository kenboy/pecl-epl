--TEST--
Check if underscore is loaded
--SKIPIF--
<?php
if (!extension_loaded('epl')) {
	echo 'skip';
}
?>
--FILE--
<?php 
echo 'The extension "epl" is available';
?>
--EXPECT--
The extension "epl" is available
