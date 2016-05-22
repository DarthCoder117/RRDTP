/*==============================================================================
The MIT License (MIT)

Copyright (c) 2016 Tanner Mickelson & The RRDTP Team

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
==============================================================================*/

package edu.cwu.rrdtp;

import com.sun.jna.Pointer;

/**
 * Entry used to store the actual data values. Currently a work-in-progress.
 */
public abstract class Entry 
{
	public final int EDT_INT = 0;
	public final int EDT_LONG = 1;
	public final int EDT_FLOAT = 2;
	public final int EDT_DOUBLE = 3;
	public final int EDT_BOOLEAN = 4
	public final int EDT_STRING = 5;
	
    private Pointer self;
	
    public Entry(Pointer self)
	{
		this.self = self;
	}
	
	public final String GetName()
	{
		return NativeLibrary.rrdtp_Entry_GetName(self);
	}
	
	public final String GetIdentifier()
	{
		return NativeLibrary.rrdtp_Entry_GetIdentifier(self);
	}
	
	public final int GetType()
	{
		return NativeLibrary.rrdtp_Entry_GetType(self);
	}
}
