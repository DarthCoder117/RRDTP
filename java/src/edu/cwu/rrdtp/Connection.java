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
 * Manages an RRDTP connection. Can be run in either server or client mode.
 */
public class Connection 
{ 
    private Pointer self = null;
   
    /*
    Starts the connection in server mode on the default port (4309).
    */
    public void startServer()
    {
        startServer(4309);
    }
    /*
    Starts the connection in server mode on a custom port.
    */
    public void startServer(int port)
    {
        self = NativeLibrary.rrdtp_OpenServerConnection(port);
    }
    
    /*
    Connects to a server running RRDTP on the default port (4309).
    */
    public void startClient(String ip)
    {
        startClient(ip, 4309);
    }
    /*
    Connects to a server running RRDTP on a different port number.
    */
    public void startClient(String ip, int port)
    {
        self = NativeLibrary.rrdtp_OpenClientConnection(ip, port);
    }
    
    /*
    Closes the connection. Works for both client and server.
    */
    public void close()
    {
        NativeLibrary.rrdtp_CloseConnection(self);
    }
    
    /*
    Call this periodically to send and receive data.
    */
    public void poll()
    {
        NativeLibrary.rrdtp_PollConnection(self);
    }
    
    public void SetInt(String identifier, int val)
    {
        NativeLibrary.rrdtp_SetInt(self, identifier, val);
    }
    public int GetInt(String identifier)
    {
        return GetInt(identifier, 0);
    }
    public int GetInt(String identifier, int defaultVal)
    {
        return NativeLibrary.rrdtp_GetInt(self, identifier, defaultVal);
    }

    public void SetLong(String identifier, long val)
    {
        NativeLibrary.rrdtp_SetLong(self, identifier, new NativeLong(val));
    }
    public long GetLong(String identifier)
    {
        return GetLong(identifier, 0);
    }
    public long GetLong(String identifier, long defaultVal)
    {
        return NativeLibrary.rrdtp_GetLong(self, identifier, new NativeLong(defaultVal)).longValue();
    }
	
	public void SetFloat(String identifier, float val)
    {
        NativeLibrary.rrdtp_SetFloat(self, identifier, val);
    }
    public float GetFloat(String identifier)
    {
        return GetFloat(identifier, 0.0f);
    }
    public float GetFloat(String identifier, float defaultVal)
    {
        return NativeLibrary.rrdtp_GetFloat(self, identifier, defaultVal);
    }

	public void SetDouble(String identifier, double val)
    {
        NativeLibrary.rrdtp_SetDouble(self, identifier, val);
    }
    public double GetDouble(String identifier)
    {
        return GetDouble(identifier, 0.0);
    }
    public double GetDouble(String identifier, double defaultVal)
    {
        return NativeLibrary.rrdtp_GetDouble(self, identifier, defaultVal);
    }
	
    public void SetBool(String identifier, boolean val)
    {
        NativeLibrary.rrdtp_SetBool(self, identifier, val);
    }
    public boolean GetBool(String identifier)
    {
        return GetBool(identifier, false);
    }
    public boolean GetBool(String identifier, boolean defaultVal)
    {
         return NativeLibrary.rrdtp_GetBool(self, identifier, defaultVal);
    }

    public void SetString(String identifier, String str)
    {
        NativeLibrary.rrdtp_SetString(self, identifier, str);
    }
    public String GetString(String identifier)
    {
        return GetString(identifier, "");
    }
    public String GetString(String identifier, String defaultVal)
    {
         return NativeLibrary.rrdtp_GetString(self, identifier, defaultVal);
    }
	
	public Category GetCategory(String identifier)
	{
		Pointer category = NativeLibrary.rrdtp_GetCategory(self, identifier);
		if (category != null)
		{
			return new Category(category);
		}
		
		return null;
	}

	public Entry GetEntry(String identifier)
	{
		Pointer entry = NativeLibrary.rrdtp_GetEntry(self, identifier);
		if (entry != null)
		{
			return createEntryObject(entry);
		}
		
		return null;
	}
	
	private Entry createEntryObject(Pointer entry)//TODO: Use map to cache entry objects
	{
		int entryType = NativeLibrary.rrdtp_Entry_GetType(entry);
		if (entryType == Entry.EDT_INT)
		{
			return new IntEntry(entry);
		}
		else if (entryType == Entry.EDT_LONG)
		{
			return new LongEntry(entry);
		}
		else if (entryType == Entry.EDT_FLOAT)
		{
			return new FloatEntry(entry);
		}
		else if (entryType == Entry.EDT_DOUBLE)
		{
			return new DoubleEntry(entry);
		}
		else if (entryType == Entry.EDT_BOOLEAN)
		{
			return new BooleanEntry(entry);
		}
		else if (entryType == Entry.EDT_STRING)
		{
			return new StringEntry(entry);
		}
		
		return null;
	}

	public void DeleteEntry(String identifier)
	{
		NativeLibrary.rrdtp_DeleteEntry(self, identifier);
	}
	
	public interface EntryChangeListener
	{
		public void onEntryChanged(Entry entry);
	}
	
	private class ValueChangeCallbackImplementation implements NativeLibrary.ValueChangedCallback
	{
		private EntryChangeListener listener = null;
		
		public ValueChangeCallbackImplementation(EntryChangeListener listener)
		{
			this.listener = listener;
		}
		
		public void invoke(Pointer connection, Pointer entry)
		{
			if (listener!=null)
			{
				listener.onEntryChanged(createEntryObject(entry));
			}
		}
	}
	
	public void SetEntryChangeListener(Pointer connection, EntryChangeListener listener)
	{
		NativeLibrary.rrdtp_SetValueChangedCallback(connection, new ValueChangeCallbackImplementation(listener));
	}
}
