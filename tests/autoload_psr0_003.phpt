--TEST--
Autoload classes with underscores in package name with PSR-0
--INI--
include_path=./tests/files
--SKIPIF--
<?php if (!extension_loaded("autoload_psr")) print "skip"; ?>
--FILE--
<?php

$class1 = new Foo\Package_Name\TestClass();
$class2 = new \Foo\Package_Name\TestClass();

?>
--EXPECT--
