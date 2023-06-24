#include "labelrenderer_global.h"
#include "../../../include/glRender/FzNode/FzLabelNode.h"


FzLabelNode::FzLabelNode(const std::string& _node_id /*= ""*/)
	:FzMovingNode(_node_id)
{
	object_name_ = LabelNodeObject;

	label_text_ = "";

	font_size_ = 12;

	font_family_ = "ºÚÌå";
	
	is_show_node_id_ = false;
}

FzLabelNode::~FzLabelNode()
{
}

void FzLabelNode::ResetAnchorPoint(gVector2f _sreen_pos)
{
	anchor_point_ = _sreen_pos;
}

gVector2f FzLabelNode::GetAnchorPoint()
{
	return anchor_point_;
}

void FzLabelNode::SetLabelRelativeAnchor(gVector2f _sreen_pos)
{
	relative_point_ = _sreen_pos;
}

gVector2f FzLabelNode::GetLabelRelativeAnchor()
{
	return relative_point_;
}

void FzLabelNode::SetText(const char* _text)
{
	label_text_ = _text;
}

const char* FzLabelNode::GetText() const
{
	return label_text_.c_str();
}

void FzLabelNode::ShowNodeID()
{
	is_show_node_id_ = true;
}

void FzLabelNode::HideNodeID()
{
	is_show_node_id_ = false;
}

bool FzLabelNode::IsShowNodeID() const
{
	return is_show_node_id_;
}

void FzLabelNode::SetFrameColor(gColor32bit _rgba)
{
	color_[0] = _rgba;
}

gColor32bit FzLabelNode::GetFrameColor()
{
	return color_[0];
}

void FzLabelNode::SetFillColor(gColor32bit _rgba)
{
	color_[1] = _rgba;
}

gColor32bit FzLabelNode::GetFillColor()
{
	return color_[1];
}

void FzLabelNode::SetLinkLineColor(gColor32bit _rgba)
{
	color_[2] = _rgba;
}

gColor32bit FzLabelNode::GetLinkLineColor()
{
	return color_[2];
}

void FzLabelNode::SetTextColor(gColor32bit _rgba)
{
	color_[3] = _rgba;
}

gColor32bit FzLabelNode::GetTextColor()
{
	return color_[3];
}

void FzLabelNode::SetTextFontSize(int _font_size)
{
	font_size_ = _font_size;
}

void FzLabelNode::SetTextFontFamily(const char* _family_name)
{
	font_family_ = _family_name;
}
