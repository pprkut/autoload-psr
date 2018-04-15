--TEST--
Check that autoload_psr is registered as an autoloader
--SKIPIF--
<?php if (!extension_loaded("autoload_psr")) print "skip"; ?>
--FILE--
<?php

var_dump(spl_autoload_functions());

?>
--EXPECT--
array(1) {
  [0]=>
  string(12) "autoload_psr"
}
