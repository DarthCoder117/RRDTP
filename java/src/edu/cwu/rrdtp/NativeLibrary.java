/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package edu.cwu.rrdtp;

import com.sun.jna.Library;
import com.sun.jna.Native;
import com.sun.jna.NativeLong;
import com.sun.jna.Pointer;
import com.sun.jna.Platform;

public class NativeLibrary 
{
    //Connection
    public static native Pointer rrdtp_OpenServerConnection(int port);
    public static native Pointer rrdtp_OpenClientConnection(String ip, int port);
    
    public static native void rrdtp_CloseConnection(Pointer connection);

    public static native void rrdtp_PollConnection(Pointer connection);
    
    //Entries
    public static native void rrdtp_SetInt(Pointer connection, String identifier, int val);
    public static native int rrdtp_GetInt(Pointer connection, String identifier, int defaultVal);

    public static native void rrdtp_SetLong(Pointer connection, String identifier, NativeLong val);
    public static native NativeLong rrdtp_GetLong(Pointer connection, String identifier, NativeLong defaultVal);

    public static native void rrdtp_SetBool(Pointer connection, String identifier, boolean val);
    public static native boolean rrdtp_GetBool(Pointer connection, String identifier, boolean defaultVal);

    public static native void rrdtp_SetString(Pointer connection, String identifier, String str);
    public static native String rrdtp_GetString(Pointer connection, String identifier, String defaultVal);
    
    static
    {
        Native.register("rrdtp_module");
    }
}