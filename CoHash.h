#include <stdint.h>
#include <stdio.h>
#include <vector>
#include <vector>
#include <string>



class CoHash
{
private:
	const static int defaultReplicaValue_ = 20;
	const static int hashSize_ = 10000000;

public:
	CoHash(int replica = defaultReplicaValue_):replicaNum_(replica){}
	int getMachine(const std::string &key);
	void addMachine(int machineId);
	void removeMachine(int machineId);
	//
	std::string getNodesAsString() const;
private:
	uint64_t hashMurmur(const char *key);
	//
	int upperBound(int hash);
	int insertPos(int hash);
	int leftEqual(int index, int hash);
	int rightEqual(int index, int hash);
	struct NodeType
	{
		int machineId_;
		int replicaId_;
		int hash_;
		bool operator<(const NodeType &node) const;
	};
	typedef std::vector<NodeType> NodeTable;
	
	NodeTable nodes_;
	int replicaNum_;
};

