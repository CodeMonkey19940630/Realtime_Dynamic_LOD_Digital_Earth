#include "../../../include/glRender/FzNode/FzNode.h"
#include "../../../include/glRender/FzNode/FzRoot.h"

bool CmpNodeID(const FzNode* _a, const FzNode* _b)
{
	return _a->GetNodeID() < _b->GetNodeID();
}

bool CmpNodeIDandPtr(const FzNode* _a, const FzNode* _b)
{
	if (_a->GetNodeID() < _b->GetNodeID())
		return true;
	else if (_a->GetNodeID() != _b->GetNodeID())
		return false;
	else
	{
		return _a < _b;
	}
}

FzNode::FzNode(const std::string& _node_id /*= ""*/)
{
	node_id_ = _node_id;
	visible_ = true;
	projected_ = UnKnowPrject;
	root_ = nullptr;
	parent_ = nullptr;
	object_name_ = nullptr;
	is_sort_id_ = false;
	children_change_flag_.clear();
}

FzNode::~FzNode()
{
	if (parent_){
		SetParent(nullptr);
	}
	if (root_){
		root_->EraseRenderNode(this);
	}
	for (size_t i = 0; i < children_.size(); i++)
	{
		children_[i]->parent_ = nullptr;
		delete children_[i], children_[i] = nullptr;
	}
	children_.clear();
}

void FzNode::SetParent(FzNode* _parent)
{
	if (parent_){
		parent_->EraseChild(this);
	}
	parent_ = _parent;
	if (parent_){
		parent_->InsertChild(this);
	}
}

FzNode* FzNode::GetParent() const
{
	return parent_;
}

bool FzNode::ViewRangeTest(const FzCamera* _camera, double* _near, double* _far)
{
	return true;
}

void FzNode::SetProject(MapProjectedEnum _project)
{
	projected_ = _project;
}

MapProjectedEnum FzNode::GetProjected() const
{
	return projected_;
}

void FzNode::SetVisible(bool _visible)
{
	visible_ = _visible;
}

bool FzNode::GetVisible() const
{
	return visible_;
}

void FzNode::DestroyChildren()
{
	while (children_change_flag_.test_and_set());
	for (size_t i = 0; i < children_.size(); i++)
	{
		children_[i]->parent_ = nullptr;
		delete children_[i], children_[i] = nullptr;
	}
	children_.clear();
	std::vector <FzNode*> empty_;
	std::swap(empty_, children_);
	children_change_flag_.clear();
}

const std::vector <FzNode*>& FzNode::GetChildren() const
{
	return children_;
}

const char* FzNode::GetNodeObjectName() const
{
	if (0 == object_name_)
		return "";
	else
		return object_name_;
}

void FzNode::SetChildrenSort(bool _is_sort /*= false*/)
{
	while (children_change_flag_.test_and_set());
	if (is_sort_id_&&!_is_sort)
	{
		//children按指针顺序排列；
		std::sort(children_.begin(), children_.end());
	}
	else if (!is_sort_id_&&_is_sort)
	{
		//children按先ID后指针顺序排列；
		std::sort(children_.begin(), children_.end(), CmpNodeIDandPtr);
	}
	is_sort_id_ = _is_sort;
	children_change_flag_.clear();
}

FzNode* FzNode::FindChildrenFirstNodeByID(const std::string& _id)
{
	if (!is_sort_id_)
	{
		printf_s("子节点未排序,无法按ID查找子节点\r\n");
		return nullptr;
	}
	std::vector<FzNode*> out_vec;
	FzNode temp;
	temp.SetNodeID(_id);
	//找寻ID相等的迭代器起止位置
	std::vector<FzNode*>::iterator first_iter = std::upper_bound(children_.begin(), children_.end(), &temp, CmpNodeID);
	if ((*first_iter)->GetNodeID() == _id)
		return *first_iter;
	else
		return nullptr;
}

std::vector<FzNode*> FzNode::FindChildrenNodesByID(const std::string& _id)
{
	std::vector<FzNode*> out_vec;
	if (!is_sort_id_)
	{
		printf_s("子节点未排序,无法按ID查找子节点\r\n");
		return out_vec;
	}
	FzNode temp;
	temp.SetNodeID(_id);
	while (children_change_flag_.test_and_set());
	//找寻ID相等的迭代器起止位置
	std::pair<std::vector<FzNode*>::iterator, std::vector<FzNode*>::iterator> equal_iter = std::equal_range(children_.begin(), children_.end(), &temp, CmpNodeID);
	//迭代器插入到vector容器中
	out_vec.insert(out_vec.end(), equal_iter.first, equal_iter.second);
	children_change_flag_.clear();
	return out_vec;
}

void FzNode::SetNodeID(std::string _node_id)
{
	if (parent_&&parent_->is_sort_id_){
		//父节点的子节点容器需要重排
		while (parent_->children_change_flag_.test_and_set());
		ReplaceParentChildrenID(_node_id);
		parent_->children_change_flag_.clear();
	}
	else
	{
		node_id_ = _node_id;
	}
}

std::string FzNode::GetNodeID() const
{
	return node_id_;
}

void FzNode::InsertChild(FzNode* _child)
{
	while (children_change_flag_.test_and_set());
	std::vector<FzNode*>::iterator iter;
	if (is_sort_id_)
		iter = std::lower_bound(children_.begin(), children_.end(), _child, CmpNodeIDandPtr);
	else
		iter = std::lower_bound(children_.begin(), children_.end(), _child);
	children_.insert(iter, _child);
	children_change_flag_.clear();
	if (root_){
		root_->AddRenderNode(_child);
	}
}

void FzNode::EraseChild(FzNode* _child)
{
	while (children_change_flag_.test_and_set());
	std::vector<FzNode*>::iterator iter;
	if (is_sort_id_)
		iter = std::lower_bound(children_.begin(), children_.end(), _child, CmpNodeIDandPtr);
	else
		iter = std::lower_bound(children_.begin(), children_.end(), _child);
//	if (iter != children_.end() && *iter == _child)/*都是内部调用,删除的前提是存在父子关系,故不用做迭代器是否有效判断*/
	children_.erase(iter);
	children_change_flag_.clear();
	if (root_){
		root_->EraseRenderNode(_child);
	}
}

void FzNode::ReplaceParentChildrenID(const std::string& _node_id)
{
	//父节点的子节点容器需要重排
	auto child_iter = std::lower_bound(parent_->children_.begin(), parent_->children_.end(), this, CmpNodeIDandPtr);
	FzNode tgt(_node_id);
	auto tgt_range = std::equal_range(parent_->children_.begin(), parent_->children_.end(), &tgt, CmpNodeID);
	auto tgt_iter =std::lower_bound(tgt_range.first, tgt_range.second, this);
	node_id_ = _node_id;
	if (child_iter<tgt_iter)
	{
		auto tmp = child_iter + 1;
		while (tmp != tgt_iter)
		{
			std::swap(*child_iter, *tmp);
			child_iter = tmp;
			++tmp;
		}
	}
	else if (child_iter>tgt_iter)
	{
		auto tmp = child_iter - 1;
		while (tmp != tgt_iter)
		{
			std::swap(*child_iter, *tmp);
			child_iter = tmp;
			--tmp;
		}
	}
}
