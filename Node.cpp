#include "Node.h"

Node::Node(cv::Point position)
{
	m_position = position;
	
	


}

void Node::setPreNodeIndex(int pre)
{
	m_pre_node_index = pre;

}

void Node::setNextNodeIndex(int next)
{
	m_next_node_index.push_back(next);

}


std::vector<int> Node::getNextNodeIndex()
{
	return m_next_node_index;
}


int Node::getPretNodeIndex()
{
	return m_pre_node_index;
}


cv::Point Node::getPostion()
{
	return m_position;
}



