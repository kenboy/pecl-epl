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
$collect = new \epl\collect([0]);
$collect["key"] = 1;
$collect[] = false;
$collect[] = 2;
$collect[] = "";
$collect[] = 3;

var_dump(
  $collect, 
  $collect->all(), 
  iterator_to_array($collect),
  count($collect),
  isset($collect[0]),
  isset($collect["key"]),
  isset($collect[6]),
  $collect[4],
  $collect["key"]
);
?>
--EXPECT--
object(epl\collect)#1 (1) {
  ["value"]=>
  array(6) {
    [0]=>
    int(0)
    ["key"]=>
    int(1)
    [1]=>
    bool(false)
    [2]=>
    int(2)
    [3]=>
    string(0) ""
    [4]=>
    int(3)
  }
}
array(6) {
  [0]=>
  int(0)
  ["key"]=>
  int(1)
  [1]=>
  bool(false)
  [2]=>
  int(2)
  [3]=>
  string(0) ""
  [4]=>
  int(3)
}
array(6) {
  [0]=>
  int(0)
  ["key"]=>
  int(1)
  [1]=>
  bool(false)
  [2]=>
  int(2)
  [3]=>
  string(0) ""
  [4]=>
  int(3)
}
int(6)
bool(true)
bool(true)
bool(false)
int(3)
int(1)