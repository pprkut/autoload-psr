--TEST--
Autoload classes without namespace with PSR-0
--INI--
include_path=./tests/files
--SKIPIF--
<?php if (!extension_loaded("autoload_psr")) print "skip"; ?>
--FILE--
<?php

$class1 = new TestClass();
$class2 = new \TestClass();

?>
--EXPECT--
