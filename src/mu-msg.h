/* 
** Copyright (C) 2010 Dirk-Jan C. Binnema <djcb@djcbsoftware.nl>
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

#ifndef __MU_MSG_H__
#define __MU_MSG_H__

#include <mu-msg-flags.h>
#include <mu-msg-fields.h>
#include <mu-msg-prio.h>
#include <mu-util.h> /* for MuResult, MuError */

G_BEGIN_DECLS

struct _MuMsg;
typedef struct _MuMsg MuMsg;

/**
 * create a new MuMsg* instance which parses a message and provides
 * read access to its properties; call mu_msg_destroy when done with it.
 *
 * @param path full path to an email message file
 * @param mdir the maildir for this message; ie, if the path is
 * ~/Maildir/foo/bar/cur/msg, the maildir would be foo/bar; you can
 * pass NULL for this parameter, in which case some maildir-specific
 * information is not available.
 * @param err receive error information (MU_ERROR_FILE or MU_ERROR_GMIME), or NULL. There
 * will only be err info if the function returns NULL
 * 
 * @return a new MuMsg instance or NULL in case of error; call
 * mu_msg_unref when done with this message
 */
MuMsg *mu_msg_new_from_file (const char* filepath, const char *maildir,
			     GError **err) G_GNUC_MALLOC G_GNUC_WARN_UNUSED_RESULT;


/**
 * increase the reference count for this message
 * 
 * @param msg a message
 *
 * @return the message with its reference count increased, or NULL in
 * case of error.
 */
MuMsg * mu_msg_ref (MuMsg *msg);

/**
 * decrease the reference count for this message. if the reference
 * count reaches 0, the message will be destroyed.
 * 
 * @param msg a message
 */
void mu_msg_unref (MuMsg *msg);



/**
 * cache the values from the backend (file or db), so we don't the
 * backend anymore
 * 
 * @param self a message
 */
void mu_msg_cache_values (MuMsg *self);


/**
 * get the plain text body of this message
 *
 * @param msg a valid MuMsg* instance
 * 
 * @return the plain text body or NULL in case of error or if there is no
 * such body. the returned string should *not* be modified or freed.
 * The returned data is in UTF8 or NULL.
 */
const char*     mu_msg_get_body_text       (MuMsg *msg);


/**
 * get the html body of this message
 *
 * @param msg a valid MuMsg* instance
 * 
 * @return the html body or NULL in case of error or if there is no
 * such body. the returned string should *not* be modified or freed.
 */
const char*     mu_msg_get_body_html       (MuMsg *msg);

/**
 * get the sender (From:) of this message
 *
 * @param msg a valid MuMsg* instance
 * 
 * @return the sender of this Message or NULL in case of error or if there 
 * is no sender. the returned string should *not* be modified or freed.
 */
const char*     mu_msg_get_from	   (MuMsg *msg);


/**
 * get the recipients (To:) of this message
 *
 * @param msg a valid MuMsg* instance
 * 
 * @return the sender of this Message or NULL in case of error or if there 
 * are no recipients. the returned string should *not* be modified or freed.
 */
const char*     mu_msg_get_to	   (MuMsg *msg);


/**
 * get the carbon-copy recipients (Cc:) of this message
 *
 * @param msg a valid MuMsg* instance
 * 
 * @return the Cc: recipients of this Message or NULL in case of error or if 
 * there are no such recipients. the returned string should *not* be modified 
 * or freed.
 */
const char*     mu_msg_get_cc	     (MuMsg *msg);


/**
 * get the blind carbon-copy recipients (Bcc:) of this message; this
 * field usually only appears in outgoing messages
 *
 * @param msg a valid MuMsg* instance
 * 
 * @return the Bcc: recipients of this Message or NULL in case of
 * error or if there are no such recipients. the returned string
 * should *not* be modified or freed.
 */
const char*     mu_msg_get_bcc	     (MuMsg *msg);

/**
 * get the file system path of this message
 *
 * @param msg a valid MuMsg* instance
 * 
 * @return the path of this Message or NULL in case of error. 
 * the returned string should *not* be modified or freed.
 */
const char*     mu_msg_get_path            (MuMsg *msg);


/**
 * get the maildir this message lives in; ie, if the path is
 * ~/Maildir/foo/bar/cur/msg, the maildir would be foo/bar
 *
 * @param msg a valid MuMsg* instance
 * 
 * @return the maildir requested or NULL in case of error. The returned
 * string should *not* be modified or freed.
 */
const char*    mu_msg_get_maildir        (MuMsg *msg);


/**
 * get the subject of this message
 *
 * @param msg a valid MuMsg* instance
 * 
 * @return the subject of this Message or NULL in case of error or if there 
 * is no subject. the returned string should *not* be modified or freed.
 */
const char*     mu_msg_get_subject         (MuMsg *msg);

/**
 * get the Message-Id of this message
 *
 * @param msg a valid MuMsg* instance
 * 
 * @return the Message-Id of this Message (without the enclosing <>)
 * or NULL in case of error or if there is none. the returned string
 * should *not* be modified or freed.
 */
const char*     mu_msg_get_msgid           (MuMsg *msg);


/**
 * get any arbitrary header from this message
 *
 * @param msg a valid MuMsg* instance
 * @header the header requested
 * 
 * @return the header requested or NULL in case of error or if there 
 * is no such header. the returned string should *not* be modified or freed.
 */
const char*     mu_msg_get_header          (MuMsg *msg, 
					    const char* header);

/**
 * get the message date/time (the Date: field) as time_t, using UTC
 *
 * @param msg a valid MuMsg* instance
 * 
 * @return message date/time or 0 in case of error or if there 
 * is no such header.
 */
