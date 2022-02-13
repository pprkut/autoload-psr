--TEST--
Autoload namespaced classes with PSR-0
--EXTENSIONS--
autoload_psr
--INI--
include_path=./tests/files
--FILE--
<?php

$class1 = new Foo\Bar\Baz();
$class2 = new \Foo\Bar\Baz();

?>
--EXPECT--
