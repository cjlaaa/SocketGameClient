//
//  Client.h
//  testClient
//
//  Created by hENRYcHANG on 14-9-12.
//
//

#ifndef __testClient__Client__
#define __testClient__Client__

#include "def.h"
#include "message.h"
#include "cocos2d.h"
#define dMAX_SOCK_BUFF 1024

class ClientDelegate
{
public:
    virtual void GetServerMsg(std::string) = 0;
};

class Client : public Node
{
public:
    static Client* CreateClient(Node*);
    bool init(Node*);
    
    //要连接的服务器IP
    char m_ServerIP[128];
    int m_Port;
    //客户端的套接字
    SOCKET m_sock;
    
    //昵称
    char m_NickName[64];
    
    //用于recv queue的变量
    int m_recvPos;
    int m_recvSize;
    char m_recvBuff[dMAX_SOCK_BUFF];
    
    //场景成员变量
    Node* m_Scene;
    
    //数据包消息函数
    void OnPacketChatMsg();
    void OnSendChatMsg(std::string);
    void OnSendChangeNick(char *nick);
    void OnSendAliveCheck();
    
    //设置套接字为Nonblocking模式
    void NonBlock(SOCKET sock);
    //数据的接收函数
    void RecvData();
    //连接处理函数
    BOOL ConnectToServer(char *host,int port);
    //关闭函数
    void CloseChat();
    //周期函数
    void update(float fT);
    //输出函数
    void ClientLog(char *buf,...);
    
protected:
    //BYTE型数据包函数
    void PutByte(char *pPacket,BYTE value,int &nPos);
    BYTE GetByte(char *pPacket,int &nPos);
    
    //WORD型数据包函数
    void PutWord(char *pPacket,WORD value,int &nPos);
    WORD GetWord(char *pPacket,int &nPos);
    
    //DWORD型数据包函数
    void PutDword(char *pPacket,DWORD value,int &nPos);
    DWORD GetDword(char *pPacket,int &nPos);
    
    //int型数据包函数
    void PutInteger(char *pPacket,int value,int &nPos);
    int GetInteger(char *pPacket,int &nPos);
    
    //short型数据包函数
    void PutShort(char *pPacket,short value,int &nPos);
    short GetShort(char *pPacket,int &nPos);
    
    //字符串型数据包函数
    void PutString(char *pPacket,char* str,int &nPos);
    void GetString(char *pPacket,char* pBuffer,int &nPos);
    
    //数据包大小输入函数
    void PutSize(char *pPacket,WORD nPos);
    
    //数据处理函数
    void ReadRecvBuff();
};

#endif /* defined(__testClient__Client__) */
