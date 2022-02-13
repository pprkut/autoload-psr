--TEST--
Autoload classes with underscores in package name with PSR-0
--EXTENSIONS--
autoload_psr
--INI--
include_path=./tests/files
--FILE--
<?php

$class1 = new Foo\Package_Name\TestClass();
$class2 = new \Foo\Package_Name\TestClass();

?>
--EXPECT--
