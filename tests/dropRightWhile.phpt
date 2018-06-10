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
function compare ($user) 
{ 
  return $user['active'];
}

$users = [
  [ 'user' => 'barney',  'active' => false ],
  [ 'user' => 'fred',    'active' => true ],
  [ 'user' => 'pebbles', 'active' => true ]
];

var_dump(
  \epl\dropRightWhile($users, 'compare'),
  (new \epl\collect($users))->dropRightWhile('compare'),
  $users
);
?>
--EXPECT--
array(2) {
  [1]=>
  array(2) {
    ["user"]=>
    string(4) "fred"
    ["active"]=>
    bool(true)
  }
  [2]=>
  array(2) {
    ["user"]=>
    string(7) "pebbles"
    ["active"]=>
    bool(true)
  }
}
object(epl\collect)#1 (1) {
  ["value":protected]=>
  array(2) {
    [1]=>
    array(2) {
      ["user"]=>
      string(4) "fred"
      ["active"]=>
      bool(true)
    }
    [2]=>
    array(2) {
      ["user"]=>
      string(7) "pebbles"
      ["active"]=>
      bool(true)
    }
  }
}
array(3) {
  [0]=>
  array(2) {
    ["user"]=>
    string(6) "barney"
    ["active"]=>
    bool(false)
  }
  [1]=>
  array(2) {
    ["user"]=>
    string(4) "fred"
    ["active"]=>
    bool(true)
  }
  [2]=>
  array(2) {
    ["user"]=>
    string(7) "pebbles"
    ["active"]=>
    bool(true)
  }
}