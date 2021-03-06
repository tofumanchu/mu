/*
** Copyright (C) 2008-2010 Dirk-Jan C. Binnema <djcb@djcbsoftware.nl>
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 3 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software Foundation,
** Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
**
*/

#ifndef __MU_MSG_ITER_H__
#define __MU_MSG_ITER_H__

#include <glib.h>
#include <mu-msg.h>

G_BEGIN_DECLS


/**
 * MuMsgIter is a structure to iterate over the results of a
 * query. You can iterate only in one-direction, and you can do it
 * only once.
 *
 */

struct _MuMsgIter;
typedef struct _MuMsgIter MuMsgIter;


/**
 * create a new MuMsgIter -- basically, an iterator over the search
 * results
 *
 * @param enq a Xapian::Enquire* cast to XapianEnquire* (because this
 * is C, not C++),providing access to search results
 * @param batchsize how many results to retrieve at once
 * @param threads whether to calculate threads
 * @param sorting field when using threads; note, when 'threads' is
 * FALSE, this should be MU_MSG_FIELD_ID_NONE
 * @param if TRUE, revert the sorting order
 *
 * @return a new MuMsgIter, or NULL in case of error
 */
MuMsgIter *mu_msg_iter_new (XapianEnquire *enq,
			    size_t batchsize, gboolean threads,
			    MuMsgFieldId threadsortfield,
			    gboolean revert) G_GNUC_WARN_UNUSED_RESULT;

/**
 * get the next message (which you got from
 * e.g. mu_query_run)
 *
 * @param iter a valid MuMsgIter iterator
 *
 * @return TRUE if it succeeded, FALSE otherwise (e.g., because there
 * are no more messages in the query result)
 */
gboolean         mu_msg_iter_next              (MuMsgIter *iter);



/**
 * reset the iterator to the beginning
 *
 * @param iter a valid MuMsgIter iterator
 *
 * @return TRUE if it succeeded, FALSE otherwise
 */
gboolean mu_msg_iter_reset (MuMsgIter *iter);

/**
 * does this iterator point past the end of the list?
 *
 * @param iter a valid MuMsgIter iterator
 *
 * @return TRUE if the iter points past end of the list, FALSE
 * otherwise
 */
gboolean         mu_msg_iter_is_done (MuMsgIter *iter);


/**
 * destroy the sequence of messages; ie. /all/ of them
 *
 * @param msg a valid MuMsgIter message or NULL
 */
void		 mu_msg_iter_destroy           (MuMsgIter *iter);


/**
 * get the corresponding MuMsg for this iter; this instance is owned
 * by MuMsgIter, and becomes invalid after either mu_msg_iter_destroy
 * or mu_msg_iter_next. _do not_ unref it; it's a floating reference.
 *
 * @param iter a valid MuMsgIter instance*
 *
 * @return a MuMsg instance, or NULL in case of error
 */
MuMsg* mu_msg_iter_get_msg_floating (MuMsgIter *iter)
          G_GNUC_MALLOC G_GNUC_WARN_UNUSED_RESULT;



/**
 * get the document id for the current message
 *
 * @param iter a valid MuMsgIter iterator
 *
 * @return the docid or (unsigned int)-1 in case of error
 */
unsigned int     mu_msg_iter_get_docid         (MuMsgIter *iter);


/**
 * calculate the message threads
 *
 * @param iter a valid MuMsgIter iterator
 *
 * @return TRUE if it worked, FALSE otherwsie.
 */
gboolean mu_msg_iter_calculate_threads (MuMsgIter *iter);


enum _MuMsgIterThreadProp {
	MU_MSG_ITER_THREAD_PROP_ROOT           = 1 << 0,
	MU_MSG_ITER_THREAD_PROP_FIRST_CHILD    = 1 << 1,
	MU_MSG_ITER_THREAD_PROP_EMPTY_PARENT   = 1 << 2,
	MU_MSG_ITER_THREAD_PROP_DUP            = 1 << 3,
	MU_MSG_ITER_THREAD_PROP_HAS_CHILD      = 1 << 4
};
typedef guint8 MuMsgIterThreadProp;

struct _MuMsgIterThreadInfo {
	gchar *threadpath; /* a string decribing the thread-path in
			    * such a way that we can sort by this
			    * string to get the right order. */
	guint level;       /* thread-depth -- [0...] */
	MuMsgIterThreadProp prop;
};
typedef struct _MuMsgIterThreadInfo MuMsgIterThreadInfo;

/**
 * get a the MuMsgThreaderInfo struct for this message; this only
 * works when you created the mu-msg-iter with threading enabled
 *
 * @param iter a valid MuMsgIter iterator
 *
 * @return an info struct
 */
const MuMsgIterThreadInfo* mu_msg_iter_get_thread_info (MuMsgIter *iter);

/* FIXME */
const char* mu_msg_iter_get_path (MuMsgIter *iter);

G_END_DECLS

#endif /*__MU_MSG_ITER_H__*/
