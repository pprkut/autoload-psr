--TEST--
Autoload namespaced classes with PSR-0
--INI--
include_path=./tests/files
--SKIPIF--
<?php if (!extension_loaded("autoload_psr")) print "skip"; ?>
--FILE--
<?php

$class1 = new Foo\Bar\Baz();
$class2 = new \Foo\Bar\Baz();

?>
--EXPECT--
