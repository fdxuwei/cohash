#include <algorithm>
#include <assert.h>
#include <string.h>
#include <sstream>
#include <iomanip>
#include "CoHash.h"
//
int CoHash::getMachine(const std::string &key)
{
	int hash = static_cast<int>(hashMurmur(key.c_str())%hashSize_);
	int nodeIndex = leftEqual(upperBound(hash), hash);
	assert(nodeIndex > 0 && nodeIndex < nodes_.size());
	//
	return nodes_[nodeIndex].machineId_;
}

void CoHash::removeMachine(int machineId)
{
	for(NodeTable::iterator it = nodes_.begin();  it != nodes_.end();)
	{
		if(it->machineId_ == machineId)
		{
			it = nodes_.erase(it);
		}
		else
		{
			++it;
		}
	}

}

//
void CoHash::addMachine(int machineId)
{
	for(int i = 0; i < replicaNum_; ++i)
	{
		char str[32];
		snprintf(str, sizeof(str), "machine_%d_replica_%d", machineId, i);
		int hash = static_cast<int>(hashMurmur(str)%hashSize_);
		NodeType node;
		node.machineId_ = machineId;
		node.replicaId_ = i;
		node.hash_ = hash;
		//
		if(nodes_.empty())
		{
			nodes_.push_back(node);
			continue;
		}
		//
		int insertAt = insertPos(hash);
		if(nodes_[insertAt].hash_ != hash)
		{
			nodes_.insert(nodes_.begin()+insertAt, node);
		}
		else
		{
			insertAt = rightEqual(insertAt, hash);
			while(insertAt >= 0 &&
				nodes_[insertAt].hash_ == hash &&
				nodes_[insertAt].machineId_ > machineId)
			{
				--insertAt;
			}
			if(insertAt < 0)
			{
				insertAt = 0;
			}
			nodes_.insert(nodes_.begin()+insertAt, node);
		}
	}
}

int CoHash::leftEqual(int index, int hash)
{
	if(index == 0 || hash != nodes_[index].hash_)
	{
		return index;
	}
	int leftmost = index-1;
	while(leftmost >= 0 && hash == nodes_[leftmost].hash_)
	{
		--leftmost;
	}
	++leftmost;
	return leftmost;

}

int CoHash::rightEqual(int index, int hash)
{
	int maxIndex = nodes_.size()-1;
	if(index == maxIndex || hash != nodes_[maxIndex].hash_)
	{
		return index;
	}
	int rightmost = index+1;
	while(rightmost <= maxIndex && hash == nodes_[rightmost].hash_)
	{
		++rightmost;
	}
	--rightmost;
	return rightmost;

}

int CoHash::insertPos(int hash)
{
	if(nodes_.empty())
	{
		return 0;
	}
	//
	int low = 0;
	int high = nodes_.size()-1;
	int mid;
	while(low <= high)
	{
		mid = (low+high)/2;
		if(hash == nodes_[mid].hash_)
		{
			return mid;
		}
		else if(hash > nodes_[mid].hash_)
		{
			low = mid+1;
		}
		else
		{
			high = mid-1;
		}
	}
	// not found
	if(hash > nodes_[mid].hash_)
	{
		return (mid+1);
	}
	else
	{
		return mid;
	}

}

int CoHash::upperBound(int hash)
{
	if(nodes_.empty())
	{
		return 0;
	}
	//
	int low = 0;
	int high = nodes_.size()-1;
	int mid;
	while(low <= high)
	{
		mid = (low+high)/2;
		if(hash == nodes_[mid].hash_)
		{
			return mid;
		}
		else if(hash > nodes_[mid].hash_)
		{
			low = mid+1;
		}
		else
		{
			high = mid-1;
		}
	}
	// not found
	if(hash > nodes_[mid].hash_)
	{
		return (mid+1)%nodes_.size();
	}
	else
	{
		return mid;
	}

}

std::string CoHash::getNodesAsString() const
{
	std::stringstream ss;
	for(int i = 0; i < nodes_.size(); ++i)
	{
		ss << std::setw(8)<< nodes_[i].machineId_ << "," << nodes_[i].replicaId_ << "," << nodes_[i].hash_ << "\n"; 
	}
	return ss.str();
}

//
uint64_t CoHash::hashMurmur(const char *key)
{
	uint64_t seed = 0x1234ABCD;
	uint64_t m = 0xc6a4a7935bd1e995;
	uint64_t r = 47;
	int len = strlen(key);
	uint64_t h = seed ^ (len * m);
	uint64_t k;

	const char *tmp = NULL;
	for (tmp = key; len >= 8; ) {
		k = *(uint64_t *)(&tmp[0]);
		tmp += 8;
		k *= m;
		k ^= k >> r;
		k *= m;
		h ^= k;
		h *= m;
		len -= 8;
	}

	uint64_t value = 0;
	char remaining[8] = {0,};
	if (len > 0) {
		memcpy(remaining, tmp, strlen(tmp));
		value = *(uint64_t *)(&remaining[0]);
		h ^= value;
		h *= m;
	}
	h ^= h >> r;
	h *= m;
	h ^= h >> r;

	return h;
}

