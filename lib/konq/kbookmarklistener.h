/* This file is part of the KDE project
   Copyright (C) 2001 Alexander Kellett <lypanov@gmx.net>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; see the file COPYING.  If not, write to
   the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.
*/

#ifndef __kbookmarklistener_h__
#define __kbookmarklistener_h__

#include <dcopobject.h>
#include <qvaluelist.h>
#include <dcopref.h>

/**
 * DCOP interface for a bookmark listener
 */
class KBookmarkListener : virtual public DCOPObject
{
  K_DCOP

k_dcop:
  virtual void addBookmark( QString url, QString text, QString address, QString icon ) = 0;
  virtual void createNewFolder( QString text, QString address ) = 0;
};

#endif

