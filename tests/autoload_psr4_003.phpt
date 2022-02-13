--TEST--
Autoload classes with PSR-4, prefix preference
--EXTENSIONS--
autoload_psr
--FILE--
<?php

autoload_register_psr4_prefix('Foo', './tests/files/Test/');
autoload_register_psr4_prefix('Foo\Bar', './tests/files/Foo/Bar/');

$class1 = new Foo\Bar\Baz();
$class2 = new \Foo\Bar\Baz();

$files = get_included_files();

foreach ($files as $file)
{
    var_dump(str_replace(__DIR__, '', $file));
}

?>
--EXPECT--
string(22) "/autoload_psr4_003.php"
string(22) "/files/Foo/Bar/Baz.php"
