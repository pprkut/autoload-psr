--TEST--
Autoload classes with PSR-4, same prefix in multiple places
--SKIPIF--
<?php if (!extension_loaded("autoload_psr")) print "skip"; ?>
--FILE--
<?php

autoload_register_psr4_prefix('Foo\Bar', './tests/files/Bar/');
autoload_register_psr4_prefix('Foo\Bar', './tests/files/Bar2/');
autoload_register_psr4_prefix('Foo\Bar', './tests/files/Foo/Bar/');

$class1 = new \Foo\Bar\Baz();
$class2 = new \Foo\Bar\Baz2();
$class3 = new \Foo\Bar\Baz3();

$files = get_included_files();

foreach ($files as $file)
{
    var_dump(str_replace(__DIR__, '', $file));
}

?>
--EXPECT--
string(22) "/autoload_psr4_007.php"
string(22) "/files/Foo/Bar/Baz.php"
string(19) "/files/Bar/Baz2.php"
string(20) "/files/Bar2/Baz3.php"
