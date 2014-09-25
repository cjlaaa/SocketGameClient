//
//  Client.cpp
//  testClient
//
//  Created by hENRYcHANG on 14-9-12.
//
//

#include "Client.h"
#include "HelloWorldScene.h"

Client* Client::CreateClient(Node *Scene)
{
    Client * ret = new Client();
    if (ret && ret->init(Scene))
    {
        ret->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(ret);
    }
	return ret;
}

bool Client::init(Node* Scene)
{
    if(Node::init()==false)
    {
        ClientLog("Client init Error!\n");
        return false;
    }
    
    //变量初始化
    m_recvPos = 0;
    m_recvSize = 0;
    *m_recvBuff = NULL;
    m_Scene = Scene;
    
    return true;
}

void Client::update(float fT)
{
    //log("running...");
    RecvData();
    ReadRecvBuff();
}

//连接处理函数
BOOL Client::ConnectToServer(char *host,int port)
{
#ifdef WIN32
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2,2),&wsa)!=0)
	{
       return false;
    }
#endif
    struct sockaddr_in s_addr_in;
    
    //套接字的创建
    m_sock = socket(AF_INET, SOCK_STREAM, 0);
    
    if (m_sock==INVALID_SOCKET) return false;
    
    s_addr_in.sin_family = AF_INET;
    s_addr_in.sin_port = htons(port);
    s_addr_in.sin_addr.s_addr = inet_addr(host);
    
    //connect
    if (connect(m_sock, (struct sockaddr*)&s_addr_in, sizeof(s_addr_in))!=0) return false;
    ClientLog("Connetct Server Success!\n");
   
    //socket Nonblocking
    NonBlock(m_sock);
    
    m_nAliveCheckDelayTime = dALIVE_CHECK_DELAY_TIME;
    return true;
}

#ifdef WIN32
void Client::NonBlock(SOCKET s)
{
    u_long u10n = 1L;
    
	ioctlsocket(s,FIONBIO,(unsigned long*)&u10n);
}
#else
//设置Linux,FreeBSD用套接字为nonblock模式
void Client::NonBlock(SOCKET s)
{
    int flags;
    
    flags = fcntl(s, F_GETFL,0);
    flags |= O_NONBLOCK;
    if (fcntl(s, F_SETFL,flags) < 0) log("* nonblock Error\r\n");
}
#endif

//聊天结束
void Client::CloseChat()
{
    if (m_sock!=INVALID_SOCKET) closesocket(m_sock);
}

//数据Recv
void Client::RecvData()
{
    fd_set read_set;
    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 0;
    
    FD_ZERO(&read_set);
    FD_SET(m_sock, &read_set);
    
    if (select(m_sock+1, &read_set, NULL, NULL, &tv)<0) return;
    
    if (FD_ISSET(m_sock, &read_set))
    {
        int recvSize;
        char recvBuff[dMAX_SOCK_BUFF];
        
        recvSize = recv(m_sock,recvBuff,1024,0);
        
        if (recvSize==0)
        {
            ClientLog("RecvData Error!Connection Failed...");
            return;
        }
        
        if (recvSize<0)
        {
            //发生NonBlock外错误时,断开连接
#if defined(WIN32)
            if (WSAGetLastError()!=WSAEWOULDBLOCK)
            {
                ClientLog("WSAGetLastError()!=WSAEWOULDBLOCK Error!");
                return;
            }
#else
            if (errno!=EWOULDBLOCK)
            {
                ClientLog("errno!=EWOULDBLOCK Error!");
                return;
            }
#endif
        }
        
        //Buffer Overflow
        if ((m_recvSize + recvSize)>=dMAX_SOCK_BUFF)
        {
            ClientLog("Buffer Overflow Error!");
            return;
        }
        
        memcpy(&m_recvBuff[m_recvSize], recvBuff, recvSize);
        m_recvSize += recvSize;
    }
}

//在pPacket参数的nPos位置放置BYTE类型的value值
void Client::PutByte(char *pPacket,BYTE value,int &nPos)
{
    *(BYTE*)(pPacket + nPos) = value;
    nPos += sizeof(BYTE);
}

//把位于pPacket参数nPos位置的值以BYTE类型返回
BYTE Client::GetByte(char *pPacket,int &nPos)
{
    BYTE value = *(BYTE*)(pPacket + nPos);
    nPos += sizeof(BYTE);
    return value;
}

//在pPacket参数的nPos位置放置WORD类型的value值
void Client::PutWord(char *pPacket,WORD value,int &nPos)
{
    *(WORD*)(pPacket + nPos) = value;
    nPos += sizeof(WORD);
}

//把位于pPacket参数nPos位置的值以WORD类型返回
WORD Client::GetWord(char *pPacket,int &nPos)
{
    WORD value = *(WORD*)(pPacket + nPos);
    nPos += sizeof(WORD);
    return value;
}

//在pPacket参数的nPos位置放置DWORD类型的value值
void Client::PutDword(char *pPacket,DWORD value,int &nPos)
{
    *(DWORD*)(pPacket + nPos) = value;
    nPos += sizeof(DWORD);
}

