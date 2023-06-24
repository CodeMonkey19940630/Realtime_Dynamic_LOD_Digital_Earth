#pragma once
#include "FzMovingNode.h"

#define  ModelNodeObject "object.model"
#ifdef MODELRENDERER_LIB
# define MODELRENDERER_EXPORT __declspec(dllexport)
#else
# define MODELRENDERER_EXPORT __declspec(dllexport)
#endif

struct ModelingExplain{
	ModelingExplain(const std::string& _keyname, const std::string& _file_path)
	{
		modeling_key_name_ = _keyname;
		modeling_file_path_ = _file_path;
		adjust_scale_ = { 1, 1, 1 };
	}
	std::string modeling_key_name_;
	gVector3d	adjust_translation_;
	rAttitude	adjust_rotate_;
	gVector3d	adjust_scale_;
	std::string modeling_file_path_;
};

class ModelRenderer;
class MODELRENDERER_EXPORT FzModelNode :public FzMovingNode
{
public:
	FzModelNode(const std::string& _node_id = "");
	~FzModelNode();
	
	virtual bool SetModelingName(const std::string& _modeling_key_name);

	virtual const std::string GetModelingName() const;

	virtual void SetScale(double _model_scale);

	virtual double GetScale() const;

	virtual void SetColor(){ return;}

public:
	static bool LoadModeling(const ModelingExplain& _explain);

	static bool UnLoadModeling(const std::string& _modeling_key_name);

	static bool UnLoadAllModelings();
protected:
	friend class ModelRenderer;
private:
	double model_scale_;
	std::string modeling_key_name_;
};
