#include "NNode.h"

const char* NodeBase::getInfo() const {
	if (type.empty()) {
		type = std::string(NodeTypeStr[getType()]) + "\n" + info;
	}
	return type.c_str();
}

bool NodeBase::setup(ImGui::NodeGraphEditor& nge, const ImVec2& pos, const char* inputSlots, const char* outputSlots, NodeType nodeTypeID) {
	init(NodeTypeStr[int(nodeTypeID)], pos, inputSlots, outputSlots, int(nodeTypeID));
	this->nge = &nge;
	this->info = NodeTooltipStr[int(nodeTypeID)];
	return this->onInit();
}

const char* NodeBase::getTooltip() const {
	return info.c_str();
}

bool NNode::acceptsLink(Node* inputNode) {
	return dynamic_cast<NNode*>(inputNode) != nullptr;
}

void NNode::onEdited() {
	markDirty();
}

void NNode::markDirty() {
	ImVector<ImGui::Node *> nodes;
	nge->getOutputNodesForNodeAndSlot(this, 0, nodes);
	for (ImGui::Node* node : nodes) {
		NNode* n = dynamic_cast<NNode*>(node);
		if (n == nullptr) {
			node->onEdited();
			continue;
		}
		n->markDirty();
	}
}

float ResultNode::getNoise(int x, int y) {
	float result = 0.0f;
	const int n = getNumInputSlots();
	for (int i = 0; i < n; ++i) {
		NNode* in = dynamic_cast<NNode*>(nge->getInputNodeForNodeAndSlot(this, i));
		if (in == nullptr) {
			continue;
		}
		switch (getType()) {
		case (int)NodeType::Add:
			result += in->getNoise(x, y);
			break;
		case (int)NodeType::Subtract:
			if (i == 0) {
				result = in->getNoise(x, y);
			} else {
				result -= in->getNoise(x, y);
			}
			break;
		case (int)NodeType::Multiply:
			if (i == 0) {
				result = in->getNoise(x, y);
			} else {
				result *= in->getNoise(x, y);
			}
			break;
		case (int)NodeType::Divide:
			if (i == 0) {
				result = in->getNoise(x, y);
			} else {
				result /= in->getNoise(x, y);
			}
			break;
		}
	}
	return result;
}
