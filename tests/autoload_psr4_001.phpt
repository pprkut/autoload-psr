--TEST--
Autoload classes with PSR-4, full namespace as prefix
--SKIPIF--
<?php if (!extension_loaded("autoload_psr")) print "skip"; ?>
--FILE--
<?php

autoload_register_psr4_prefix('Foo\Bar\Baz', './tests/files/Test/');

$class1 = new Foo\Bar\Baz\Test();
$class2 = new \Foo\Bar\Baz\Test();

?>
--EXPECT--
