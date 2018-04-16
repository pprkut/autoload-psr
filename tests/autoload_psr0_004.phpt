--TEST--
Autoload classes with underscores, but no namespace with PSR-0
--INI--
include_path=./tests/files
--SKIPIF--
<?php if (!extension_loaded("autoload_psr")) print "skip"; ?>
--FILE--
<?php

$class1 = new Foo_Baz();
$class2 = new \Foo_Baz();

?>
--EXPECT--
