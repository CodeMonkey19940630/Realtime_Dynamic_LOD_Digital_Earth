#pragma once
#include "FzMovingNode.h"

#define  LabelNodeObject "object.label"
#ifdef LABELRENDERER_LIB
# define LABELRENDERER_EXPORT __declspec(dllexport)
#else
# define LABELRENDERER_EXPORT __declspec(dllexport)
#endif

class LabelRenderer;
class LABELRENDERER_EXPORT FzLabelNode :public FzMovingNode
{
public:
	FzLabelNode(const std::string& _node_id = "");
	~FzLabelNode();

	virtual void ResetAnchorPoint(gVector2f _sreen_pos);

	gVector2f GetAnchorPoint();

	//���Ͻ������ê���ƽ��λ��
	void SetLabelRelativeAnchor(gVector2f _sreen_pos);

	gVector2f GetLabelRelativeAnchor();

	void SetText(const char* _text);

	const char* GetText() const;

	void ShowNodeID();

	void HideNodeID();

	bool IsShowNodeID() const;

	//�߿���ɫ
	void SetFrameColor(gColor32bit _rgba);

	gColor32bit GetFrameColor();

	//�����ɫ
	void SetFillColor(gColor32bit _rgba);

	gColor32bit GetFillColor();

	//��������ɫ
	void SetLinkLineColor(gColor32bit _rgba);

	gColor32bit GetLinkLineColor();

	//������ɫ
	void SetTextColor(gColor32bit _rgba);

	gColor32bit GetTextColor();

	void SetTextFontSize(int _font_size);

	void SetTextFontFamily(const char* _family_name);
protected:
	friend class LabelRenderer;
protected:
	bool is_show_node_id_;

	std::string label_text_;

	int font_size_;

	std::string font_family_;

	//�߿�;���;������;����
	gColor32bit color_[4];

	gVector2f anchor_point_;

	gVector2f relative_point_;

};

