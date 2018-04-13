--TEST--
Check if underscore is loaded
--SKIPIF--
<?php
if (!extension_loaded('underscore')) {
	echo 'skip';
}
?>
--FILE--
<?php 
echo 'The extension "underscore" is available';
?>
--EXPECT--
The extension "underscore" is available
