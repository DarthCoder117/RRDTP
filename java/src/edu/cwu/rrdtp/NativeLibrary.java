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

import com.sun.jna.Native;
import com.sun.jna.NativeLong;
import com.sun.jna.Callback;
import com.sun.jna.Pointer;

/*
Native wrapper for C++ RRDTP library. For internal use only.
*/
class NativeLibrary 
{
	public interface ValueChangedCallback extends Callback 
	{
        void invoke(Pointer connection, Pointer entry);
    }
	
    //Connection
    public static native Pointer rrdtp_OpenServerConnection(int port);
    public static native Pointer rrdtp_OpenClientConnection(String ip, int port);
    
    public static native void rrdtp_CloseConnection(Pointer connection);

    public static native void rrdtp_PollConnection(Pointer connection);
    
    public static native void rrdtp_SetInt(Pointer connection, String identifier, int val);
    public static native int rrdtp_GetInt(Pointer connection, String identifier, int defaultVal);

    public static native void rrdtp_SetLong(Pointer connection, String identifier, NativeLong val);
    public static native NativeLong rrdtp_GetLong(Pointer connection, String identifier, NativeLong defaultVal);
	
	public static native void rrdtp_SetFloat(Pointer connection, String identifier, float val);
    public static native float rrdtp_GetFloat(Pointer connection, String identifier, float defaultVal);

    public static native void rrdtp_SetDouble(Pointer connection, String identifier, double val);
    public static native double rrdtp_GetDouble(Pointer connection, String identifier, double defaultVal);

    public static native void rrdtp_SetBool(Pointer connection, String identifier, boolean val);
    public static native boolean rrdtp_GetBool(Pointer connection, String identifier, boolean defaultVal);

    public static native void rrdtp_SetString(Pointer connection, String identifier, String str);
    public static native String rrdtp_GetString(Pointer connection, String identifier, String defaultVal);
    
	public static native Pointer rrdtp_GetCategory(Pointer connection, String identifier);

	public static native Pointer rrdtp_GetEntry(Pointer connection, String identifier);

	public static native void rrdtp_DeleteEntry(Pointer connection, String identifier);
	
	public static native void rrdtp_SetValueChangedCallback(Pointer connection, ValueChangedCallback callback);
	
	//Entry
	public static native String rrdtp_Entry_GetName(Pointer entry);

	public static native String rrdtp_Entry_GetIdentifier(Pointer entry);

	public static native int rrdtp_Entry_GetType(Pointer entry);

	public static native void rrdtp_IntEntry_Set(Pointer entry, int val);
	public static native int rrdtp_IntEntry_Get(Pointer entry);

	public static native void rrdtp_LongEntry_Set(Pointer entry, NativeLong val);
	public static native NativeLong rrdtp_LongEntry_Get(Pointer entry);

	public static native void rrdtp_FloatEntry_Set(Pointer entry, float val);
	public static native float rrdtp_FloatEntry_Get(Pointer entry);

	public static native void rrdtp_DoubleEntry_Set(Pointer entry, double val);
	public static native double rrdtp_DoubleEntry_Get(Pointer entry);

	public static native void rrdtp_BooleanEntry_Set(Pointer entry, boolean val);
	public static native boolean rrdtp_BooleanEntry_Get(Pointer entry);

	public static native void rrdtp_StringEntry_Set(Pointer entry, String str);
	public static native String rrdtp_StringEntry_Get(Pointer entry);
	
    static
    {
        Native.register("rrdtp_module");
    }
}
