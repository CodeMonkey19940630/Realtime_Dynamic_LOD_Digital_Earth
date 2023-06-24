#pragma once
#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <vector>
#include "../FzNode/FzNode.h"
#include "../EarthBasics/FzSituationSystem.h"

class FZNODE_EXPORT IRenderer
{
public:
	virtual ~IRenderer() {};

	/*!
	 * \fn	virtual const char* RenderObjectName() const = 0;
	 *
	 * \brief	获取绘制器名称
	 *
	 * \author	Admin
	 * \date	2022/7/26
	 *
	 * \return	Null if it fails, else a pointer to a const char.
	 */

	virtual const char* RenderObjectName() const = 0;

	/*!
	 * \fn	virtual bool Init(QOpenGLWidget* _widget, QOpenGLFunctions_3_3_Core* _function) = 0;
	 *
	 * \brief	初始化绘制器对象
	 *
	 * \author	Admin
	 * \date	2022/7/26
	 *
	 * \param [in,out]	_widget  	If non-null, the widget.
	 * \param [in,out]	_function	If non-null, the function.
	 *
	 * \return	True if it succeeds, false if it fails.
	 */

	virtual bool Init(QOpenGLWidget* _widget, QOpenGLFunctions_3_3_Core* _function) = 0;

	/*!
	 * \fn	virtual void Render(FzSituationSystem* _system, std::vector <FzNode*>)
	 *
	 * \brief	依据态势系统参数，绘制立体空间的节点
	 *
	 * \author	Admin
	 * \date	2022/7/26
	 *
	 * \param [in,out]	_system	If non-null, the system.
	 * \param [in,out]	>	   	If non-null, the >
	 */

	virtual void Render(FzSituationSystem* _system, std::vector <FzNode*>) = 0;

	/*!
	 * \fn	virtual void Paint()
	 *
	 * \brief	完成立体空间绘制后,绘制平面窗口内容
	 *
	 * \author	Admin
	 * \date	2022/7/26
	 */

	virtual void Paint(){ return; };

	/*!
	 * \fn	virtual void Uninit() = 0;
	 *
	 * \brief	反初始化
	 *
	 * \author	Admin
	 * \date	2022/7/26
	 */

	virtual void Uninit() = 0;

	virtual void AddNode(FzNode* _node);

	virtual void EraseNode(FzNode* _node);

	virtual void ClearAllNodes();

	virtual std::vector<FzNode*> GetAllNodes();

protected:
	std::vector<FzNode*> nodes_;
};

