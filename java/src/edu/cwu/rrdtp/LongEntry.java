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
import com.sun.jna.NativeLong;

/**
 * Entry used to store the actual data values. Currently a work-in-progress.
 */
public class LongEntry extends Entry
{
    public LongEntry(Pointer self)
	{
		super(self);
	}
	
	public final void Set(long val)
	{
		NativeLibrary.rrdtp_LongEntry_Set(self, new NativeLong(val));
	}
	
	public final long Get()
	{
		return NativeLibrary.rrdtp_LongEntry_Get(self).longValue();
	}
	
	@Override
	public String toString()
	{
		return Long.toString(Get());
	}
}
