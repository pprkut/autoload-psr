--TEST--
Autoload classes without namespace with PSR-4
--SKIPIF--
<?php if (!extension_loaded("autoload_psr")) print "skip"; ?>
--FILE--
<?php

$class1 = new TestClass();
$class2 = new \TestClass();

?>
--EXPECT--
Fatal error: Uncaught Error: Class 'TestClass' not found in /mnt/progs/projects/personal/autoload_psr/tests/autoload_psr4_005.php:3
Stack trace:
#0 {main}
  thrown in /mnt/progs/projects/personal/autoload_psr/tests/autoload_psr4_005.php on line 3
