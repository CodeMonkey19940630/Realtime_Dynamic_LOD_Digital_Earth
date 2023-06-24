#pragma once
#include "FzSituationSystem.h"
class QOpenGLShaderProgram;
struct FRenderEvent
{
	QOpenGLShaderProgram*	program_;
	FzSituationSystem*		situ_system_;
};