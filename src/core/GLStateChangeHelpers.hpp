#pragma once
#include <glad/glad.h>

namespace dream
{
namespace glwrapper
{
template <GLenum attribute>
class GLPushAttribute
{
public:
	GLPushAttribute()
	{
		glPushAttrib(attribute);
	}
	~GLPushAttribute()
	{
		glPopAttrib();
	}
};

template <bool Trigger, GLenum State>
class GLEnable
{
public:
	GLEnable(const GLEnable &)					= delete;
	GLEnable(GLEnable &&)						= delete;
	const GLEnable &operator=(const GLEnable &) = delete;
	const GLEnable &operator=(GLEnable &&)		= delete;

	GLEnable()
	{
		mEnabled = glIsEnabled(State);
		if ((Trigger && mEnabled) || (!Trigger && !mEnabled))
			return;

		mSwitched = true;

		if (Trigger)
			glEnable(State);
		else
			glDisable(State);
	}
	~GLEnable()
	{
		if (!mSwitched)
			return;

		if (Trigger)
			glDisable(State);
		else
			glEnable(State);
	}

private:
	bool mSwitched = { false };
	bool mEnabled  = { false };
};
} // namespace glwrapper
} // namespace dream