// This file is part of osG.
//
// osG is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// osG is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Foobar.  If not, see <https://www.gnu.org/licenses/>.
//
// osG is also available under a commercial license.
// Please contact GIMASI at info@gimasi.ch for further information.
//
#include "osg/utils/Log.h"
#include <stddef.h>

void _osg_error(const char * msg, const char * file, const int line, const char * func)
{
    for(;;)
    {
        // ntd
        if (msg == NULL
            && file == NULL
            && line == 0
            && func == NULL)
            break;
    }
}
