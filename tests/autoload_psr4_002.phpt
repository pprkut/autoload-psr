--TEST--
Autoload classes with PSR-4, partial namespace as prefix
--EXTENSIONS--
autoload_psr
--FILE--
<?php

autoload_register_psr4_prefix('Foo', './tests/files/Test/');

$class1 = new Foo\Bar\Baz();
$class2 = new \Foo\Bar\Baz();

?>
--EXPECT--
