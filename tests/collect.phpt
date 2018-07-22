--TEST--
collect
--SKIPIF--
<?php
if (!extension_loaded('epl')) {
	echo 'skip';
}
?>
--FILE--
<?php
$collect = new \epl\collect();
$collect[false] = 0;
$collect["key"] = 1;
$collect[2.3] = false;
$collect[null] = 2;
$collect[] = "string";
$collect[5] = 3;

var_dump(
  $collect, 
  $collect->all(), 
  iterator_to_array($collect),
  count($collect),
  isset($collect[false]),
  isset($collect["key"]),
  isset($collect[6]),
  $collect[4],
  $collect["key"]
);
?>
--EXPECT--
object(epl\collect)#1 (1) {
  ["value":protected]=>
  array(6) {
    [0]=>
    int(0)
    ["key"]=>
    int(1)
    [2]=>
    bool(false)
    [3]=>
    int(2)
    [4]=>
    string(6) "string"
    [5]=>
    int(3)
  }
}
array(6) {
  [0]=>
  int(0)
  ["key"]=>
  int(1)
  [2]=>
  bool(false)
  [3]=>
  int(2)
  [4]=>
  string(6) "string"
  [5]=>
  int(3)
}
array(6) {
  [0]=>
  int(0)
  ["key"]=>
  int(1)
  [2]=>
  bool(false)
  [3]=>
  int(2)
  [4]=>
  string(6) "string"
  [5]=>
  int(3)
}
int(6)
bool(true)
bool(true)
bool(false)
string(6) "string"
int(1)