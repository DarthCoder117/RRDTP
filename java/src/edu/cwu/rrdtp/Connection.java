/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package edu.cwu.rrdtp;

import com.sun.jna.Pointer;

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
}
