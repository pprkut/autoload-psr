--TEST--
Autoload classes with underscores, but no namespace with PSR-0
--EXTENSIONS--
autoload_psr
--INI--
include_path=./tests/files
--FILE--
<?php

$class1 = new Foo_Baz();
$class2 = new \Foo_Baz();

?>
--EXPECT--
