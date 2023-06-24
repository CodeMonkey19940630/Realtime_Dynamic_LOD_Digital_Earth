#pragma once
#include <algorithm>
#include <vector>
#include <map>
#include <atomic>
#include "../EarthBasics/FzEvent.h"

# if defined(FZNODE_LIB)
#  define FZNODE_EXPORT __declspec(dllexport)
# else
#  define FZNODE_EXPORT __declspec(dllimport)
# endif

class FzRoot;
class FZNODE_EXPORT FzNode
{
public:
	FzNode(const std::string& _node_id = "");

	virtual ~FzNode();

	virtual void SetParent(FzNode* _parent);

	virtual FzNode* GetParent() const;

	virtual bool ViewRangeTest(const FzCamera* _camera, double* _near = nullptr, double* _far = nullptr);

	virtual void SetProject(MapProjectedEnum _project);

	virtual MapProjectedEnum GetProjected() const;

    virtual void SetVisible(bool _visible);

    virtual bool GetVisible() const;

	virtual void DestroyChildren();

    virtual const std::vector <FzNode*>& GetChildren() const;

	const char* GetNodeObjectName() const;

	void SetChildrenSort(bool _is_sort = false);

	FzNode* FindChildrenFirstNodeByID(const std::string& _id);

	std::vector<FzNode*> FindChildrenNodesByID(const std::string& _id);

	void SetNodeID(std::string _node_id);

	std::string GetNodeID() const;

	friend class FzRoot;
protected:
	virtual  void InsertChild(FzNode* _child);

	virtual  void EraseChild(FzNode* _child);

	void	ReplaceParentChildrenID(const std::string& _node_id);
protected:
	FzNode* parent_;
	bool visible_;
	MapProjectedEnum projected_;
	const char*	object_name_;
	std::string node_id_;
	std::vector <FzNode*> children_;
	bool is_sort_id_;
	FzRoot*	root_;
	std::atomic_flag children_change_flag_;
};
