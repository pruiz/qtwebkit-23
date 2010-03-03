/*
 * Copyright (C) 2010 Apple Inc.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 */

import java.applet.Applet;
import com.netscape.javascript.qa.liveconnect.*;
import com.netscape.javascript.qa.lc3.bool.*;
import com.netscape.javascript.qa.lc3.jsnull.*;
import com.netscape.javascript.qa.lc3.jsobject.*;
import com.netscape.javascript.qa.lc3.number.*;
import com.netscape.javascript.qa.lc3.string.*;
import com.netscape.javascript.qa.lc3.undefined.*;

public class lc3 extends Applet {

    public Object createQAObject(String name) throws Exception {
        Class theClass  = Class.forName(name);
        return theClass.newInstance();
    }

    public byte[] stringToByteArray(String string) {
        return string.getBytes();
    }
}
