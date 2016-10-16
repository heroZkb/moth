#ifndef _MESSENGER_H_
#define _MESSENGER_H_

#include <list>
#include <set>
#include "msg_types.h"
#include "message.h"
#include "dispatcher.h"

// ͨ�Ż���
class CMessenger
{
public:
	CMessenger();

	CMessenger(entity_name_t entityname) : entity_(), magic_(0)
	{
		entity_.name_ = entityname;
	}
	
	virtual ~CMessenger() {};

	// 
	static CMessenger* create(std::string type, entity_name_t name, std::string lname, uint64_t nonce = 0, uint64_t features = 0);

	const entity_inst_t& get_entity() { return entity_; }

	void set_entity(entity_inst_t e) { entity_ = e; }

	uint32_t get_magic() { return magic_; }
	
	void set_magic(uint32_t magic) { magic_ = magic; }

	const entity_addr_t& get_entity_addr() { return entity_.addr_; }

	const entity_name_t& get_entity_name() { return entity_.name_; }

	struct Policy
	{
		bool lossy;
		bool server;
		bool standby;
		bool resetcheck;
	};

	// �󶨵�ַ,��������ʵ��
	virtual int bind(const entity_addr_t& bind_addr) = 0;

	// ���°󶨶˿�,���Ҳ�����֮ǰ�󶨵Ķ˿ڳ�ͻ
	virtual int rebind(const std::set<int>& avoid_ports) { return -EOPNOTSUPP; }
	// ����
	virtual int start() { started_ = true; return 0; }
	// ֹͣ
	virtual int shutdown() { started_ = false; return 0; }
	// �����ȴ��ر�
	virtual void wait() = 0;
	// ������Ϣ
	virtual int send_message(Message* msg, const entity_inst_t& dest) = 0;
	// ������ӶϿ�
	virtual void mark_down(const entity_addr_t& addr) = 0;

protected:
	virtual void set_entity_addr(const entity_addr_t& a) { entity_.addr_ = a; }

public:
	int crc_flag_;

protected:
	// ��ǰͨ��ʵ��
	entity_inst_t entity_;

	// �Ƿ�������
	bool started_;

	uint32_t magic_;

private:
	// ��Ϣ�ַ�
	std::list<CDispatcher*> dispatchers_;
	// ������Ϣ�ַ�
	std::list<CDispatcher*> fast_dispatchers_;
};

#endif