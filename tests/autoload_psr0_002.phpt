--TEST--
Autoload classes with underscores in class name with PSR-0
--EXTENSIONS--
autoload_psr
--INI--
include_path=./tests/files
--FILE--
<?php

$class1 = new Foo\Bar\Test_Class();
$class2 = new \Foo\Bar\Test_Class();

?>
--EXPECT--
