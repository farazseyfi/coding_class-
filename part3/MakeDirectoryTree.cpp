
#include <dirent.h>
#include <sys/stat.h>
#include <memory>
#include <string_view>
#include <algorithm> // for find

#include <filesystem>
using namespace std::filesystem;

#include "makeDirectoryTree.hpp"

std::shared_ptr<Directory> make_directory_tree(const std::string_view& dirPath)
{
	path rootPath{path(dirPath)};
	if (exists(rootPath))
	{
		auto rootDir{std::make_shared<Directory>(rootPath.filename().generic_string())};
		directory_iterator it{rootPath};
		while (it != directory_iterator{})
		{
			path subPath{path(*it)};
			std::string subPathStr{subPath.generic_string()};
			if (is_regular_file(subPath))
			{
				auto file{std::make_shared<File>(subPath.filename().generic_string())};
				rootDir->add_childP(file);
			}
			if (is_directory(subPath))
			{
				auto subDir{make_directory_tree(subPathStr)};
				if (!subDir) // if the dir does not exist!
					subDir = std::make_shared<Directory>(subPath.filename().generic_string());
				rootDir->add_childP(subDir);
			}
			it++;
		}
		return rootDir;
	}
	else
	{
		OUT << rootPath << " does not exist.\n";
		return nullptr;
	}
}
