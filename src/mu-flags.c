/*
** Copyright (C) 2011  <djcb@djcbsoftware.nl>
**
** This program is free software; you can redistribute it and/or modify it
** under the terms of the GNU General Public License as published by the
** Free Software Foundation; either version 3, or (at your option) any
** later version.
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
#include "mu-flags.h"

struct _FlagInfo {
	MuFlags		 flag;
	char		 kar;
	const char	*name;
	MuFlagType	 flag_type;
};
typedef struct _FlagInfo FlagInfo;

static const FlagInfo FLAG_INFO[] = {

	/* NOTE: order of this is significant, due to optimizations
	 * below */
	
	{ MU_FLAG_DRAFT,      'D', "draft",     MU_FLAG_TYPE_MAILFILE },
	{ MU_FLAG_FLAGGED,    'F', "flagged",   MU_FLAG_TYPE_MAILFILE },
	{ MU_FLAG_PASSED,     'P', "passed",    MU_FLAG_TYPE_MAILFILE },
	{ MU_FLAG_REPLIED,    'R', "replied",   MU_FLAG_TYPE_MAILFILE },
	{ MU_FLAG_SEEN,       'S', "seen",      MU_FLAG_TYPE_MAILFILE },
	{ MU_FLAG_TRASHED,    'T', "trashed",   MU_FLAG_TYPE_MAILFILE },
			       		     
	{ MU_FLAG_NEW,        'N', "new",       MU_FLAG_TYPE_MAILDIR  },	

	{ MU_FLAG_SIGNED,     's', "signed",    MU_FLAG_TYPE_CONTENT  },
	{ MU_FLAG_ENCRYPTED,  'x', "encrypted", MU_FLAG_TYPE_CONTENT  },
	{ MU_FLAG_HAS_ATTACH, 'a', "attach",    MU_FLAG_TYPE_CONTENT  },

	{ MU_FLAG_UNREAD,     'u', "unread",    MU_FLAG_TYPE_PSEUDO  }
};

/* does not use FLAG_INFO, optimized */
MuFlagType
mu_flag_type (MuFlags flag)
{
	if (flag >= MU_FLAG_DRAFT && flag <= MU_FLAG_TRASHED)
		return MU_FLAG_TYPE_MAILFILE;
	if (flag == MU_FLAG_NEW)
		return MU_FLAG_TYPE_MAILDIR;
	if (flag == MU_FLAG_UNREAD)
		return MU_FLAG_TYPE_PSEUDO;
	if (flag >= MU_FLAG_SIGNED && flag <= MU_FLAG_HAS_ATTACH)
		return MU_FLAG_TYPE_CONTENT;

	return MU_FLAG_TYPE_INVALID;
}


/* does not use FLAG_INFO, optimized */
char
mu_flag_char (MuFlags flag)
{
	switch (flag) {
		
	case MU_FLAG_DRAFT:		return  'D';
	case MU_FLAG_FLAGGED:		return	'F';
	case MU_FLAG_PASSED:		return	'P';
	case MU_FLAG_REPLIED:		return	'R';
	case MU_FLAG_SEEN:		return	'S';
	case MU_FLAG_TRASHED:		return	'T';
		
	case MU_FLAG_NEW:		return	'N';
		
	case MU_FLAG_SIGNED:		return	's';
	case MU_FLAG_ENCRYPTED:		return	'x';
	case MU_FLAG_HAS_ATTACH:	return	'a';

	case MU_FLAG_UNREAD:		return	'u';

	default:
		return 0;
	}
}



static MuFlags
mu_flag_from_char (char kar)
{
	switch (kar) {
		
	case 'D': return MU_FLAG_DRAFT;		
	case 'F': return MU_FLAG_FLAGGED;	
	case 'P': return MU_FLAG_PASSED;	
	case 'R': return MU_FLAG_REPLIED;	
	case 'S': return MU_FLAG_SEEN;		
	case 'T': return MU_FLAG_TRASHED;	

	case 'N': return MU_FLAG_NEW;		
		
	case 's': return MU_FLAG_SIGNED;	
	case 'x': return MU_FLAG_ENCRYPTED;	
	case 'a': return MU_FLAG_HAS_ATTACH;	

	case 'u': return MU_FLAG_UNREAD;	
		
	default:
		return MU_FLAG_INVALID;
	}
}


/* does not use FLAG_INFO, optimized */
const char*
mu_flag_name (MuFlags flag)
{
	switch (flag) {
	case  MU_FLAG_DRAFT:		return  "draft";    
	case  MU_FLAG_FLAGGED:		return	"flagged";  
	case  MU_FLAG_PASSED:		return	"passed";   
	case  MU_FLAG_REPLIED:		return	"replied";  
	case  MU_FLAG_SEEN:		return	"seen";     
	case  MU_FLAG_TRASHED:		return	"trashed";  
					     
	case  MU_FLAG_NEW:		return	"new";      
	
	case  MU_FLAG_SIGNED:		return	"signed";   
	case  MU_FLAG_ENCRYPTED:	return  "encrypted";
	case  MU_FLAG_HAS_ATTACH:	return	"attach";

	case MU_FLAG_UNREAD:		return  "unread";	
		
	default:
		return NULL;
	}
}


const char*
mu_flags_to_str_s (MuFlags flags, MuFlagType types)
{
	unsigned u,v;
	static char str[sizeof(FLAG_INFO) + 1];
	
	for (u = 0, v = 0; u != G_N_ELEMENTS(FLAG_INFO); ++u) 
		if (flags & FLAG_INFO[u].flag &&
		    types & FLAG_INFO[u].flag_type)
			str[v++] = FLAG_INFO[u].kar;
	str[v] = '\0';
	
	return str;
}



MuFlags
mu_flags_from_str (const char *str, MuFlagType types)
{
	const char *cur;
	MuFlags flag;

	g_return_val_if_fail (str, MU_FLAG_INVALID);

	for (cur = str, flag = MU_FLAG_NONE; *cur; ++cur) {

		MuFlags f;

		f = mu_flag_from_char (*cur);
		if (mu_flag_type (f) & types)
			flag |= f;
	}
		
	return flag;
}



void
mu_flags_foreach (MuFlagsForeachFunc func, gpointer user_data)
{
	unsigned u;
	
	g_return_if_fail (func);
	
	for (u = 0; u != G_N_ELEMENTS(FLAG_INFO); ++u)
		func (FLAG_INFO[u].flag, user_data);
}


MuFlags
mu_flags_from_str_delta (const char *str, MuFlags oldflags,
			 MuFlagType types)
{
	const char *cur;
	MuFlags newflags;
		
	g_return_val_if_fail (str, MU_FLAG_INVALID);

	for (cur = str, newflags = oldflags; *cur; ++cur) {

		MuFlags f;
		if (*cur == '+' || *cur == '-') {
			f = mu_flag_from_char (cur[1]);
			if (f == 0)
				goto error;
			if (*cur == '+')
				newflags  |= f;
			else
				newflags  &= ~f;
			++cur;		
			continue;
		}

		goto error;
	}
	
	return newflags;
error:
	g_warning ("invalid flag string");
	return MU_FLAG_INVALID;
	
}


