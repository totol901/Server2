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
            string             ip_;
            UInt32             port_;
            string             name_;

            void encode(Stream& stream)
            {
                stream << (Int64)(this->type());
                stream << ip_;
                stream << port_;
                stream << name_;
            }

            void decode(Stream& stream)
            {                stream >> &ip_;
                stream >> &port_;
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
