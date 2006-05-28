//$Id$
#include "NetServer.h"
using namespace mones;
using namespace MonAPI;

NetServer::NetServer() : 
    next_port(0), observerThread(0xffffffff),timerid(0),
    nic(NULL), started(false), loopExit(false)
{

}

NetServer::~NetServer()
{
    if( (nic != NULL) && (nic->getIRQ() != 0 ) ){
        syscall_remove_irq_receiver(this->nic->getIRQ());
        delete nic;
    }
    if( timerid != 0)
        kill_timer(timerid);
    if( ipstack != NULL)
        delete ipstack;
}

bool NetServer::initialize()
{
    syscall_get_io();
    this->nic = NicFactory::create();
    if(this->nic == NULL){
        printf("NicFactory error\n");
        return false;
    }
    if( nic->getIRQ()!=0 ) // with mask Interrrupt by higepon
        syscall_set_irq_receiver(this->nic->getIRQ(), SYS_MASK_INTERRUPT); 
    this->nic->enableNetwork();
    this->ipstack = new IPStack();
    this->observerThread= Message::lookupMainThread();
    this->myID = System::getThreadID();
    timerid=set_timer(5000);
    return true;
}

dword NetServer::getThreadID() const
{
    return this->myID;
}

void NetServer::ICMPreply(IP* pkt)
{
    Ether* rframe = nic->MakePKT(pkt->srcip);
    //ether header has been already filled.
    if( rframe != NULL){
        ICMP* icmp=rframe->IPHeader->ICMPHeader;
        //FillICMPHeader();
        //create ICMP echo reply.
        icmp->type=ECHOREPLY;
        icmp->code=0x00;
        icmp->chksum=0x0000;
        memcpy(icmp->data,pkt->ICMPHeader->data,bswap(pkt->len)-sizeof(IP)-sizeof(ICMP));
        icmp->chksum=bswap(ipstack->checksum((byte*)icmp,bswap(pkt->len)-sizeof(IP)));
        printf("replying.\n");    
        ipstack->FillIPHeader(rframe->IPHeader);
        nic->Send(rframe);
    }
}

void NetServer::messageLoop()
{
    this->started = true;
    for (MessageInfo msg; !loopExit;){
        if (Message::receive(&msg)) continue;
        switch (msg.header)
        {
        case MSG_INTERRUPTED:
            this->interrupt(&msg);   
            break;
        case MSG_NET_GETFREEPORT:
            this->getfreeport(&msg);
            break;
        case MSG_NET_WRITE:
            this->write(&msg);
            break;
        case MSG_NET_READ:
            this->read(&msg);
            break;
        case MSG_NET_STATUS:
            this->status(&msg);
            break;
        case MSG_NET_OPEN:
            this->open(&msg);
            break;
        case MSG_NET_CLOSE:
            this->close(&msg);
            break;
        case MSG_NET_CONFIG:
            this->config(&msg);
            break;
        case MSG_TIMER:
            this->ontimer(&msg);
            break;
        default:
            printf("NetServer::MessageLoop default MSG=%x\n", msg.header);
            break;
        }
    }
    printf("NetServer exit\n");
}

/*typedef struct {
    dword header;
    dword arg1;
    dword arg2;
    dword arg3;
    dword from;
    char str[128];
    int length;
} MessageInfo; */

//////////// MESSAGE HANDLERS ////////////////////////
void NetServer::config(MessageInfo* msg)
{
    printf("configure the net server.\n");
    Message::reply(msg);
}

void NetServer::getfreeport(MessageInfo* msg)
{
    next_port++;
    if( next_port <= 0x400)
        next_port=0x401;
    Message::reply(msg,next_port);
}

void NetServer::open(MessageInfo* msg)
{    
    ConnectionInfo* c=new ConnectionInfo();
    cinfolist.add(c);
    c->Id.remoteip   = (word)((msg->arg1)>>16);
    c->Id.remoteport = (word)(msg->arg2|0xFFFF);
    c->Id.localport  = msg->arg2;
    c->Id.protocol   = msg->arg3;
    c->clientid   = msg->from;    
    c->netdsc     = cinfolist.size();    
    Message::reply(msg, c->netdsc);
}

