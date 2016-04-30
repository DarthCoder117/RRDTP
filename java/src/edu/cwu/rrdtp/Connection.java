/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package edu.cwu.rrdtp;

import com.sun.jna.Pointer;
import com.sun.jna.NativeLong;

/**
 *
 * @author super
 */
public class Connection 
{ 
    private Pointer self = null;
   
    public void startServer()
    {
        startServer(4309);
    }
    
    public void startServer(int port)
    {
        self = NativeLibrary.rrdtp_OpenServerConnection(port);
    }
    
    public void startClient(String ip)
    {
        startClient(ip, 4309);
    }
    
    public void startClient(String ip, int port)
    {
        self = NativeLibrary.rrdtp_OpenClientConnection(ip, port);
    }
    
    public void close()
    {
        NativeLibrary.rrdtp_CloseConnection(self);
    }
    
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
}
