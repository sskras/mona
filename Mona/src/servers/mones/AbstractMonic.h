/*!
    \file  AbstractMonic.h
    \brief 抽象NICクラス ヘッダ
    
    Copyright (c) 2004 Yamami
    WITHOUT ANY WARRANTY

    \author  Yamami
    \version $Revision$
    \date   create:2004/09/04 update:$Date$
*/
#ifndef _MONA_ABSTRACT_MONIC_
#define _MONA_ABSTRACT_MONIC_

#include <sys/types.h>

/*!
    abstract class AbstractMonic
*/
class AbstractMonic {

  private:

  protected:


  public:
  
    //Publicメンバ
    /*! \brief NE2000 テンポラリ受信バッファ */ 
    byte   frame_buf[1500];
    
    /*! \brief MACアドレス */ 
    byte   ether_mac_addr[6];
    /*! \brief 受信パケット本体の長さ */ 
    dword    frame_len;
    
    AbstractMonic();
    virtual ~AbstractMonic();
    
    virtual void frame_input(void) = 0;
    virtual void frame_output( byte *, byte *, dword, word ) = 0;
    virtual int nic_probe(void) = 0;
    virtual void nic_init(void) = 0;
    virtual void enableNetWork(void) = 0;
    virtual void disableNetWork(void) = 0;
    virtual int getNicIRQ() = 0;
};

#endif
