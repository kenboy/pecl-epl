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
$users = [
  [ 'user' => 'barney',  'active' => false ],
  [ 'user' => 'fred',    'active' => true ],
  [ 'user' => 'pebbles', 'active' => true ]
];

var_dump(\_\dropWhile($users, function($user) { return !$user['active']; }));
?>
--EXPECT--
array(1) {
  [0]=>
  array(2) {
    ["user"]=>
    string(6) "barney"
    ["active"]=>
    bool(false)
  }
}