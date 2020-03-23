#pragma once
#include "stdafx.h"

#define PACKET_MAKE_DATE "2020/31/23 11:31:07" 
enum PacketType : Int64 {
    /*0*/    E_C_REQ_EXIT = 0,
    /*1*/    E_S_ANS_EXIT = 1,
    /*2*/    E_I_NOTIFY_TERMINAL = 2,
    /*3*/    E_C_NOTIFY_HEARTBEAT = 3,
    /*4*/    E_C_REQ_ID_PW = 4,
    /*5*/    E_S_ANS_ID_PW_FAIL = 5,
    /*6*/    E_S_ANS_ID_PW_SUCCESS = 6,
    /*7*/    E_I_DB_REQ_ID_PW = 7,
    /*8*/    E_I_DB_ANS_ID_PW = 8,
    /*9*/    E_I_CHTTING_NOTIFY_ID = 9,
    /*10*/    E_I_DB_REQ_LOAD_DATA = 10,
    /*11*/    E_I_DB_ANS_PARSE_DATA = 11,
    /*12*/    E_I_LOGIN_NOTIFY_ID_LOADED = 12,
    /*13*/    E_C_REQ_REGIST_CHATTING_NAME = 13,
    /*14*/    E_C_REQ_CHATTING = 14,
    /*15*/    E_S_ANS_CHATTING = 15,
};
