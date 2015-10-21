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
		@mkdir($this->config['output'], 0755, true);
		$this->config['output'] = realpath($this->config['output']);
		if (empty($this->config['output']) || !is_dir($this->config['output']))
		{
			printf("ERR: invalid output dir %s\n", $this->config['output']);
			return false;
		}

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
		$modules = $this->prepareForPack($files);

		$bytes = $this->getModulesData($modules, $this->config['key'], $this->config['sign']);
		if (!is_array($bytes))
		{
			$this->cleanupTempFiles($modules);
			return false;
		}

		if (!$this->moveEncrptyFiles($modules, $bytes))
		{
			$this->cleanupTempFiles($modules);
			return false;
		}

		return true;
	}

	protected function searchSourceFiles()
	{
		if (!$this->config['quiet'])
		{
			printf("Pack source files in path %s\n", $this->config['srcpath']);
		}
		$files = array();
		findFiles($this->config['srcpath'], $files);
		return $files;
	}

	private function copyFile($moduleName)
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
			printf("ERROR: mkdir failure\n");
			return false;
		}

		copy($srcPath, $destPath );
		return true;
	}

	protected function prepareForPack(array $files)
	{
		$modules = array();
		// prepare for pack
		foreach ($files as $key => $path)
		{
			$moduleName = substr($path, $this->config['srcpathLength']);
			$fileType = strtolower(substr($path, -4));
			if ($fileType != '.jpg'
				&& $fileType != '.jpeg'
				&& $fileType != '.ccz' // texturePacker
				&& $fileType != '.pvr' // texturePacker
				&& $fileType != '.bmp'
				&& $fileType != '.png')
			{
				$this->copyFile($moduleName);
			}
			else
			{
				// pic wait to encrpty
				$moduleName = str_replace('.', SPLIT_CHAR, $moduleName);
				$tempFilePath = $this->config['srcpath'] . DS . $moduleName . '.tmp';
				$moduleName = str_replace(DS, '.', $moduleName);

				$modules[$path] = array(
					'moduleName' => $moduleName,
					'tempFilePath' => $tempFilePath,
				);
			}
		}
		return $modules;
	}

	protected function cleanupTempFiles(array $modules)
	{
		foreach ($modules as $module)
		{
			if (file_exists($module['tempFilePath']))
			{
				unlink($module['tempFilePath']);
			}
		}
	}

	protected function getModulesData(array $modules, $key = null, $sign = null)
	{
		if (!empty($key))
		{
			$xxtea = new XXTEA();
			$xxtea->setKey($key);
		}

		$modulesBytes = array();
		foreach ($modules as $path => $module)
		{
			$bytes = file_get_contents($path);
			if (!empty($key))
			{
				$bytes = $sign . $xxtea->encrypt($bytes);
			}
			file_put_contents($module['tempFilePath'], $bytes);
			if (!$bytes)
			{
				print("\n");
				return false;
			}
			$modulesBytes[$path] = $bytes;
			if (!$this->config['quiet'])
			{
				printf("  > get bytes [% 3d KB] %s\n", ceil(strlen($bytes) / 1024), $module['moduleName']);
			}
		}
		return $modulesBytes;
	}

	protected function moveEncrptyFiles(array $modules, array $bytes)
	{
		foreach ($modules as $module)
		{
			$destPath = $this->config['output'] . DS . str_replace(SPLIT_CHAR, '.', str_replace('.', DS, $module['moduleName']));
			@mkdir(pathinfo($destPath, PATHINFO_DIRNAME), 0755, true);
			rename($module['tempFilePath'], $destPath);
		}

		printf("create output files in %s .\n", $this->config['output']);
		print("done.\n\n");
		return true;
	}
}