time_t          mu_msg_get_date            (MuMsg *msg);

/**
 * get the flags for this message
 *
 * @param msg valid MuMsg* instance
 * 
 * @return the fileflags as logically OR'd #Mu MsgFlags or 0 if
 * there are none.
 */
MuMsgFlags     mu_msg_get_flags      (MuMsg *msg);


/**
 * get the file size in bytes of this message
 *
 * @param msg a valid MuMsg* instance
 * 
 * @return the filesize 
 */
size_t          mu_msg_get_size       (MuMsg *msg);


/**
 * get some field value as string
 * 
 * @param msg a valid MuMsg instance
 * @param field the field to retrieve; it must be a string-typed field
 * 
 * @return a string that should not be freed
 */
const char*  mu_msg_get_field_string  (MuMsg *msg, MuMsgFieldId mfid);


/**
 * get some field value as string
 * 
 * @param msg a valid MuMsg instance
 * @param field the field to retrieve; it must be a numeric field
 * 
 * @return a string that should not be freed
 */
gint64      mu_msg_get_field_numeric (MuMsg *msg, MuMsgFieldId mfid);

/**
 * get the message priority for this message (MU_MSG_PRIO_LOW,
 * MU_MSG_PRIO_NORMAL or MU_MSG_PRIO_HIGH) the X-Priority,
 * X-MSMailPriority, Importance and Precedence header are checked, in
 * that order.  if no known or explicit priority is set,
 * MU_MSG_PRIO_NORMAL is assumed
 *
 * @param msg a valid MuMsg* instance
 * 
 * @return the message priority (!= 0) or 0 in case of error
 */
MuMsgPrio   mu_msg_get_prio        (MuMsg *msg);

/**
 * get the timestamp (mtime) for the file containing this message 
 *
 * @param msg a valid MuMsg* instance
 * 
 * @return the timestamp or 0 in case of error
 */
time_t          mu_msg_get_timestamp       (MuMsg *msg);


/**
 * get a list of message ids this message refers to -- this is based
 * on the References: and In-reply-to: headers.
 * 
 * @param msg a valid MuMsg instance
 * 
 * @return a list of message id, with the most immediate parent as the
 * last element. Don't modify/free this list.
 */
const GSList *mu_msg_get_references (MuMsg *msg);


/**
 * get the list of references as a comma-separated string
 * 
 * @param msg a valid MuMsg
 * 
 * @return a comma-separated string with the references or NULL if
 * there are none. Don't modify/free
 */
const char* mu_msg_get_references_str (MuMsg *msg);


enum _MuMsgContactType {  /* Reply-To:? */
	MU_MSG_CONTACT_TYPE_TO    = 0,
	MU_MSG_CONTACT_TYPE_FROM,  
	MU_MSG_CONTACT_TYPE_CC,   
	MU_MSG_CONTACT_TYPE_BCC,
	
	MU_MSG_CONTACT_TYPE_NUM
};
typedef guint MuMsgContactType;

#define mu_msg_contact_type_is_valid(MCT)\
	((MCT) < MU_MSG_CONTACT_TYPE_NUM)

struct _MuMsgContact {
	const char		*name;	    /* Foo Bar */
	const char		*address;   /* foo@bar.cuux */
	MuMsgContactType	 type;	    /* MU_MSG_CONTACT_TYPE_{ TO,
					     * CC, BCC, FROM} */  
};
typedef struct _MuMsgContact	 MuMsgContact;

/**
 * create a new MuMsgContact object; note, in many case, this is not
 * needed, any a stack-allocated struct can be uses.
 * 
 * @param name the name of the contact
 * @param address the e-mail address of the contact
 * @param type the type of contact: cc, bcc, from, to
 * 
 * @return a newly allocated MuMsgConcact or NULL in case of
 * error. use mu_msg_contact_destroy to destroy it when it's no longer
 * needed.
 */
MuMsgContact *mu_msg_contact_new (const char *name, const char *address,
				  MuMsgContactType type)
				  G_GNUC_MALLOC G_GNUC_WARN_UNUSED_RESULT;

/**
 * destroy a MuMsgConcact object
 * 
 * @param contact a contact object, or NULL
 */
void          mu_msg_contact_destroy (MuMsgContact *contact);

/**
 * macro to get the name of a contact
 * 
 * @param ct a MuMsgContact
 * 
 * @return the name
 */
#define mu_msg_contact_name(ct)    ((ct)->name)

/**
 * macro to get the address of a contact
 * 
 * @param ct a MuMsgContact
 * 
 * @return the address
 */
#define mu_msg_contact_address(ct) ((ct)->address)

/**
 * macro to get the contact type
 * 
 * @param ct a MuMsgContact
 * 
 * @return the contact type
 */
#define mu_msg_contact_type(ct)    ((ct)->type)


/**
 * callback function
 *
 * @param contact
 * @param user_data a user provided data pointer
 *
 * @return TRUE if we should continue the foreach, FALSE otherwise
 */
typedef gboolean  (*MuMsgContactForeachFunc) (MuMsgContact* contact,
					      gpointer user_data);

/**
 * call a function for each of the contacts in a message 
 *
 * @param msg a valid MuMsgGMime* instance
 * @param func a callback function to call for each contact; when
 * the callback does not return TRUE, it won't be called again
 * @param user_data a user-provide pointer that will be passed to the callback
 * 
 */
void mu_msg_contact_foreach (MuMsg *msg, MuMsgContactForeachFunc func,
			     gpointer user_data);

G_END_DECLS

#endif /*__MU_MSG_H__*/
