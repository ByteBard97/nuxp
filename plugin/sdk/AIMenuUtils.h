#ifndef __AIMenuUtils__
#define __AIMenuUtils__
/*************************************************************************
 * ADOBE CONFIDENTIAL
 * ___________________
 *
 *  Copyright 2025 Adobe
 *  All Rights Reserved.
 *
 * NOTICE:  All information contained herein is, and remains
 * the property of Adobe and its suppliers, if any. The intellectual
 * and technical concepts contained herein are proprietary to Adobe
 * and its suppliers and are protected by all applicable intellectual
 * property laws, including trade secret and copyright laws.
 * Dissemination of this information or reproduction of this material
 * is strictly forbidden unless prior written permission is obtained
 * from Adobe.
 **************************************************************************/

#ifndef __AITypes__
#include "AITypes.h"
#endif

#ifndef __AIPlugin__
#include "AIPlugin.h"
#endif

#ifndef __ASHelp__
#include "ASHelp.h"
#endif

#ifndef _IAIUNICODESTRING_H_
#include "IAIUnicodeString.h"
#endif

#include "AIHeaderBegin.h"

/** Option flags that control the behavior of a menu item.
    See \c #kMenuItemNoOptions and following. */
typedef ai::int32 AIMenuItemOption;
/** Bit flags for \c #AIMenuItemOption */
enum  {
    /** Turn off all options */
    kMenuItemNoOptions                    = 0,
    /** Set to receive the \c #kSelectorAIUpdateMenuItem selector. */
    kMenuItemWantsUpdateOption            = (1<<0),
    /** Set to disallow disabling (graying) of item. */
    kMenuItemAlwaysEnabled                = (1<<1),
    /** Set to create a separator. Pass \c nullptr for
        \c #AIPlatformAddMenuItemDataUS::itemText. */
    kMenuItemIsSeparator                = (1<<2),
    /** Not for item creation. Identifies a group header when iterating
    through items. See \c #AIMenuSuite::SetMenuGroupHeader(). */
    kMenuItemIsGroupHeader                = (1<<3),
    /** Set to ignore the parent group's
        \c #kMenuGroupSortedAlphabeticallyOption state. */
    kMenuItemIgnoreSort                    = (1<<4)
};

/*******************************************************************************
 **
 **	Types
 **
 **/

/** Opaque reference to a menu item. Never dereferenced.
Access with \c #AIMenuSuite functions.  */
typedef struct _t_AIMenuItemOpaque	*AIMenuItemHandle;
/** Opaque reference to a menu group. Never dereferenced.
Access with \c #AIMenuSuite functions. */
typedef struct _t_MenuGroupOpaque	*AIMenuGroup;   // new for AI7.0

#if Macintosh
	/** Platform-specific menu reference. \li In Mac OS, a \c MenuRef.
		\li In Windows, cast to \c HMENU.*/
	typedef struct MacMenu_t* AIPlatformMenuHandle;
#elif MSWindows
	/** Platform-specific menu reference. \li In Mac OS, a \c MenuRef.
		\li In Windows, cast to \c HMENU. */
	typedef struct WinMenu	**AIPlatformMenuHandle;	 // can cast it to HMENU
#elif LINUX_ENV
	typedef void	**AIPlatformMenuHandle;
#endif

/** Menu item definition data. */
typedef struct {
	/** The menu group to which an item is added. See @ref menuGroups.*/
	const char *groupName;
	/** The display label for an item. (To create a menu separator
		set the \c #kMenuItemIsSeparator options flag and set this
		to \c nullptr.) */
	ai::UnicodeString itemText;
} AIPlatformAddMenuItemDataUS;

/** A platform-specific menu structure corresponding to an
	Illustrator menu item.
	See \c #AIMenuSuite::GetPlatformMenuItem(). */
typedef struct {
	/** An \c HMENU in Windows, a \c MenuRef in Mac OS. */
	AIPlatformMenuHandle menu;
	/** The index of the item in the platform menu. */
	ai::int16 item;
} AIPlatformMenuItem;

/** Message sent with menu selectors. */
typedef struct {
	/** The message data. */
	SPMessageData d;
	/** The  menu item object. */
	AIMenuItemHandle menuItem;
} AIMenuMessage;

#include "AIHeaderEnd.h"

#endif
