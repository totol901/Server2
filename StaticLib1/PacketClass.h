#pragma once
using namespace std;
        class Packet
        {
        public:
            virtual PacketType type() = 0;

            virtual void encode(Stream& stream)
            {
                  stream << (Int64)(this->type());
            }

            virtual void decode(Stream& stream)
            {
            }
        };

        class PK_PK_NONE : public Packet
      {
        public:
           PacketType type()  {  return E_PK_NONE;  }
            string             test_;

            void encode(Stream& stream)
            {
                stream << (Int64)(this->type());
                stream << test_;
            }

            void decode(Stream& stream)
            {                stream >> &test_;
            }
      };

        class PK_I_NOTIFY_READY : public Packet
      {
        public:
           PacketType type()  {  return E_I_NOTIFY_READY;  }
            UInt64             oid_;

            void encode(Stream& stream)
            {
                stream << (Int64)(this->type());
                stream << oid_;
            }

            void decode(Stream& stream)
            {                stream >> &oid_;
            }
      };

        class PK_I_DB_REQ_LOGOUT : public Packet
      {
        public:
           PacketType type()  {  return E_I_DB_REQ_LOGOUT;  }
            string             name_;

            void encode(Stream& stream)
            {
                stream << (Int64)(this->type());
                stream << name_;
            }

            void decode(Stream& stream)
            {                stream >> &name_;
            }
      };

        class PK_C_REQ_EXIT : public Packet
      {
        public:
           PacketType type()  {  return E_C_REQ_EXIT;  }
      };

        class PK_S_ANS_EXIT : public Packet
      {
        public:
           PacketType type()  {  return E_S_ANS_EXIT;  }
      };

        class PK_I_NOTIFY_TERMINAL : public Packet
      {
        public:
           PacketType type()  {  return E_I_NOTIFY_TERMINAL;  }
      };

        class PK_C_NOTIFY_HEARTBEAT : public Packet
      {
        public:
           PacketType type()  {  return E_C_NOTIFY_HEARTBEAT;  }
            UInt64             ping_;

            void encode(Stream& stream)
            {
                stream << (Int64)(this->type());
                stream << ping_;
            }

            void decode(Stream& stream)
            {                stream >> &ping_;
            }
      };

        class PK_C_REQ_CREATE_ID : public Packet
      {
        public:
           PacketType type()  {  return E_C_REQ_CREATE_ID;  }
            string             id_;
            string             password_;
            string             name_;

            void encode(Stream& stream)
            {
                stream << (Int64)(this->type());
                stream << id_;
                stream << password_;
                stream << name_;
            }

            void decode(Stream& stream)
            {                stream >> &id_;
                stream >> &password_;
                stream >> &name_;
            }
      };

        class PK_C_ANS_CREATE_ID_FAIL : public Packet
      {
        public:
           PacketType type()  {  return E_C_ANS_CREATE_ID_FAIL;  }
      };

        class PK_C_ANS_CREATE_ID_SUCCESS : public Packet
      {
        public:
           PacketType type()  {  return E_C_ANS_CREATE_ID_SUCCESS;  }
      };

        class PK_I_DB_REQ_CREATE_ID : public Packet
      {
        public:
           PacketType type()  {  return E_I_DB_REQ_CREATE_ID;  }
            UInt64             clientId_;
            string             id_;
            string             password_;
            string             name_;

            void encode(Stream& stream)
            {
                stream << (Int64)(this->type());
                stream << clientId_;
                stream << id_;
                stream << password_;
                stream << name_;
            }

            void decode(Stream& stream)
            {                stream >> &clientId_;
                stream >> &id_;
                stream >> &password_;
                stream >> &name_;
            }
      };

        class PK_I_DB_ANS_CREATE_ID : public Packet
      {
        public:
           PacketType type()  {  return E_I_DB_ANS_CREATE_ID;  }
            UInt64             clientId_;
            Byte             result_;

            void encode(Stream& stream)
            {
                stream << (Int64)(this->type());
                stream << clientId_;
                stream << result_;
            }

            void decode(Stream& stream)
            {                stream >> &clientId_;
                stream >> &result_;
            }
      };

        class PK_C_REQ_ID_PW : public Packet
      {
        public:
           PacketType type()  {  return E_C_REQ_ID_PW;  }
            string             id_;
            string             password_;

            void encode(Stream& stream)
            {
                stream << (Int64)(this->type());
                stream << id_;
                stream << password_;
            }

            void decode(Stream& stream)
            {                stream >> &id_;
                stream >> &password_;
            }
      };

        class PK_S_ANS_ID_PW_FAIL : public Packet
      {
        public:
           PacketType type()  {  return E_S_ANS_ID_PW_FAIL;  }
      };

        class PK_S_ANS_ID_PW_SUCCESS : public Packet
      {
        public:
           PacketType type()  {  return E_S_ANS_ID_PW_SUCCESS;  }
            string             chattingServerIp_;
            UInt32             chattingServerPort_;
            string             gameServerIp_;
            UInt32             gameServerPort_;
            string             name_;

            void encode(Stream& stream)
            {
                stream << (Int64)(this->type());
                stream << chattingServerIp_;
                stream << chattingServerPort_;
                stream << gameServerIp_;
                stream << gameServerPort_;
                stream << name_;
            }

            void decode(Stream& stream)
            {                stream >> &chattingServerIp_;
                stream >> &chattingServerPort_;
                stream >> &gameServerIp_;
                stream >> &gameServerPort_;
                stream >> &name_;
            }
      };

        class PK_I_DB_REQ_ID_PW : public Packet
      {
        public:
           PacketType type()  {  return E_I_DB_REQ_ID_PW;  }
            UInt64             clientId_;
            string             id_;
            string             password_;

            void encode(Stream& stream)
            {
                stream << (Int64)(this->type());
                stream << clientId_;
                stream << id_;
                stream << password_;
            }

            void decode(Stream& stream)
            {                stream >> &clientId_;
                stream >> &id_;
                stream >> &password_;
            }
      };

        class PK_I_DB_ANS_ID_PW : public Packet
      {
        public:
           PacketType type()  {  return E_I_DB_ANS_ID_PW;  }
            UInt64             clientId_;
            UInt64             oidAccountId_;
            Byte             result_;

            void encode(Stream& stream)
            {
                stream << (Int64)(this->type());
                stream << clientId_;
                stream << oidAccountId_;
                stream << result_;
            }

            void decode(Stream& stream)
            {                stream >> &clientId_;
                stream >> &oidAccountId_;
                stream >> &result_;
            }
      };

        class PK_I_CHTTING_NOTIFY_ID : public Packet
      {
        public:
           PacketType type()  {  return E_I_CHTTING_NOTIFY_ID;  }
            UInt64             clientId_;
            UInt64             oidAccountId_;

            void encode(Stream& stream)
            {
                stream << (Int64)(this->type());
                stream << clientId_;
                stream << oidAccountId_;
            }

            void decode(Stream& stream)
            {                stream >> &clientId_;
                stream >> &oidAccountId_;
            }
      };

        class PK_I_DB_REQ_LOAD_DATA : public Packet
      {
        public:
           PacketType type()  {  return E_I_DB_REQ_LOAD_DATA;  }
            UInt64             clientId_;
            UInt64             oidAccountId_;

            void encode(Stream& stream)
            {
                stream << (Int64)(this->type());
                stream << clientId_;
                stream << oidAccountId_;
            }

            void decode(Stream& stream)
            {                stream >> &clientId_;
                stream >> &oidAccountId_;
            }
      };

        class PK_I_DB_ANS_PARSE_DATA : public Packet
      {
        public:
           PacketType type()  {  return E_I_DB_ANS_PARSE_DATA;  }
            UInt64             clientId_;
            string             name_;
            Byte             result_;

            void encode(Stream& stream)
            {
                stream << (Int64)(this->type());
                stream << clientId_;
                stream << name_;
                stream << result_;
            }

            void decode(Stream& stream)
            {                stream >> &clientId_;
                stream >> &name_;
                stream >> &result_;
            }
      };

        class PK_I_LOGIN_NOTIFY_ID_LOADED : public Packet
      {
        public:
           PacketType type()  {  return E_I_LOGIN_NOTIFY_ID_LOADED;  }
            UInt64             clientId_;
            string             name_;
            Byte             result_;

            void encode(Stream& stream)
            {
                stream << (Int64)(this->type());
                stream << clientId_;
                stream << name_;
                stream << result_;
            }

            void decode(Stream& stream)
            {                stream >> &clientId_;
                stream >> &name_;
                stream >> &result_;
            }
      };

        class PK_C_REQ_REGIST_CHATTING_NAME : public Packet
      {
        public:
           PacketType type()  {  return E_C_REQ_REGIST_CHATTING_NAME;  }
            string             name_;

            void encode(Stream& stream)
            {
                stream << (Int64)(this->type());
                stream << name_;
            }

            void decode(Stream& stream)
            {                stream >> &name_;
            }
      };

        class PK_C_REQ_CHATTING : public Packet
      {
        public:
           PacketType type()  {  return E_C_REQ_CHATTING;  }
            string             text_;

            void encode(Stream& stream)
            {
                stream << (Int64)(this->type());
                stream << text_;
            }

            void decode(Stream& stream)
            {                stream >> &text_;
            }
      };

        class PK_S_ANS_CHATTING : public Packet
      {
        public:
           PacketType type()  {  return E_S_ANS_CHATTING;  }
            string             name_;
            string             text_;

            void encode(Stream& stream)
            {
                stream << (Int64)(this->type());
                stream << name_;
                stream << text_;
            }

            void decode(Stream& stream)
            {                stream >> &name_;
                stream >> &text_;
            }
      };

        class PK_C_REQ_GAME_START : public Packet
      {
        public:
           PacketType type()  {  return E_C_REQ_GAME_START;  }
      };

        class PK_C_ANS_GAME_START : public Packet
      {
        public:
           PacketType type()  {  return E_C_ANS_GAME_START;  }
      };

        class PK_C_MOVE_START_INPUT : public Packet
      {
        public:
           PacketType type()  {  return E_C_MOVE_START_INPUT;  }
            Int32             state_;
            float             PosX_;
            float             PosY_;
            float             PosZ_;
            float             quatX_;
            float             quatY_;
            float             quatZ_;
            float             quatW_;
            UInt32             moveNum_;

            void encode(Stream& stream)
            {
                stream << (Int64)(this->type());
                stream << state_;
                stream << PosX_;
                stream << PosY_;
                stream << PosZ_;
                stream << quatX_;
                stream << quatY_;
                stream << quatZ_;
                stream << quatW_;
                stream << moveNum_;
            }

            void decode(Stream& stream)
            {                stream >> &state_;
                stream >> &PosX_;
                stream >> &PosY_;
                stream >> &PosZ_;
                stream >> &quatX_;
                stream >> &quatY_;
                stream >> &quatZ_;
                stream >> &quatW_;
                stream >> &moveNum_;
            }
      };

        class PK_S_MOVE_START : public Packet
      {
        public:
           PacketType type()  {  return E_S_MOVE_START;  }
            Int32             state_;
            UInt64             oid_;
            float             PosX_;
            float             PosY_;
            float             PosZ_;
            float             quatX_;
            float             quatY_;
            float             quatZ_;
            float             quatW_;
            UInt32             moveNum_;

            void encode(Stream& stream)
            {
                stream << (Int64)(this->type());
                stream << state_;
                stream << oid_;
                stream << PosX_;
                stream << PosY_;
                stream << PosZ_;
                stream << quatX_;
                stream << quatY_;
                stream << quatZ_;
                stream << quatW_;
                stream << moveNum_;
            }

            void decode(Stream& stream)
            {                stream >> &state_;
                stream >> &oid_;
                stream >> &PosX_;
                stream >> &PosY_;
                stream >> &PosZ_;
                stream >> &quatX_;
                stream >> &quatY_;
                stream >> &quatZ_;
                stream >> &quatW_;
                stream >> &moveNum_;
            }
      };

        class PK_C_REQ_JOIN_MAP : public Packet
      {
        public:
           PacketType type()  {  return E_C_REQ_JOIN_MAP;  }
            string             name_;

            void encode(Stream& stream)
            {
                stream << (Int64)(this->type());
                stream << name_;
            }

            void decode(Stream& stream)
            {                stream >> &name_;
            }
      };

        class PK_C_ANS_JOIN_MAP : public Packet
      {
        public:
           PacketType type()  {  return E_C_ANS_JOIN_MAP;  }
            string             name_;
            UInt64             oid_;
            Int32             isRedTeam_;
            float             posX_;
            float             posY_;
            float             posZ_;
            float             quatX_;
            float             quatY_;
            float             quatZ_;
            float             quatW_;

            void encode(Stream& stream)
            {
                stream << (Int64)(this->type());
                stream << name_;
                stream << oid_;
                stream << isRedTeam_;
                stream << posX_;
                stream << posY_;
                stream << posZ_;
                stream << quatX_;
                stream << quatY_;
                stream << quatZ_;
                stream << quatW_;
            }

            void decode(Stream& stream)
            {                stream >> &name_;
                stream >> &oid_;
                stream >> &isRedTeam_;
                stream >> &posX_;
                stream >> &posY_;
                stream >> &posZ_;
                stream >> &quatX_;
                stream >> &quatY_;
                stream >> &quatZ_;
                stream >> &quatW_;
            }
      };

        class PK_C_SHELL_SHOOT_INPUT : public Packet
      {
        public:
           PacketType type()  {  return E_C_SHELL_SHOOT_INPUT;  }
            float             PosX_;
            float             PosY_;
            float             PosZ_;
            float             quatX_;
            float             quatY_;
            float             quatZ_;
            float             quatW_;
            float             speed_;

            void encode(Stream& stream)
            {
                stream << (Int64)(this->type());
                stream << PosX_;
                stream << PosY_;
                stream << PosZ_;
                stream << quatX_;
                stream << quatY_;
                stream << quatZ_;
                stream << quatW_;
                stream << speed_;
            }

            void decode(Stream& stream)
            {                stream >> &PosX_;
                stream >> &PosY_;
                stream >> &PosZ_;
                stream >> &quatX_;
                stream >> &quatY_;
                stream >> &quatZ_;
                stream >> &quatW_;
                stream >> &speed_;
            }
      };

        class PK_S_SHELL_SHOOT : public Packet
      {
        public:
           PacketType type()  {  return E_S_SHELL_SHOOT;  }
            UInt64             oid_;
            float             PosX_;
            float             PosY_;
            float             PosZ_;
            float             quatX_;
            float             quatY_;
            float             quatZ_;
            float             quatW_;
            float             speed_;

            void encode(Stream& stream)
            {
                stream << (Int64)(this->type());
                stream << oid_;
                stream << PosX_;
                stream << PosY_;
                stream << PosZ_;
                stream << quatX_;
                stream << quatY_;
                stream << quatZ_;
                stream << quatW_;
                stream << speed_;
            }

            void decode(Stream& stream)
            {                stream >> &oid_;
                stream >> &PosX_;
                stream >> &PosY_;
                stream >> &PosZ_;
                stream >> &quatX_;
                stream >> &quatY_;
                stream >> &quatZ_;
                stream >> &quatW_;
                stream >> &speed_;
            }
      };

        class PK_C_SHEEL_DEMAGE_INPUT : public Packet
      {
        public:
           PacketType type()  {  return E_C_SHEEL_DEMAGE_INPUT;  }
            UInt64             oid_;
            float             demage_;

            void encode(Stream& stream)
            {
                stream << (Int64)(this->type());
                stream << oid_;
                stream << demage_;
            }

            void decode(Stream& stream)
            {                stream >> &oid_;
                stream >> &demage_;
            }
      };

        class PK_S_SHEEL_DEMAGE : public Packet
      {
        public:
           PacketType type()  {  return E_S_SHEEL_DEMAGE;  }
            UInt64             oid_;
            float             demage_;

            void encode(Stream& stream)
            {
                stream << (Int64)(this->type());
                stream << oid_;
                stream << demage_;
            }

            void decode(Stream& stream)
            {                stream >> &oid_;
                stream >> &demage_;
            }
      };

        class PK_C_CHECK_ALREADY_LOGIN : public Packet
      {
        public:
           PacketType type()  {  return E_C_CHECK_ALREADY_LOGIN;  }
            string             name_;
            UInt64             clientId_;

            void encode(Stream& stream)
            {
                stream << (Int64)(this->type());
                stream << name_;
                stream << clientId_;
            }

            void decode(Stream& stream)
            {                stream >> &name_;
                stream >> &clientId_;
            }
      };

        class PK_S_CHECK_ALREADY_LOGIN_SUCCESS : public Packet
      {
        public:
           PacketType type()  {  return E_S_CHECK_ALREADY_LOGIN_SUCCESS;  }
            string             name_;
            UInt64             clientId_;

            void encode(Stream& stream)
            {
                stream << (Int64)(this->type());
                stream << name_;
                stream << clientId_;
            }

            void decode(Stream& stream)
            {                stream >> &name_;
                stream >> &clientId_;
            }
      };

        class PK_S_CHECK_ALREADY_LOGIN_FAIL : public Packet
      {
        public:
           PacketType type()  {  return E_S_CHECK_ALREADY_LOGIN_FAIL;  }
            UInt64             clientId_;

            void encode(Stream& stream)
            {
                stream << (Int64)(this->type());
                stream << clientId_;
            }

            void decode(Stream& stream)
            {                stream >> &clientId_;
            }
      };


