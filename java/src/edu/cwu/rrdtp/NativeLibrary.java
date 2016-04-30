/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package edu.cwu.rrdtp;

import com.sun.jna.Library;
import com.sun.jna.Native;
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
    
    static
    {
        Native.register("rrdtp_module");
    }
}