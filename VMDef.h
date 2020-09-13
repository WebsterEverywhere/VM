#pragma once

///------------------------------
// 宏定义
///------------------------------
#define ROOT_PATH_LENGTH 100 // 根目录路径的长度



///------------------------------
// 结构体定义
///------------------------------

// 全局配置项结构体定义
typedef struct config
{
	char* rootPath; // 系统工作区根目录
	bool workPathFlag; // 系统工作根目录存在标志。
}CONFIG;


///------------------------------
// 枚举变量定义
///------------------------------
typedef enum state
{
	CONFIG_SUCESS,        // 配置成功
	CONFIG_NOROOTFILE	  // 没有工作目录
}CONFIG_STATE;