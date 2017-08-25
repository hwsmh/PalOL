#ifndef _AUTHCOM_H_
#define _AUTHCOM_H_

typedef signed char        int8;
typedef unsigned char      uint8;
typedef short              int16;
typedef unsigned short     uint16;
typedef int                int32;
typedef unsigned int       uint32;
typedef long long          int64;
typedef unsigned long long uint64;

#pragma pack(1)


enum PACKET_CMD_TYPE
{
	MinCmd = 1,
	Cmd_Register,
	Cmd_Login,
	MaxCmd,
    MinTok = 1,
	Tok_LoginSuc,
	Tok_Error,
	MaxTok
};


enum 登录认证
{
	账户可以登录 = 1,
	账户或密码不正确
};

enum Eorror
{
	没有错误 = 0
};


typedef struct _PACKET_CMD_ 
{
	int32 dwCmd;
	int32 dwSize;
}PACKET_CMD,*PPACKET_CMD;

typedef struct _CMD_LOGIN_ 
{
	int8 username[256];
	int8 password[256];
}CMD_LOGIN,*PCMD_LOGIN;

typedef struct _TOKEN_ERROR_
{
	int8 error[256];
}TOKEN_ERROR,*PTOKEN_ERROR;

typedef struct _TOKEN_LOGINSUC_
{
	int8 cookie[256];
}TOKEN_LOGINSUC,*PTOKEN_LOGINSUC;


#pragma pack()
#endif // !_AUTHCOM_H_
