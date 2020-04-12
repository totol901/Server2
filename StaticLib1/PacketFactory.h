#pragma once

        class PacketFactory : public Singleton<PacketFactory>
        {
            friend Singleton;
        public:
            Packet* getPacket(Int64 packetType)
            {
                 switch(packetType)
                 {
                    case E_PK_NONE: return new PK_PK_NONE();
                    case E_I_NOTIFY_READY: return new PK_I_NOTIFY_READY();
                    case E_C_REQ_EXIT: return new PK_C_REQ_EXIT();
                    case E_S_ANS_EXIT: return new PK_S_ANS_EXIT();
                    case E_I_NOTIFY_TERMINAL: return new PK_I_NOTIFY_TERMINAL();
                    case E_C_NOTIFY_HEARTBEAT: return new PK_C_NOTIFY_HEARTBEAT();
                    case E_C_REQ_CREATE_ID: return new PK_C_REQ_CREATE_ID();
                    case E_C_ANS_CREATE_ID_FAIL: return new PK_C_ANS_CREATE_ID_FAIL();
                    case E_C_ANS_CREATE_ID_SUCCESS: return new PK_C_ANS_CREATE_ID_SUCCESS();
                    case E_I_DB_REQ_CREATE_ID: return new PK_I_DB_REQ_CREATE_ID();
                    case E_I_DB_ANS_CREATE_ID: return new PK_I_DB_ANS_CREATE_ID();
                    case E_C_REQ_ID_PW: return new PK_C_REQ_ID_PW();
                    case E_S_ANS_ID_PW_FAIL: return new PK_S_ANS_ID_PW_FAIL();
                    case E_S_ANS_ID_PW_SUCCESS: return new PK_S_ANS_ID_PW_SUCCESS();
                    case E_I_DB_REQ_ID_PW: return new PK_I_DB_REQ_ID_PW();
                    case E_I_DB_ANS_ID_PW: return new PK_I_DB_ANS_ID_PW();
                    case E_I_CHTTING_NOTIFY_ID: return new PK_I_CHTTING_NOTIFY_ID();
                    case E_I_DB_REQ_LOAD_DATA: return new PK_I_DB_REQ_LOAD_DATA();
                    case E_I_DB_ANS_PARSE_DATA: return new PK_I_DB_ANS_PARSE_DATA();
                    case E_I_LOGIN_NOTIFY_ID_LOADED: return new PK_I_LOGIN_NOTIFY_ID_LOADED();
                    case E_C_REQ_REGIST_CHATTING_NAME: return new PK_C_REQ_REGIST_CHATTING_NAME();
                    case E_C_REQ_CHATTING: return new PK_C_REQ_CHATTING();
                    case E_S_ANS_CHATTING: return new PK_S_ANS_CHATTING();
                 }
                 return nullptr;
             }
        };
    