void NetServer::close(MessageInfo* msg)
{
    dword ret=1;
    for(int i=0; i<cinfolist.size(); i++){
        ConnectionInfo* c  = cinfolist.get(i);
        if( c->netdsc == msg->arg1 ){
            delete cinfolist.removeAt(i);
            i--;
            ret=0;
        }
    }
    Message::reply(msg,ret);
}

void NetServer::status(MessageInfo* msg)
{
    NetStatus stat;
    nic->getStatus(&stat);
    monapi_cmemoryinfo* mi = monapi_cmemoryinfo_new();  
    if (mi != NULL){
        monapi_cmemoryinfo_create(mi, sizeof(NetStatus)/*+sizeof(arpcache)*N*/, true);        
        if( mi != NULL ){
            memcpy(mi->Data,&stat,mi->Size);
            Message::reply(msg, mi->Handle, mi->Size);
        }
        monapi_cmemoryinfo_delete(mi);
    }else{
        Message::reply(msg);
    }
}

void NetServer::ontimer(MessageInfo* msg)
{
   //printf("TIMER\n");
}

void NetServer::interrupt(MessageInfo* msg)
{   
    //Don't say anything about in case mona is a router.
    int val = nic->interrupt();
    if( val & Nic::RX_INT ){
        printf("=RX\n");
        Dispatch();
    }
    if(val & Nic::TX_INT){
        printf("=TX\n");
    }
    if( val & Nic::ER_INT){
        printf("=ERROR.\n");    
    }
}

void NetServer::Dispatch()
{
    Ether* frame =NULL;
    //TODO handling two or more packet. 
    while( frame = nic ->Recv(0) ){
        IP* pkt=frame->IPHeader;
        ipstack->dumpPacket(pkt);
        if( pkt->prot == TYPEICMP && pkt->ICMPHeader->type==ECHOREQUEST){
            ICMPreply(pkt);
        }
        ConnectionInfo* cinfo=NULL;
        //find a waiting client for current packt.
        for(int i=0; i<cinfolist.size(); i++){
            cinfo  = cinfolist.get(i);
            if ( ipstack->Match((byte*)&(cinfo->Id),pkt ) ){ 
                break;
            }
        }
        if( cinfo != NULL ){
            byte val[]="string";   
            printf("noblock=%d\n",cinfo->msg.arg2);
            monapi_cmemoryinfo* mi = monapi_cmemoryinfo_new();  
            if (mi != NULL){    
                monapi_cmemoryinfo_create(mi,7, true);        
                if( mi != NULL ){
                    memcpy(mi->Data,val,mi->Size);mi->Data[6]='\0';
                    Message::reply(&(cinfo->msg), mi->Handle, mi->Size); 
                }
                monapi_cmemoryinfo_delete(mi);
            }else{
                Message::reply(&(cinfo->msg));
            }
            memset(&(cinfo->msg),'\0',sizeof(MessageInfo));
            cinfo=NULL;
        }
        delete frame;
    }
}

void NetServer::read(MessageInfo* msg)
{
    //Register msg to waiting client list.
    for(int i=0; i<cinfolist.size(); i++){
        ConnectionInfo* c  = cinfolist.get(i);
        if( c->netdsc == msg->arg1 ){
            memcpy(&(c->msg),(byte*)msg,sizeof(MessageInfo));
            break;
        }
    }
    Dispatch();
}

void NetServer::write(MessageInfo* msg)
{ 
    monapi_cmemoryinfo* ret = monapi_cmemoryinfo_new();
    if( ret != NULL){
        ret->Handle = msg->arg2;
        ret->Owner  = msg->from;
        ret->Size   = msg->arg3;
        monapi_cmemoryinfo_map(ret);
        byte* buf=new byte[ret->Size+1];
        memcpy(buf,ret->Data,ret->Size); buf[ret->Size]='\0';
        printf("<%s>\n",buf);
        //send the data.
        monapi_cmemoryinfo_delete(ret);
        delete buf;
    }
    Message::reply(msg);  
}
