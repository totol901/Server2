#pragma once

        #define PACKET_MAKE_DATE "2020/56/05 11:56:23"
        enum PacketType :  Int64
        {
              /*0*/    E_PK_NONE = 0,
              /*1*/    E_I_NOTIFY_READY = 1,
              /*2*/    E_C_REQ_EXIT = 2,
              /*3*/    E_S_ANS_EXIT = 3,
              /*4*/    E_I_NOTIFY_TERMINAL = 4,
              /*5*/    E_C_NOTIFY_HEARTBEAT = 5,
              /*6*/    E_C_REQ_CREATE_ID = 6,
              /*7*/    E_C_ANS_CREATE_ID_FAIL = 7,
              /*8*/    E_C_ANS_CREATE_ID_SUCCESS = 8,
              /*9*/    E_I_DB_REQ_CREATE_ID = 9,
              /*10*/    E_I_DB_ANS_CREATE_ID = 10,
              /*11*/    E_C_REQ_ID_PW = 11,
              /*12*/    E_S_ANS_ID_PW_FAIL = 12,
              /*13*/    E_S_ANS_ID_PW_SUCCESS = 13,
              /*14*/    E_I_DB_REQ_ID_PW = 14,
              /*15*/    E_I_DB_ANS_ID_PW = 15,
              /*16*/    E_I_CHTTING_NOTIFY_ID = 16,
              /*17*/    E_I_DB_REQ_LOAD_DATA = 17,
              /*18*/    E_I_DB_ANS_PARSE_DATA = 18,
              /*19*/    E_I_LOGIN_NOTIFY_ID_LOADED = 19,
              /*20*/    E_C_REQ_REGIST_CHATTING_NAME = 20,
              /*21*/    E_C_REQ_CHATTING = 21,
              /*22*/    E_S_ANS_CHATTING = 22,
              /*23*/    E_C_REQ_GAME_START = 23,
              /*24*/    E_C_ANS_GAME_START = 24,
              /*25*/    E_C_MOVE_START_INPUT = 25,
              /*26*/    E_S_MOVE_START = 26,
              /*27*/    E_C_REQ_JOIN_MAP = 27,
              /*28*/    E_C_ANS_JOIN_MAP = 28,
              /*29*/    E_C_SHELL_SHOOT_INPUT = 29,
              /*30*/    E_S_SHELL_SHOOT = 30,
              /*31*/    E_C_SHEEL_DEMAGE_INPUT = 31,
              /*32*/    E_S_SHEEL_DEMAGE = 32,
        };
    

