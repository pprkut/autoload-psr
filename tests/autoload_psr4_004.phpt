--TEST--
Autoload classes with underscores in the class/package name with PSR-4
--SKIPIF--
<?php if (!extension_loaded("autoload_psr")) print "skip"; ?>
--FILE--
<?php

autoload_register_psr4_prefix('Foo', './tests/files/Foo/');

$class1 = new Foo\Package_Name\Test_Class();
$class2 = new \Foo\Package_Name\Test_Class();

?>
--EXPECT--
