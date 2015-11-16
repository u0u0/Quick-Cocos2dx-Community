<?php

require_once(__DIR__ . '/init.php');
require_once(__DIR__ . '/xxtea.php');

class ResEncrpty
{
	const ENCRYPT_XXTEA_DEFAULT_SIGN = 'XXTEA';

	private $config;
	private $options;
	private $validated = false;

	function __construct($config, $options)
	{
		$this->config = $config;
		$this->options = $options;
	}

	function validateConfig()
	{
		if (empty($this->config['src']))
		{
			printf("ERR: not specifies source files directory\n");
			return false;
		}

		if (empty($this->config['output']))
		{
			printf("ERR: not output filename or output directory\n");
			return false;
		}

		if (empty($this->config['key']))
        {
            print("ERR: not set encrypt key\n");
            return false;
        }

		if (empty($this->config['sign']))
		{
			$this->config['sign'] = self::ENCRYPT_XXTEA_DEFAULT_SIGN;
		}

		if (!$this->config['quiet'])
		{
			dumpConfig($this->config, $this->options);
		}

		// check src path
		$srcpath = realpath($this->config['src']);
		if (!is_dir($srcpath))
		{
			printf("ERR: invalid src dir %s\n", $srcpath);
			return false;
		}
		$this->config['srcpath'] = $srcpath;
		$this->config['srcpathLength'] = strlen($srcpath) + 1;

		// check output path
		if (empty($this->config['output']) 
			|| file_exists($this->config['output']))
		{
			printf("ERR: invalid output dir or output dir already exists\n");
			return false;
		}
		@mkdir($this->config['output'], 0755, true);
		$this->config['output'] = realpath($this->config['output']);		

		$this->validated = true;
		return true;
	}

	function run()
	{
		if (!$this->validated)
		{
			print("ERR: invalid config\n");
			return false;
		}

		$files = $this->searchSourceFiles();
		return $this->encryptFiles($files);


		printf("create output files in %s .\n", $this->config['output']);
		printf("done.\n");
	}

	protected function searchSourceFiles()
	{
		if (!$this->config['quiet'])
		{
			printf("INFO:Pack source files in path %s\n", $this->config['srcpath']);
		}
		$files = array();
		findFiles($this->config['srcpath'], $files);
		return $files;
	}

	protected function getModuleData($moduleName)
	{
		$xxtea = new XXTEA();
		$xxtea->setKey($this->config['key']);

		$srcPath = $this->config['srcpath'] . DS . $moduleName;
		$bytes = file_get_contents($srcPath);
		$bytes = $this->config['sign'] . $xxtea->encrypt($bytes);
		return $bytes;
	}

	private function copyFile($moduleName, $isNeedEncrypt)
	{
		$srcPath = $this->config['srcpath'] . DS . $moduleName;
		$destPath = $this->config['output'] . DS . $moduleName;
		$destinationDir = pathinfo($destPath, PATHINFO_DIRNAME);

		if (!is_dir($destinationDir))
		{
			mkdir($destinationDir, 0755, true);
		}
		if (!is_dir($destinationDir))
		{
			exit("ERROR: mkdir failure");
		}

		if ($isNeedEncrypt)
		{
			$bytes = $this->getModuleData($moduleName);
			file_put_contents($destPath, $bytes);
			if (!$this->config['quiet'])
			{
				printf("INFO: encrypt bytes [% 3d KB] %s\n", ceil(strlen($bytes) / 1024), $moduleName);
			}
		} else {
			copy($srcPath, $destPath);

			if (!$this->config['quiet'])
			{
				printf("INFO: copy file:%s\n", $moduleName);
			}
		}
	}

	protected function encryptFiles(array $files)
	{
		foreach ($files as $key => $path)
		{
			$moduleName = substr($path, $this->config['srcpathLength']);
			$fileType = strtolower(substr($path, -4));
			if ($fileType != '.jpg'
				&& $fileType != '.ccz' // texturePacker
				&& $fileType != '.pvr' // texturePacker
				&& $fileType != '.bmp'
				&& $fileType != '.png')
			{
				// copy directly
				$this->copyFile($moduleName, false);
			}
			else
			{
				// encrpty to dest
				$this->copyFile($moduleName, true);
			}
		}
	}
}
