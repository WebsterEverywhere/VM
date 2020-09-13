#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <io.h>
#include "resource.h"

using namespace std;

string g_rootPath;
void ChoseFileRoot();
bool VM_GetModuleFilePath(char* path);
bool IniConfigByPath(char* path);
bool IniConfig();