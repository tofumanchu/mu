#compdef mu

## Copyright (C) 2008-2011 Dirk-Jan C. Binnema <djcb@djcbsoftware.nl>
##
## This program is free software; you can redistribute it and/or modify
## it under the terms of the GNU General Public License as published by
## the Free Software Foundation; either version 3 of the License, or
## (at your option) any later version.
##  
## This program is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU General Public License for more details.
##  
## You should have received a copy of the GNU General Public License
## along with this program; if not, write to the Free Software Foundation,
## Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.  

# zsh completion for mu.

_mu_commands() {
  local -a mu_commands
  mu_commands=(
    'index:scan your maildirs and import their metadata in the database'
    'find:search for messages in the database'
    'cfind:search for contacts (name + email) in the database'
    'cleanup:remove deleted messages from the database'
    'extract:list message-parts (attachments) and extract them to files'
    'mkdir:create maildirs'
    'view:display specific messages')

  _describe -t command 'command' mu_commands
}

_mu_common_options=(
    '--debug[output information useful for debugging mu]'
    '--quiet[do not give any non-critical information]'
    '--color[use colors in some of the output]'
    '--version[display mu version and copyright information]'
    '--log-stderr[log to standard error]'
)

_mu_db_options=(
    '--muhome[use some non-default location for the mu database]:directory:_files'
)

_mu_find_options=(
    '--fields[fields to display in the output]'
    '--sortfield[field to sort the output by]'
    '--descending[sort in descending order]'
    '--summary[include a summary of the message]'
    '--bookmark[use a named bookmark]'
    '--output[set the kind of output for the query]'
)

_mu_view_options=(
    '--summary[only show a summary of the message]'
)


_mu_view() {
    _arguments -s : \
        $_mu_common_options \
	$_mu_view_options
}

_mu_extract() {
 _files 
}

_mu_find() {
    _arguments -s : \
        $_mu_common_options \
	$_mu_db_options \
	$_mu_find_options
}

_mu_index() {
    _arguments -s : \
	$_mu_db_options \
        $_mu_common_options
}

_mu_cleanup() {
    _arguments -s : \
	$_mu_db_options \
       $_mu_common_options
}


_mu_mkdir() {
   _arguments -s : \
    '--mode=[file mode for the new Maildir]:file mode: ' \
    $_mu_common_options
}

_mu() {
  if (( CURRENT > 2 )) ; then
      local cmd=${words[2]}
      curcontext="${curcontext%:*:*}:mu-$cmd"
      (( CURRENT-- ))
      shift words
      _call_function ret _mu_$cmd
      return ret
  else
      _mu_commands
  fi
}

_mu "$@"

# Local variables:
# mode: sh
# End:
