<?php

ini_set('memory_limit','256M');
require_once(__DIR__ . '/quick/ResEncrypt.php');

$options = array(
    array('h',   'help',       0,      false,       'show help'),
    array('i',   'src',        1,      null,        'source files directory'),
    array('o',   'output',     1,      null,        'output filename | output directory'),
    array('ek',  'key',        1,      null,        'encrypt key'),
    array('es',  'sign',       1,      null,        'encrypt sign'),
    array('c',   'config',     1,      null,        'load options from config file'),
    array('q',   'quiet',      0,      false,       'quiet'),
);

function errorhelp()
{
    print("\nshow help:\n    encrypt_res -h\n\n");
}

function help()
{
    global $options;

    echo <<<EOT

usage: encrypt_res -i src -o output ...

options:

EOT;

    for ($i = 0; $i < count($options); $i++)
    {
        $o = $options[$i];
        printf("    -%s %s\n", $o[0], $o[4]);
    }

    echo <<<EOT

config file format:

    return array(
        'src'      => source files directory,
        'output'   => output filename or output directory,
        'key'      => encrypt key,
        'sign'     => encrypt sign,
    );

examples:

    # encrypt res/*.* to resnew/, with XXTEA, specifies sign
    encrypt_res -i res -o resnew -es XXTEA -ek test

    # load options from config file
    encrypt_res -c my_config.lua

EOT;

}

// ----

print("\n");
if ($argc < 2)
{
    help();
    return(1);
}

$config = fetchCommandLineArguments($argv, $options, 4);
if (!$config)
{
    errorhelp();
    return(1);
}

if ($config['help'])
{
    help();
    return(0);
}

if ($config['config'])
{
    $configFilename = $config['config'];
    if (file_exists($configFilename))
    {
        $config = @include($configFilename);
    }
    else
    {
        $config = null;
    }

    if (!is_array($config))
    {
        printf("ERR: invalid config file, %s\n", $configFilename);
        errorhelp();
        return(1);
    }
}

$packer = new ResEncrpty($config, $options);
if ($packer->validateConfig())
{
    return($packer->run());
}
else
{
    errorhelp();
    return(1);
}
