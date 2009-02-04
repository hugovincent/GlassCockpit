// FIXME copyright header

/**
 * Defines messages which can be triggered by mouse clicks, key presses, etc.
 */

#ifndef Messages_h
#define Messages_h

namespace OpenGC
{

enum Message
{
	/** Nav zoom in and out */
	MSG_NAV_ZOOM_DECREASE,
	MSG_NAV_ZOOM_INCREASE,

	/** Application control messages */
	MSG_APP_QUIT
};

} // end namespace OpenGC

#endif