//把位于pPacket参数nPos位置的值以DWORD类型返回
DWORD Client::GetDword(char *pPacket,int &nPos)
{
    DWORD value = *(DWORD*)(pPacket + nPos);
    nPos += sizeof(DWORD);
    return value;
}

//在pPacket参数的nPos位置放置int类型的value值
void Client::PutInteger(char *pPacket,int value,int &nPos)
{
    *(int*)(pPacket + nPos) = value;
    nPos += sizeof(int);
}

//把位于pPacket参数nPos位置的值以int类型返回
int Client::GetInteger(char *pPacket,int &nPos)
{
    int value = *(int*)(pPacket + nPos);
    nPos += sizeof(int);
    return value;
}

//在pPacket参数的nPos位置放置short类型的value值
void Client::PutShort(char *pPacket,short value,int &nPos)
{
    *(short*)(pPacket + nPos) = value;
    nPos += sizeof(short);
}

//把位于pPacket参数nPos位置的值以short类型返回
short Client::GetShort(char *pPacket,int &nPos)
{
    short value = *(short*)(pPacket + nPos);
    nPos += sizeof(short);
    return value;
}

//在pPacket参数的nPos位置放置字符串
void Client::PutString(char *pPacket,char* str,int &nPos)
{
    *(WORD*)(pPacket + nPos) = strlen(str);
    nPos += sizeof(WORD);
    memcpy(pPacket + nPos, str, strlen(str));
    nPos += strlen(str);
}

//把位于pPacket参数nPos位置的字符串数据复制到pBuffer(包括NULL的处理)
void Client::GetString(char *pPacket,char* pBuffer,int &nPos)
{
    WORD BufferLength;
    
    BufferLength = *(WORD*)(pPacket + nPos);
    nPos += sizeof(WORD);
    memcpy(pBuffer, pPacket + nPos, BufferLength);
    *(pBuffer + BufferLength) = '\0';
    nPos += BufferLength;
}

//设置数据包的总长度
void Client::PutSize(char *pPacket,WORD nPos)
{
    *(WORD*)pPacket = nPos;
}

//数据处理函数
void Client::ReadRecvBuff()
{
    WORD msgSize;
    WORD tag;
    
    while (m_recvSize > 0)
    {
        m_recvPos = 0;
        
        msgSize = GetWord(m_recvBuff, m_recvPos);
        tag = GetWord(m_recvBuff, m_recvPos);
        
        //当目前m_recvBuff内数据长度小于数据包总长度时
        if (m_recvSize < msgSize) return;
        
        //数据包处理
        switch (tag)
        {
            case dPACKET_SEND_MSG:
                OnPacketChatMsg();
                break;
            default:
                break;
        }
        
        memmove(&m_recvBuff[0], &m_recvBuff[msgSize], m_recvSize - msgSize);
        m_recvSize -= msgSize;
    }
}

//聊天消息处理
void Client::OnPacketChatMsg()
{
    char chatMsg[1024];
    
    GetString(m_recvBuff, chatMsg, m_recvPos);
    
    ((HelloWorld*)m_Scene)->GetServerMsg(chatMsg);
}

//发送聊天消息
void Client::OnSendChatMsg(std::string strMsg)
{
    if (strMsg.empty())
    {
        return;
    }
    
    int packetPos;
    char packet[dMAX_SOCK_BUFF];
    
    packetPos = 2;
    PutWord(packet, dPACKET_RECV_MSG, packetPos);
    PutString(packet,(char*)strMsg.c_str(),packetPos);
    PutSize(packet,packetPos);
    
    send(m_sock,packet,packetPos,0);
}

//修改昵称
void Client::OnSendChangeNick(char* nick)
{
    int packetPos;
    char packet[dMAX_SOCK_BUFF];
    
    packetPos = 2;
    PutWord(packet, dPACKET_CHANGE_NICK, packetPos);
    PutString(packet,nick, packetPos);
    PutSize(packet, packetPos);
    
    send(m_sock, packet, packetPos, 0);
}

//Keep Alive
void Client::OnSendAliveCheck()
{
    if (m_nAliveCheckDelayTime > 0)
    {
        m_nAliveCheckDelayTime--;
    }
    else
    {
        m_nAliveCheckDelayTime = dALIVE_CHECK_DELAY_TIME;
        
        char packet[128];
        int packetPos;
        
        packetPos = 2;
        PutWord(packet, dPACKET_ALIVE_CHECK, packetPos);
        PutSize(packet, packetPos);
        send(m_sock, packet, packetPos, 0);
    }
}

void Client::ClientLog(char *buf,...)
{
    char tmp_buf[1024],tmp_buf2[1024];
    va_list args;
    
    time_t tval;
    time(&tval);
    struct tm *currTM = localtime(&tval);
    
    va_start(args,buf);
    vsprintf(tmp_buf,buf,args);
    va_end(args);
    
    sprintf(tmp_buf2,"[%d/%02d/%02d %02d:%02d:%02d]%s",
            currTM->tm_year+1900,
            currTM->tm_mon+1,
            currTM->tm_mday,
            currTM->tm_hour,
            currTM->tm_min,
            currTM->tm_sec,
            tmp_buf);
    
    strcpy(tmp_buf,tmp_buf2);
    
    printf(tmp_buf);
}
