/*!
    \file   MoPacUtl.cpp
    \brief  Mona パケットユーティリティクラス

    Copyright (c) 2004 Yamami
    All rights reserved.
    License=MIT/X License

    \author  Yamami
    \version $Revision$
    \date   create:2004/08/18 update:$Date$
*/

/*! \class MoPacUtl
 *  \brief Mona パケットユーティリティクラス
 */



#include "MoPacUtl.h"


/*!
    \brief initialize
         MoPacUtl コンストラクタ
    \author Yamami
    \date   create:2004/08/12 update:
*/
MoPacUtl::MoPacUtl()
{

}

/*!
    \brief initialize
         MoEther init
    \author Yamami
    \date   create:2004/08/12 update:
*/



/*!
    \brief initialize
         MoPacUtl デスクトラクタ
    \author Yamami
    \date   create:2004/08/12 update:
*/
MoPacUtl::~MoPacUtl() 
{

}


/*!
    \brief packet_get_4byte
         4バイトパケット取得
         ネットワークエンディアン変換
    \param  byte *buf [in] 対象バッファ
    \param  int offset [in] 対象オフセット
    \return dword 取得値
    
    \author Yamami
    \date   create:2004/08/18 update:
*/
dword MoPacUtl::packet_get_4byte(byte *buf, int offset)
{
    dword a, b, c;

    a = (dword)buf[offset++] << 24;
    b = (dword)buf[offset++] << 16;
    c = (dword)buf[offset++] << 8;

    return a | b | c | buf[offset];
}

/*!
    \brief packet_get_2byte
         2バイトパケット取得
         ネットワークエンディアン変換
    \param  byte *buf [in] 対象バッファ
    \param  int offset [in] 対象オフセット
    \return word 取得値
    
    \author Yamami
    \date   create:2004/08/18 update:
*/
word MoPacUtl::packet_get_2byte(byte *buf, dword offset)
{
    word a;

    a = buf[offset++] << 8;
    
    return a | buf[offset];
}


/*!
    \brief packet_put_4byte
         4バイトパケット設定
         ネットワークエンディアン変換
    \param  byte *buf [in] 対象バッファ
    \param  int offset [in] 対象オフセット
    \param  idword [in] 設定値
    \return void 無し
    
    \author Yamami
    \date   create:2004/08/18 update:
*/
void MoPacUtl::packet_put_4byte(byte* buf, int offset, dword val)
{
    buf += offset;
    *buf++ = (val >> 24) & 0xff;
    *buf++ = (val >> 16) & 0xff;
    *buf++ = (val >>  8) & 0xff;
    *buf = val & 0xff;
}

/*!
    \brief packet_put_2byte
         2バイトパケット設定
         ネットワークエンディアン変換
    \param  byte *buf [in] 対象バッファ
    \param  int offset [in] 対象オフセット
    \param  iword [in] 設定値
    \return void 無し
    
    \author Yamami
    \date   create:2004/08/18 update:
*/
void MoPacUtl::packet_put_2byte(byte* buf, int offset, word val)
{
    buf += offset;
    *buf++ = (val >> 8) & 0xff;
    *buf = val & 0xff;
}


