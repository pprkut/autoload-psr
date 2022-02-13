--TEST--
Namespaced class not found with either PSR-0 or PSR-4
--EXTENSIONS--
autoload_psr
--FILE--
<?php

$class1 = new Foo\Bar\TestClass();

?>
--EXPECTF--
Fatal error: Uncaught Error: Class 'Foo\Bar\TestClass' not found in %sautoload_psr_not_found.php:%d
Stack trace:
#0 {main}
  thrown in %sautoload_psr_not_found.php on line %d
